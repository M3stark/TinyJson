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

Json::Json(const std::string& val)
    : _jsonVal(std::make_unique<JsonValue>(val)) {}

Json::Json(std::string&& val)
    : _jsonVal(std::make_unique<JsonValue>(std::move(val))) {}

Json::Json(const _array& val) : _jsonVal(std::make_unique<JsonValue>(val)) {}

Json::Json(_array&& val)
    : _jsonVal(std::make_unique<JsonValue>(std::move(val))) {}

Json::Json(const _obj& val) : _jsonVal(std::make_unique<JsonValue>(val)) {}

Json::Json(_obj&& val)
    : _jsonVal(std::make_unique<JsonValue>(std::move(val))) {}

/**
 * 析构函数
 */
Json::~Json() = default;

/**
 * 拷贝构造
 */
Json::Json(const Json& rhs) {
    switch (rhs.getType()) {
        case JsonType::m_nullptr: {
            _jsonVal = std::make_unique<JsonValue>(nullptr);
            break;
        }
        case JsonType::m_bool: {
            _jsonVal = std::make_unique<JsonValue>(rhs.toBool());
            break;
        }
        case JsonType::m_number: {
            _jsonVal = std::make_unique<JsonValue>(rhs.toDouble());
            break;
        }
        case JsonType::m_string: {
            _jsonVal = std::make_unique<JsonValue>(rhs.toString());
            break;
        }
        case JsonType::m_array: {
            _jsonVal = std::make_unique<JsonValue>(rhs.toArray());
            break;
        }
        case JsonType::m_obj: {
            _jsonVal = std::make_unique<JsonValue>(rhs.toObj());
            break;
        }
        default: { break; }
    }
}

/**
 * 拷贝赋值
 */
Json& Json::operator=(const Json& rhs) noexcept {
    // copy && swap
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
        Parser p(content);
        return p.parse();
    } catch (JsonExcept& e) {
        errMsg = e.what();
        return Json(nullptr);
    }
}

std::string Json::serialize() const noexcept {
    switch (_jsonVal->getType()) {
        case JsonType::m_nullptr:
            return "null";
        case JsonType::m_bool:
            return _jsonVal->toBool() ? "true" : "false";
        case JsonType::m_number:
            char buf[32];
            snprintf(
                buf, sizeof(buf), "%.17g",
                _jsonVal
                    ->toDouble());  // enough to convert a double to a string
            return std::string(buf);
        case JsonType::m_string:
            return SerializeString();
        case JsonType::m_array:
            return SerializeArray();
        default:
            return SerializeObject();
    }
}

/**
 * 类型接口
 */
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

/**
 * 类型转换接口
 */
bool Json::toBool() const { 
    return _jsonVal->toBool(); 
}

double Json::toDouble() const { 
    return _jsonVal->toDouble(); 
}

const std::string& Json::toString() const { 
    return _jsonVal->toString(); 
}

const Json::_array& Json::toArray() const { 
    return _jsonVal->toArray(); 
}

const Json::_obj& Json::toObj() const { 
    return _jsonVal->toObj(); 
}

/**
 * 访问 array / obj 的接口
 */
size_t Json::size() const {
    return _jsonVal->size();
}

Json& Json::operator[](size_t pos) { 
    return _jsonVal->operator[](pos); 
    }

const Json& Json::operator[](size_t pos) const {
    return _jsonVal->operator[](pos);
}

Json& Json::operator[](const std::string& key) {
    return _jsonVal->operator[](key);
}

const Json& Json::operator[](const std::string& key) const {
    return _jsonVal->operator[](key);
}

void Json::swap(Json& rhs) noexcept {
    // std::move() -> 将一个左值强制转化为右值引用. (C++11)
    // std::move() 相当于一个类型转换：static_cast<T&&>(lvalue).
    // std::swap() -> 只移动而不去复制，从而缩小交换的代价. (C++11)
    using std::swap;
    swap(_jsonVal, rhs._jsonVal);
}


std::string Json::SerializeString() const noexcept {
    std::string res = "\"";
    for (auto e : _jsonVal->toString()) {
        switch (e) {
            case '\"':
                res += "\\\"";
                break;
            case '\\':
                res += "\\\\";
                break;
            case '\b':
                res += "\\b";
                break;
            case '\f':
                res += "\\f";
                break;
            case '\n':
                res += "\\n";
                break;
            case '\r':
                res += "\\r";
                break;
            case '\t':
                res += "\\t";
                break;
            default:
                // int sprintf(char *str, const char *format, ...)
                    // 发送格式化输出到 str 所指向的字符串
                if (static_cast<unsigned char>(e) < 0x20) {
                    char buf[7];
                    sprintf(buf, "\\u%04X", e);
                    res += buf;
                } else
                    res += e;
        }
    }
    return res + '"';
}

std::string Json::SerializeArray() const noexcept {
    std::string res = "[ ";
    for (size_t i = 0; i != _jsonVal->size(); ++i) {
        if (i > 0) {
            res += ", ";
        }
        res += (*this)[i].serialize();
    }
    return res + " ]";
}

std::string Json::SerializeObject() const noexcept {
    std::string res = "{ ";
    bool first = true;  // indicate now is the first object
    for (auto&& p : _jsonVal->toObj()) {
        if (first) {
            first = false;
        } else {
            res += ", ";
        }
        res += "\"" + p.first + "\"";
        res += ": ";
        res += p.second.serialize();
    }
    return res + " }";
}

bool operator==(const Json& lhs, const Json& rhs) {
    if (lhs.getType() != rhs.getType()) {
        return false;
    }
    switch (lhs.getType()) {
        case JsonType::m_nullptr: {
            return true;
        }
        case JsonType::m_bool: {
            return lhs.toBool() == rhs.toBool();
        }
        case JsonType::m_number: {
            return lhs.toDouble() == rhs.toDouble();
        }
        case JsonType::m_string: {
            return lhs.toString() == rhs.toString();
        }
        case JsonType::m_array: {
            return lhs.toArray() == rhs.toArray();
        }
        case JsonType::m_obj: {
            return lhs.toObj() == rhs.toObj();
        }
        default: { return false; }
    }
}

};  // ------------------- namespace zzjson