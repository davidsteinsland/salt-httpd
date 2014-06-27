#include <memory>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/thread.h>
#include <stdio.h>
#include <fcntl.h>

#include <ioutils.hpp>
#include <stringutils.hpp>

#include <exceptions.hpp>
#include <config/config_source.hpp>
#include <config/config_descriptor.hpp>
#include <config/config_commandline.hpp>
#include <config/config_file.hpp>
#include <config/config_default.hpp>
#include <config/configurator.hpp>

#include <concurrency/thread_pool.hpp>

static config::Configurator cfg;
static concurrency::ThreadPool thread_pool(5, false);

static const struct table_entry {
  const char *extension;
  const char *content_type;
} content_type_table[] = {
  { "txt", "text/plain" },
  { "css", "text/css" },
  { "js", "application/x-javascript" },
  { "html", "text/html" },
  { "htm", "text/htm" },
  { "gif", "image/gif" },
  { "jpg", "image/jpeg" },
  { "jpeg", "image/jpeg" },
  { "png", "image/png" },
  { NULL, NULL },
};

/* Try to guess a good content-type for 'path' */
static const char* guess_content_type(const char *path) {
  const char *last_period, *extension;
  const struct table_entry *ent;
  last_period = strrchr(path, '.');
  if (!last_period || strchr(last_period, '/'))
    goto not_found; /* no exension */
  
  extension = last_period + 1;
  for (ent = &content_type_table[0]; ent->extension; ++ent) {
    if (!evutil_ascii_strcasecmp(ent->extension, extension))
      return ent->content_type;
  }

  not_found:
    return "text/plain";
}

void handle_request(evhttp_request *req, void* arg) {
  std::string document_root = (const char*)arg;

  auto *outputBuffer = evhttp_request_get_output_buffer(req);
    
  if (!outputBuffer) {
    return;
  }

  const char* reqline(evhttp_request_get_uri(req));

  std::stringstream ss;
  ss << string::utils::chop(document_root, "/") << reqline;

  auto status = HTTP_OK;

  if (io::utils::isDirectory(ss.str())) {
    ss << "index.html";
  }

  if (!io::utils::isFile(ss.str())) {
    ss.str("");
    ss << string::utils::chop(cfg.getString("www.errors"), "/") << "/404.html";
    status = HTTP_NOTFOUND;
  }

  std::string reqfile = ss.str();

  if (!io::utils::isFile(reqfile)) {
    std::cerr << "Not found: " << reqfile << std::endl;

    evbuffer_add_printf(outputBuffer, "404: File not found");
    evhttp_send_reply(req, HTTP_NOTFOUND, "Not Found", outputBuffer);
    return;
  }

  const char *type = guess_content_type(reqfile.c_str());

  int file = open(reqfile.c_str(), O_RDONLY);
  
  evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", type);
  evbuffer_add_file(outputBuffer, file, 0, io::utils::filesize(reqfile));

  evhttp_send_reply(req, status, "", outputBuffer);
}

void handle_request_cb(evhttp_request *req, void* arg) {
  thread_pool.push(handle_request, req, arg);
}


int main(int argc, char** argv) {
  config::ConfigDescriptor cfgdesc;
  
  // only need to add required values
  cfgdesc.add("listen.address", true);
  cfgdesc.add("listen.port", true);
  cfgdesc.add("www.root", true);
  cfgdesc.add("www.errors", true);

  config::DefaultValueSource* defValues = new config::DefaultValueSource();
  defValues->add("listen.address", "127.0.0.1");
  defValues->add("listen.port", "5555");
  defValues->add("www.root", "htdocs");
  defValues->add("www.errors", "errors");

  config::CommandlineOptions* cliOpts = new config::CommandlineOptions();
  cliOpts->addOption(config::Option('a', "The address to bind to", "server.address"));
  cliOpts->addOption(config::Option('p', "The port to listen on", "server.port"));
  cliOpts->addOption(config::Option('d', "Document root", "www.root"));
  cliOpts->addOption(config::Option('e', "Error template directory", "www.errors"));
  cliOpts->addOption(config::Option('c', "Path to configuration file", "config.file"));

  config::ConfigFile* cfgFile = new config::ConfigFile();
  cfgFile->add("server.address", "listen.address");
  cfgFile->add("server.port", "listen.port");
  cfgFile->add("www.root");
  cfgFile->add("www.errors");

  cfg.setDescriptor(cfgdesc);

  cfg.addSource(cliOpts, config::Priority::HIGHEST);
  cfg.addSource(defValues, config::Priority::LOWEST);
  cfg.addSource(cfgFile, config::Priority::HIGH);

  try {
    if (cfg.parse(argc, argv) == 1) {
      return 1;
    }
  } catch (RequiredSettingNotFoundException e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (SettingNotFoundException e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (FileNotFoundException e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (ParseException e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (IOException e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  thread_pool.start();
  
  evthread_use_pthreads();

  struct event_base* base;
  struct evhttp* http;
  struct evhttp_bound_socket* handle;

  base = event_base_new();

  if (!base) {
    std::cerr << "Failed to create event base." << std::endl;
    return 1;
  }

  http = evhttp_new(base);

  if (!http) {
    std::cerr << "Failed to create http server" << std::endl;
    return 1;
  }

  const char* document_root = cfg.getString("www.root").c_str();

   evhttp_set_gencb(http, handle_request_cb, (void*)(document_root));
  handle = evhttp_bind_socket_with_handle(http, cfg.getString("listen.address").c_str(), cfg.getInt("listen.port"));

  if (!handle) {
    std::cerr << "Failed to bind to address." << std::endl;
    return 1;
  }

  std::cout << "Starting server on " << cfg.getString("listen.address") << ":" << cfg.getInt("listen.port") << std::endl;
  
  if (event_base_dispatch(base) == -1) {
    std::cerr << "Failed to start event thread." << std::endl;
    return -1;
  }

  return 0;
}