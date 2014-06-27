#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <sstream>

class BaseException : public std::exception {
  protected:
    int err_code;
    std::string err_message;
    static const int DEFAULT_ERR_CODE = -100;

  public:
    BaseException(std::string msg) : err_code(DEFAULT_ERR_CODE), err_message(msg) {
    }

    BaseException(int err, std::string msg) : err_code(err), err_message(msg) {
    }

    ~BaseException() throw() {
    }

    virtual const char* what() const throw() {
      return err_message.c_str();
    }

    int code() {
      return err_code;
    }
};

class ThreadPoolException : public BaseException {
  public:
    ThreadPoolException(std::string msg) : BaseException(msg) {
    }

    ThreadPoolException(int err, std::string msg) : BaseException(err, msg) {
    }
};

class ThreadShutdownException : public ThreadPoolException {
  public:
    ThreadShutdownException(std::string msg) : ThreadPoolException(msg) {
    }

    ThreadShutdownException(int err, std::string msg) : ThreadPoolException(err, msg) {
    }
};

class NotFoundException : public BaseException {
  public:
    NotFoundException(std::string msg) : BaseException(msg) {
    }

    NotFoundException(int err, std::string msg) : BaseException(err, msg) {
    }
};

class IOException : public BaseException {
  public:
    IOException(std::string msg) : BaseException(msg) {
    }

    IOException(int err, std::string msg) : BaseException(err, msg) {
    }
};

class HttpException : public IOException {
  public:
    HttpException(std::string msg) : IOException(msg) {
    }

    HttpException(int err, std::string msg) : IOException(err, msg) {
    }
};

class BadRequestException : public HttpException {
  public:
    BadRequestException(std::string msg) : HttpException(msg) {
    }

    BadRequestException(int err, std::string msg) : HttpException(err, msg) {
    }
};

class FileNotFoundException : public IOException {
  public:
    FileNotFoundException(std::string msg) : IOException(msg) {
    }

    FileNotFoundException(int err, std::string msg) : IOException(err, msg) {
    }
};

class ParseException : public IOException {
  public:
    ParseException(std::string msg) : IOException(msg) {
    }

    ParseException(int err, std::string msg) : IOException(err, msg) {
    }
};

class SettingNotFoundException : public ParseException {
  public:
    SettingNotFoundException(std::string msg) : ParseException(msg) {
    }

    SettingNotFoundException(int err, std::string msg) : ParseException(err, msg) {
    }
};

class RequiredSettingNotFoundException : public SettingNotFoundException {
  public:
    RequiredSettingNotFoundException(std::string msg) : SettingNotFoundException(msg) {
    }

    RequiredSettingNotFoundException(int err, std::string msg) : SettingNotFoundException(err, msg) {
    }
};

class ConfigurationException : public BaseException {
  public:
    ConfigurationException(std::string msg) : BaseException(msg) {
    }

    ConfigurationException(int err, std::string msg) : BaseException(err, msg) {
    }
};

#endif
