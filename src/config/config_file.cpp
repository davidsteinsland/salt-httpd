#include <config/config_file.hpp>
#include <config/configurator.hpp>
#include <cstdio>

config::ConfigFile::ConfigFile() {

}

config::ConfigFile::~ConfigFile() {

}

void config::ConfigFile::add(std::string name, std::string bindTo) {
  values.insert(std::make_pair(name, bindTo));
}

void config::ConfigFile::add(std::string name) {
  add(name, name);
}

int config::ConfigFile::parse(Configurator* cfg, int argc, char** argv) {
  try {
    std::string config_file = cfg->getString("config.file");

    if (!io::utils::isFile(config_file)) {
      throw FileNotFoundException(config_file + " does not exist");
    }

    try {
      const char* file = config_file.c_str();
      configLib.readFile(file);
    } catch (libconfig::ParseException& ex) {
      std::stringstream ss;
      ss << "Parse error at " << ex.getFile() << ":" << ex.getLine() << " - " << ex.getError();
      throw ParseException(ss.str());
    } catch (libconfig::FileIOException& ex) {
      throw IOException("I/O error while reading config file");
    } catch (...) {
      // "brew install libconfig" causes this weird behavior, that we're unable to catch ParseException
      throw IOException("Unknown error caught at configuration parsing. Probably parse error.");
    }

    for (std::pair<std::string, std::string> pair : values) {
      const char* strval;
      bool boolval;
      int intval;

      if (configLib.lookupValue(pair.first, strval)) {
        setValue(pair.second, std::string(strval));
      } else if (configLib.lookupValue(pair.first, intval)) {
        char buffer[10];
        int k = sprintf(buffer, "%d", intval);
        setValue(pair.second, std::string(buffer, k));
      } else if (configLib.lookupValue(pair.first, boolval)) {
        setValue(pair.second, boolval ? "1" : "0");
      }
    }
  } catch (SettingNotFoundException e) {
    // cant parse without config file
  }

  return 0;
}
