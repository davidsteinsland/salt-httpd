#ifndef CONFIG_COMMANDLINE_HPP
#define CONFIG_COMMANDLINE_HPP

#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include <unistd.h>

#include <config.h>
#include <config/configurator.hpp>
#include <config/config_source.hpp>
#include <exceptions.hpp>

#include <iostream>

namespace config {
  /**
   * A Command line option.
   */
  class Option {
    protected:
      /**
       * The character that identifies the option
       */
      char _key;

      /**
       * Description of the option, shown in help screen
       */
      std::string _description;

      /**
       * The configuration value it binds to
       */
      std::string _name;

      /**
       * Whether or not the option is a flag or not
       */
      bool _isFlag;
    public:
      /**
       * Creates a new command line option
       * @param key the option key
       * @param description the description of the option
       * @param name the configuration value it binds to
       * @param isFlag whether or not the option is a flag
       */
      Option(char key, std::string description, std::string name, bool isFlag);

      /**
       * Creates a new command line option that requires an argument
       * @param key the option key
       * @param description the description of the option
       * @param name the configuration value it binds to
       */
      Option(char key, std::string description, std::string name);

      /**
       * Creates a new command line option that requires an argument,
       * but does not bind to a configuration value.
       * @param key the option key
       * @param description the description of the option
       */
      Option(char key, std::string description);

      /**
       * Returns the character key of the option
       * @return the option character
       */
      char getOption();

      /**
       * Returns the description of the option
       * @return the description of the option
       */
      std::string getDescription();

      /**
       * Returns the configuration value it binds to
       * @returns the name of the configuration value it binds to
       */
      std::string getName();

      /**
       * Whether or not the option is a flag
       * @return true is the option does not takes an argument, false otherwise
       */
      bool isFlag();
  };

  /**
   * The Command line options class. This is responsible for adding options, parsing the options
   * and setting the values properly.
   */
  class CommandlineOptions : public ConfigSource {
    protected:
      /**
       * Vector of the options
       */
      std::vector<Option> opts;

      /**
       * Shows the help screen
       */
      void showHelpScreen(const char* program);

      /**
       * Shows information about the package
       */
      void showVersionInformation();
    public:
      /**
       * Creates a new Command line options instance, with the options "-h" for help and "-v" for version
       * information added by default
       */
      CommandlineOptions();

      /**
       * Clears/removes all the options
       */
      ~CommandlineOptions();

      /**
       * Returns the option string required by getopt()
       * @return getopt() compliant option string
       */
      std::string getOptionString();

      /**
       * Adds a new option that should be set via the command line interface
       * @param opt a command line option
       */
      void addOption(Option opt);

      /**
       * Parses the arguments given to the command line interface, setting values accordingly.
       * @param cfg the Configurator instance
       * @param argc the argument count
       * @param argv the argument vector
       * @return 0 if everything is OK, 1 if help screen of version screen should be displayed
       */
      int parse(Configurator* cfg, int argc, char** argv);
  };
};
#endif
