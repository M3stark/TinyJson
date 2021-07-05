#include "json_val.h"   
// #include <variant> -> since C++17
#include "json_except.h"

namespace zzjson {  // ------------------- namespace zzjson


/**
 * 数据类型接口
 */
JsonType JsonValue::getType() const noexcept {
    // variant 是 C++17 所提供的变体类型.
    // variant<X, Y, Z> 是可存放 X, Y, Z 这三种类型数据的变体类型.
    // std::holds_alternative<T>(v) 可查询变体类型 v 是否存放了 T 类型的数据.
    if (std::holds_alternative<std::nullptr_t>(_val)) {
        return JsonType::m_nullptr;
    } else if (std::holds_alternative<bool>(_val)) {
        return JsonType::m_bool;
    } else if (std::holds_alternative<double>(_val)) {
        return JsonType::m_number;
    } else if (std::holds_alternative<std::string>(_val)) {
        return JsonType::m_string;
    } else if (std::holds_alternative<Json::_array>(_val)) {
        return JsonType::m_array;
    } else {
        return JsonType::m_obj;
    }
}

/**
 * array/obj的 size() 接口
 */
size_t JsonValue::size() const {
    if (std::holds_alternative<Json::_array>(_val)) {
        return std::get<Json::_array>(_val).size();
    } else if (std::holds_alternative<Json::_obj>(_val)) {
        return std::get<Json::_obj>(_val).size();
    } else {
        throw JsonExcept("Error! Not a array or object!");
    }
}

/**
 * 随机访问 array
 */
const Json& JsonValue::operator[](size_t pos) const {
    if (std::holds_alternative<Json::_array>(_val)) {
        return std::get<Json::_array>(_val)[pos];
    } else {
        throw JsonExcept("Error! Not a array!");
    }
}

/**
 * Notes: C++中的强制类型转换
 * -------------------------------------------------------------------------------
 * ref: https://blog.csdn.net/mikasoi/article/details/81071764
 * > const_cast: 从类中移除const、volatile和__unaligned特性.
 * > static_cast: 基本类型转换.(static_cast不执行运行时类型检查，而dynamic_cas执行该检查.)
 * > daynamic_cast: 多态类之间的类型转换.
 * > reinterpreter_cast: 不同类型的指针类型转换.
 */
Json& JsonValue::operator[](size_t pos) {
    return const_cast<Json&>(static_cast<const JsonValue&>(*this)[pos]);
}

/**
 * O(1)访问 array
 */
const Json& JsonValue::operator[](const std::string& key) const {
    if (std::holds_alternative<Json::_obj>(_val)) {
        return std::get<Json::_obj>(_val).at(key);
    } else {
        throw JsonExcept("Error! Not a object!");
    }
}

Json& JsonValue::operator[](const std::string& key) {
    return const_cast<Json&>(static_cast<const JsonValue&>(*this)[key]);
}

/**
 * 转换接口
 */
std::nullptr_t JsonValue::toNull() const {
    try {
        return std::get<std::nullptr_t>(_val);
    }
    // class bad_variant_access : public std::exception (sice C++17)
    // ----------------------------------------------------------------
    // std::bad_variant_access 是下列情形中抛出的异常类型：
    // 以不匹配当前活跃可选项的下标或类型调用 std::get(std::variant).
    // 调用 std::visit 观览因异常无值 (valueless_by_exception) 的 variant.
    catch (const std::bad_variant_access&) {
        throw JsonExcept("Error! Not a null!");
    }
}

bool JsonValue::toBool() const {
    try {
        return std::get<bool>(_val);
    } catch (const std::bad_variant_access&) {
        throw JsonExcept("Error! Not a bool!");
    }
}

double JsonValue::toDouble() const {
    try {
        return std::get<double>(_val);
    } catch (const std::bad_variant_access&) {
        throw JsonExcept("Error! Not a double!");
    }
}

const std::string& JsonValue::toString() const {
    try {
        return std::get<std::string>(_val);
    }
    catch (const std::bad_variant_access&) {
        throw JsonExcept("Error! Not a string!");
    }
}

const Json::_array& JsonValue::toArray() const {
    try {
        return std::get<Json::_array>(_val);
    }
    catch (const std::bad_variant_access&) {
        throw JsonExcept("Error! Not a array!");
    }
}

const Json::_obj& JsonValue::toObj() const {
    try {
        return std::get<Json::_obj>(_val);
    }
    catch (const std::bad_variant_access&) {
        throw JsonExcept("Error! Not a object!");
    }
}


};      // ------------------- namespace zzjson