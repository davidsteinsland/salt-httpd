#ifndef CONFIG_SOURCE_HPP
#define CONFIG_SOURCE_HPP

#include <string>
#include <map>
#include <utility>
#include <vector>

namespace config {
  class Configurator;

  /**
   * Describes a source for configuration values
   */
  class ConfigSource {
    protected:
      /**
       * The map containing the values for the configuration keys
       */
      std::map<std::string, std::string> values;
    public:
      ConfigSource();

      virtual ~ConfigSource();

      /**
       * Sets a value for a configuration key
       * @param key the configuration key
       * @param value the value to set
       */
      void setValue(std::string key, std::string value);

      /**
       * Checks whether or not a configuration value has got a value
       * @param name the configuration key
       * @return true if the configuration key has a value, false otherwise
       */
      bool hasValue(std::string name);

      /**
       * Returns the value of the configuration key
       * @param name the configuration key
       * @return the value of the configuraiton key
       */
      std::string getValue(std::string name);

      /**
       * Virtual parse method. Called when values/options should be parsed
       * and fetched.
       * 
       * Configurator is passed down in case the ConfigSource is
       * dependant on some config values and needs to fetch them.
       */
      virtual int parse(Configurator*, int, char**) = 0;
  };
};
#endif
