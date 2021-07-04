#ifndef JSON_VAL_H__
#define JSON_VAL_H__

#pragma once

#include <variant>  // since C++17
#include "json.h"
#include "json_except.h"

namespace zzjson {  // ------------------- namespace zzjson

class JsonValue {
public:
    /**
     * 构造函数
     * 
     * tips: explicit -> 禁用只有一个参数的构造函数的隐式调用
     */
    explicit JsonValue(std::nullptr_t) : _val(nullptr) {}
    explicit JsonValue(bool val) : _val(val) {}
    explicit JsonValue(double val) : _val(val) {}
    explicit JsonValue(const std::string val) : _val(val) {}
    explicit JsonValue(const Json::_array val) : _val(val) {}
    explicit JsonValue(const Json::_obj val) : _val(val) {}

public:
    /**
     * 移动构造函数
     */
    explicit JsonValue(std::string && val) : _val(std::move(val)) {}
    explicit JsonValue(Json::_array && val) : _val(std::move(val)) {}
    explicit JsonValue(Json::_obj && val) : _val(std::move(val)) {}

public:
    /**
     * 析构函数
     */
    ~JsonValue() = default;

public:
    /**
     * 数据类型接口
     */
    JsonType getType() const noexcept;

public:
    size_t size() const;

    /**
     * 访问 array
     */
    Json& operator[] (size_t);
    const Json& operator[] (size_t) const;

    /**
     * 访问 obj
     */
    Json& operator[] (const std::string&);
    const Json& operator[] (const std::string&) const;

public:
    /**
     * 转换函数：
     * JsonValue -> null, bool, double, etc.
     */
    std::nullptr_t toNull() const;
    bool toBool() const;
    double toDouble() const;
    const std::string& toString() const;
    const Json::_array& toArray() const;
    const Json::_obj& toObj() const;

private:
    std::variant<std::nullptr_t, bool, double, 
                 std::string, Json::_array, Json::_obj>
    _val;

    /**
     * Notes:
     * C++17之std::variant
     *  C++提供了对union的支持，但union有一些缺点:
     *      1) 对象不知道它们当前持有的值的类型;
     *      2) 不能有non-trivial的成员;
     *      3) 能从union中派生类;
     * c++标准库提供了一个封闭的区分联合,std:: variable <>:
     *      1) 当前值的类型总是已知的;
     *      2) 可以有任何指定类型的成员;
     *      3) 可以派生类;
     */
};

};                  // ------------------- namespace zzjson


#endif