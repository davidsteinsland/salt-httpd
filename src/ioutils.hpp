#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

namespace io {
  /**
   * Utility class to perform I/O operations.
   */
  class utils {
    public:
      /**
       * Runs the file size of a file
       */
      static std::ifstream::pos_type filesize(std::string filename) {
        std::ifstream in(filename.c_str(), std::ifstream::binary | std::ifstream::ate);
        return in.tellg(); 
      }

      /**
       * Checks whether or not the given pathname is a file on
       * the file system.
       * @param filename the path to the gile
       * @return true if the path is a file, false otherwise
       */
      static bool isFile (std::string filename) {
        if (isDirectory (filename)) {
          return false;
        }

        std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
        if (in) {
          in.close();
          return true;
        }

        return false;
      }

      /**
       * Checks whether or not the pathname is a directory
       * on the file system.
       * @param path the path to the directory
       * @return true if the path is a directory, false otherwise
       */
      static bool isDirectory (std::string path) {
        struct stat st;
        if (stat(path.c_str(), &st) == 0 && S_ISDIR (st.st_mode)) {
          return true;
        }

        return false;
      }
  };
};

#endif
