#include <config/config_descriptor.hpp>

void config::ConfigDescriptor::add(std::string name, bool required) {
  keys.insert(std::make_pair(name, required));
}

void config::ConfigDescriptor::add(std::string name) {
  add(name, false);
}

void config::ConfigDescriptor::verify(std::vector<std::pair<ConfigSource*, int>> sources) {
  if (!keys.size()) {
    return;
  }

  for (auto it = begin(keys); it != end(keys); ++it) {
    std::string val = it->first;
    bool required = it->second;
    bool found = false;

    if (!required) {
      continue;
    }

    for (auto srcit = begin(sources); srcit != end(sources); ++srcit) {
      ConfigSource* src = (*srcit).first;

      if (src->hasValue(val)) {
        found = true;
        break;
      }
    }

    if (!found) {
      throw RequiredSettingNotFoundException("Did not find required value for " + val);
    }
  }
}

bool config::ConfigDescriptor::hasKey(std::string name) {
  return keys.find(name) != keys.end();
}
