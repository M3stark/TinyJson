
#include <cstdio>
#include "json.h"
#include "json_val.h"
#include "parse.h"

namespace zzjson {  // ------------------- namespace zzjson

/**
 * 构造函数
 */
Json::Json(std::nullptr_t) : _jsonVal(std::make_unique<JsonValue>(nullptr)) {}

Json::Json(bool val) : _jsonVal(std::make_unique<JsonValue>(val)) {}

Json::Json(double val) : _jsonVal(std::make_unique<JsonValue>(val)) {}

Json::Json(const std::string& val) :
            _jsonVal(std::make_unique<JsonValue>(val)) {}

Json::Json(std::string&& val) : _jsonVal(std::make_unique<JsonValue>(val)) {}

Json::Json(const _array& val) : _jsonVal(std::make_unique<JsonValue>(val)) {}

Json::Json(_array&& val) : _jsonVal(std::make_unique<JsonValue>(val)) {}

Json::Json(const _obj& val) : _jsonVal(std::make_unique<JsonValue>(val)) {}

Json::Json(_obj&& val) : _jsonVal(std::make_unique<JsonValue>(val)) {}

/**
 * 析构函数
 */
Json::~Json() = default;

/**
 * 拷贝构造
 */
Json::Json(const Json& rhs) {
    switch (rhs.getType())
    {
    case JsonType::m_nullptr: {
        /* nullptr */
        _jsonVal = std::make_unique<JsonValue>(nullptr);
        break;
    }
    case JsonType::m_bool: {
        /* bool */
        _jsonVal = std::make_unique<JsonValue>(rhs.toBool());
        break;
    }
    case JsonType::m_number: {
        /* double */
        _jsonVal = std::make_unique<JsonValue>(rhs.toDouble());
        break;
    }
    case JsonType::m_string: {
        /* string */
        _jsonVal = std::make_unique<JsonValue>(rhs.toString());
        break;
    }
    case JsonType::m_array: {
        /* array */
        _jsonVal = std::make_unique<JsonValue>(rhs.toArray());
        break;
    }
    case JsonType::m_obj: {
        /* object */
        _jsonVal = std::make_unique<JsonValue>(rhs.toObj());
        break;
    }
    
    default: {
        break;
    }

    }
}

/**
 * 拷贝赋值
 */
Json& Json::operator= (const Json& rhs) noexcept {
    Json temp(rhs);
    swap(temp);
    return *this;
}

};