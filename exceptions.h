#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>


class ModelException : public std::exception {
private:
    std::string message;

public:
    explicit ModelException(const std::string& msg);
    const char* what() const noexcept override;
};

class ValidationError : public std::exception {
private:
    std::string message;
    int errorCode;

public:
    ValidationError(const std::string& msg, int code);
    const char* what() const noexcept override;
    int getErrorCode() const noexcept;
};


#endif // EXCEPTIONS_H

