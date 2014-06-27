#include <config/configurator.hpp>

int config::Priority::HIGHEST = 10;
int config::Priority::HIGH = 5;
int config::Priority::LOWEST = 0;

config::Configurator::Configurator(ConfigDescriptor descriptor) : desc(descriptor) {

}

config::Configurator::Configurator() {

}

config::Configurator::~Configurator() {
  for (auto it = begin(priorities); it != end(priorities); ++it) {
    ConfigSource* src = (*it).first;
    delete src;
  }
}

void config::Configurator::addSource(ConfigSource* src, int priority) {
  priorities.push_back(std::make_pair(src, priority));
}

void config::Configurator::addSource(ConfigSource* src) {
  priorities.push_back(std::make_pair(src, Priority::HIGH));
}

void config::Configurator::setDescriptor(ConfigDescriptor descriptor) {
  desc = descriptor;
}

bool config::Configurator::hasValue(std::string path) {
  for (auto it = priorities.begin(); it != priorities.end(); ++it) {
    ConfigSource* src = (*it).first;

    if (src->hasValue(path)) {
      return true;
    }
  }

  return false;
}

std::string config::Configurator::getString(std::string path) {
  for (auto it = priorities.begin(); it != priorities.end(); ++it) {
    ConfigSource* src = (*it).first;

    if (src->hasValue(path)) {
      return src->getValue(path);
    }
  }

  throw SettingNotFoundException("No source has bound to the key <" + path + ">, or has values for it!");
}

int config::Configurator::getInt(std::string path) {
  return atoi(getString(path).c_str());
}

bool config::Configurator::getBool(std::string path) {
  std::string val = getString(path);
  return val == "true" || val == "1";
}

int config::Configurator::parse(int argc, char** argv) {
  for (auto it = priorities.begin(); it != priorities.end(); ++it) {
    ConfigSource* src = (*it).first;

    if (src->parse(this, argc, argv) == 1) {
      return 1;
    }
  }

  desc.verify(priorities);

  // sort high to low
  int len = priorities.size();
  for (int i = 0; i < len - 1; ++i) {
    bool sort_done = true;

    for (int j = 0; j < len - 1 - i; ++j) {
      int p = priorities[j].second;
      int q = priorities[j + 1].second;

      if (p < q) {
        std::pair<ConfigSource*, int> t = priorities[j + 1];

        priorities[j + 1] = priorities[j];
        priorities[j] = t;

        sort_done = false;
      }
    }

    if (sort_done) {
      break;
    }
  }

  return 0;
}
