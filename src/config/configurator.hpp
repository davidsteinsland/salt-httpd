#ifndef CONFIGURATOR_HPP
#define CONFIGURATOR_HPP

#include <string>
#include <vector>
#include <typeinfo>

#include <exceptions.hpp>
#include <config/config_descriptor.hpp>
#include <config/config_source.hpp>

namespace config {
  /**
   * A priority class used to specify the priority a configuration source has,
   * when searching for a value.
   */
  class Priority {
      public:
          /**
           * The highest priority. A configuration source with this value will be asked first
           */
          static int HIGHEST;

          /**
           * The medium priority
           */
          static int HIGH;

          /**
           * The lowest priority. Configuration source with this value will be asked last
           */
          static int LOWEST;
  };

  /**
   * The configuration class is responsible for parsing sources, searching for values
   * and returning their values.
   */
  class Configurator {
    protected:
      /**
       * The descriptor saying which keys are required
       */
      ConfigDescriptor desc;

      /**
       * The vector containing the configuration sources, ordered by
       * priority
       */
      std::vector<std::pair<ConfigSource*, int>> priorities;
    public:
      /**
       * Creates a configuration object
       * @param descriptor the configdescriptor
       */
      Configurator(ConfigDescriptor descriptor);

      /**
       * Creates a new configuration object, without a descriptor
       */
      Configurator();

      /**
       * Deletes the configuration sources
       */
      ~Configurator();

      /**
       * Adds a new configuration source with a specified priority.
       * @param src the configuration source to add
       * @param priority the priority of the source
       */
      void addSource(ConfigSource* src, int priority);

      /**
       * Adds a new configuration source with no priority
       * @param src the configuration source to add
       */
      void addSource(ConfigSource* src);

      /**
       * Sets the configuration descriptor
       * @param descriptor the configuration descriptor
       */
      void setDescriptor(ConfigDescriptor descriptor);

      /**
       * Parses the configuration sources
       * @param argc the argument count
       * @param argv the argument vector
       * @return 0 if everything when OK, 1 otherwise
       */
      int parse(int argc, char** argv);

      /**
       * Checks whether or not the configurator can find a value for the key
       * @param path the configuration key
       * @return true if the key has a value
       */
      bool hasValue(std::string path);

      /**
       * Returns a configuration value as a string
       * @param path the configuration key
       * @return the configuration value
       */
      std::string getString(std::string path);

      /**
       * Returns a configuration value as an int
       * @param path the configuration key
       * @return the configuration value
       */
      int getInt(std::string path);

      /**
       * Returns a configuration value as a bool
       * @param path the configuration key
       * @return true if the configuration value equals "true" or "1", false otherwise
       */
      bool getBool(std::string path);
  };
};
#endif
