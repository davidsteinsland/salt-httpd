#ifndef CONFIG_DESCRIPTOR_HPP
#define CONFIG_DESCRIPTOR_HPP

#include <string>
#include <map>
#include <utility>
#include <vector>

#include <exceptions.hpp>
#include <config/config_source.hpp>

namespace config {
  /**
   * The ConfigDescriptor is meant to describe which configuration values are
   * required by the application.
   */
  class ConfigDescriptor {
    protected:
      /**
       * The map storing the configuration keys, and a bool stating it is required
       * or not
       */
      std::map<std::string, bool> keys;
    public:
      /**
       * Adds a new required configuration key
       * @param name the configuration key
       * @required whether or not the configuration key is required
       */
      void add(std::string name, bool required);

      /**
       * Adds a new configuration key that is optional
       * @param name the configuration key
       */
      void add(std::string name);

      /**
       * Verifies that all the required configuration keys has got values
       * @param sources a vector containing the different configuration sources
       */
      void verify(std::vector<std::pair<ConfigSource*, int>> sources);

      /**
       * Checks whether or not the configuration key exists in the descriptor
       * @param name the configuration key to search for
       * @return true if the key exists, false otherwise
       */
      bool hasKey(std::string name);
  };
};
#endif
