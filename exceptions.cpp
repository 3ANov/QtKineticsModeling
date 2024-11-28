#include "exceptions.h"

ModelException::ModelException(const std::string& msg) : message(msg) {}


const char* ModelException::what() const noexcept {
    return message.c_str();
}


ValidationError::ValidationError(const std::string& msg, int code)
    : message(msg), errorCode(code) {}

const char* ValidationError::what() const noexcept {
    return message.c_str();
}

int ValidationError::getErrorCode() const noexcept {
    return errorCode;
}
