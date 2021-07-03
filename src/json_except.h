#ifndef JSON_EXCEPT_H__
#define JSON_EXCEPT_H__

#pragma once

#include <stdexcept>

namespace zzjson {  // ------------------- namespace zzjson

class JsonExcept : public std::runtime_error {
public:
    explicit JsonExcept(const std::string& errMsg) : runtime_error(errMsg) {}

    const char* what() const noexcept override {
        return runtime_error::what();
    };
};

};                  // ------------------- namespace zzjson



#endif  // JSON_EXCEPT_H__