#ifndef DEFAULT_VALUE_SOURCE_HPP
#define DEFAULT_VALUE_SOURCE_HPP

#include <string>
#include <vector>

#include <config/config_source.hpp>
#include <config/configurator.hpp>

namespace config {
  /**
   * A default value source. It sets a default value for a configuration value
   */
  class DefaultValueSource : public ConfigSource {
    public:
      /**
       * Adds a new value to the configuration key
       * @param name the configuration key
       * @param val the value to set it to
       */
      void add(std::string name, std::string val);

      /**
       * Does nothing
       */
      int parse(Configurator*, int, char**);
  };
};
#endif
