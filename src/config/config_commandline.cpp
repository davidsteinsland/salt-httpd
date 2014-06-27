#include <config/config_commandline.hpp>

config::Option::Option(char key, std::string description, std::string name, bool isFlag) : _key(key), _description(description),
  _name(name), _isFlag(isFlag) {
}

config::Option::Option(char key, std::string description, std::string name) : _key(key), _description(description),
  _name(name), _isFlag(false) {
}

config::Option::Option(char key, std::string description) : _key(key), _description(description),
  _name(""), _isFlag(false) {
}

char config::Option::getOption() {
  return _key;
}

std::string config::Option::getDescription() {
  return _description;
}

std::string config::Option::getName() {
  return _name;
}

bool config::Option::isFlag() {
  return _isFlag;
}

void config::CommandlineOptions::showHelpScreen(const char* program) {
  std::stringstream ss;
  ss << "Usage: " << program << std::endl;

  for (Option c : opts) {
    ss << "-" << c.getOption() << "\t" << c.getDescription() << std::endl;
  }

  std::cout << ss.str() << std::endl;
}

void config::CommandlineOptions::showVersionInformation() {
  std::stringstream ss;
  ss << PACKAGE_NAME << " version " << PACKAGE_VERSION << std::endl;
  ss << "Configured with: " << CONFIG_FLAGS << std::endl << std::endl;
  ss << "Report bugs to: " << PACKAGE_BUGREPORT << std::endl;

  std::cout << ss.str();
}

config::CommandlineOptions::CommandlineOptions() {
  addOption(Option('h', "Shows this help screen", "", true));
  addOption(Option('v', "Shows version information", "", true));
}

config::CommandlineOptions::~CommandlineOptions() {

}

std::string config::CommandlineOptions::getOptionString() {
  // they must be listed alphabetically ...?
  std::stringstream ss;
  for (Option opt : opts) {
    ss << opt.getOption();

    // opt requires a value
    if (!opt.isFlag()) {
      ss << ":";
    }
  }

  return ss.str();
}

void config::CommandlineOptions::addOption(Option opt) {
  opts.push_back(opt);

  for (int i = opts.size() - 1; i > 0; --i) {
    Option next = opts[i - 1];

    if (next.getOption() > opt.getOption()) {
      opts[i] = next;
      opts[i - 1] = opt;
    }
  }
}

int config::CommandlineOptions::parse(config::Configurator* cfg, int argc, char** argv) {
  // don't let getopt() print error messages
  //opterr = 0;

  int c;
  std::string parsed_opts = getOptionString();
  const char* optstring = parsed_opts.c_str();
  while ((c = getopt(argc, argv, optstring)) != -1) {
    if (c == '?') {
      char err_msg[40];
      int n = 0;

      n = sprintf(err_msg, "Unknown option character %c.", optopt);

      for (Option opt : opts) {
          if (opt.getOption() == optopt && !opt.isFlag()) {
              n = sprintf(err_msg, "Option -%c requires an argument.", optopt);
              break;
          }
      }

      std::cerr << std::string(err_msg, 0, n) << std::endl;
      return 1;
    }

    if (c == 'h') {
      showHelpScreen(argv[0]);
      return 1;
    }

    if (c == 'v') {
      showVersionInformation();
      return 1;
    }

    for (auto it = opts.begin(); it != opts.end(); ++it) {
      Option optit = *it;

      if (c == optit.getOption()) {
        if (optit.isFlag()) {
          setValue(optit.getName(), "true");
        } else {
          setValue(optit.getName(), std::string(optarg));
        }
        break;
      }
    }
  }

  return 0;
}
