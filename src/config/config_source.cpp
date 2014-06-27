#include <config/config_source.hpp>

config::ConfigSource::ConfigSource() {

}
config::ConfigSource::~ConfigSource() {

}

void config::ConfigSource::setValue(std::string key, std::string value) {
  values.insert(std::make_pair(key, value));
}

bool config::ConfigSource::hasValue(std::string name) {
  return values.find(name) != values.end();
}

std::string config::ConfigSource::getValue(std::string name) {
  return values[name];
}
