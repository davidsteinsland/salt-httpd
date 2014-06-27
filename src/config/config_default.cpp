#include <config/config_default.hpp>

void config::DefaultValueSource::add(std::string name, std::string val) {
  setValue(name, val);
}

int config::DefaultValueSource::parse(config::Configurator*, int, char**) {
  return 0;
}
