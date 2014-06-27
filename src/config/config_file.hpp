#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP

#include <map>
#include <utility>

#include <libconfig.h++>

#include <exceptions.hpp>
#include <config/config_source.hpp>
#include <ioutils.hpp>

namespace config {
  /**
   * A configuration source that parses a file for values
   */
  class ConfigFile : public ConfigSource {
    protected:
      /**
       * The libconfig instance
       */
      libconfig::Config configLib;

      /**
       * A map containing a key-value pair to the configuration keys and values
       */
      std::map<std::string, std::string> values;
    public:
      /**
       * Creates a new configuration file instance
       */
      ConfigFile();

      /**
       * Clears any allocated memory
       */
      ~ConfigFile();

      /**
       * Adds a new configuration key that should be read from the configuration file.
       * @param name the configuration key found in the config file
       * @param bindTo the configuration key to bind to
       */
      void add(std::string name, std::string bindTo);

      /**
       * Adds a new configuration key that should be read from the configuration file,
       * that binds to the configuration key of the same name
       * @param name the configuration key
       */
      void add(std::string name);

      /**
       * Parses the configuration file and reads the values into a map
       * @param cfg the configurator instance
       * @param argc the argument count
       * @param argv the argument vector
       * @return 0 if everything's OK, 1 otherwise
       */
      int parse(Configurator* cfg, int argc, char** argv);
  };
};
#endif
