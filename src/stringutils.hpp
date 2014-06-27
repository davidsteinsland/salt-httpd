#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <algorithm>
#include <ioutils.hpp>

namespace string {
  /**
   * String utility class.
   */
  class utils {
    public:
      /**
       * Trims whitespaces from the ends of the string
       * @param t the string to trim
       * @return the trimmed string
       */
      static std::string trim(std::string& t) {
        return chop(t, " \n\r\t");
      }

      static void trimWhitespace(std::string &t) {
        t.erase(std::remove_if(t.begin(), t.end(), [](char x){
          return std::isspace(x);
        }), t.end());
      }

      /**
       * Chops away the specified characters from the string
       * @param t the string to chop
       * @param ws the characters to remove
       * @return the chopped string
       */
      static std::string chop(const std::string &t, const std::string &ws) {
        std::string str = t;
        size_t found = str.find_last_not_of(ws);

        if (found != std::string::npos) {
          str.erase(found + 1);
        } else {
          str.clear();
        }

        return str;
      }
  };
};

#endif
