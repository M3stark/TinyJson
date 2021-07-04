
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

/**
 * 移动构造函数
 */
Json::Json(Json&& rhs) noexcept = default;

Json& Json::operator=(Json&& rhs) noexcept = default;

/**
 * parse()      -> 解析接口
 * serialize()  -> 序列化接口
 * errMsg       -> 存储异常消息
 */
Json Json::parse(const std::string& content, std::string& errMsg) noexcept {
    try {
        Paser p(content);
        return p.parse();
    }
    catch (JsonExcept& e) {
        errMsg = e.what();
        return Json(nullptr);
    }
}

std::string Json::serialize() const noexcept {
    switch (_jsonVal->getType())
    {
    case JsonType::m_nullptr:{
        /* code */
        return "null";
    }
    case JsonType::m_bool:{
        /* code */
        return _jsonVal->toBool() ? "true" : "false";
    }
    case JsonType::m_number:{
        /* code */
        char buf[32];
        snprintf(buf, sizeof(buf), "%.17g", _jsonVal->toDouble());
        break;
    }
    case JsonType::m_string:{
        /* code */
        return SerializeString();
        break;
    }
    case JsonType::m_array:{
        /* code */
        return SerializeArray();
        break;
    }
    default:
        return SerializeObject();
    }
}

JsonType Json::getType() const noexcept {
    return _jsonVal->getType();
}

bool Json::isNull() const noexcept {
    return getType() == JsonType::m_nullptr;
}

bool Json::isBool() const noexcept {
    return getType() == JsonType::m_bool;
}

bool Json::isNumber() const noexcept {
    return getType() == JsonType::m_number;
}

bool Json::isString() const noexcept {
    return getType() == JsonType::m_string;
}

bool Json::isArray() const noexcept {
    return getType() == JsonType::m_array;
}

bool Json::isObject() const noexcept {
    return getType() == JsonType::m_obj;
}


};