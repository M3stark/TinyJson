#ifndef JSON_H__
#define JSON_H__

#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

namespace zzjson {  // ------------------- namespace zzjson

enum class JsonType {
    m_nullptr,
    m_bool,
    m_number,
    m_string,
    m_array,
    m_obj
};

class JsonValue;

class Json final {
public:
    using _array  = std::vector<Json>;
    using _obj    = std::unordered_map<std::string, Json>;

public:
    /**
     * 构造函数
     */
    Json() : Json(nullptr) {};
    Json(std::nullptr_t);

    Json(bool);
    Json(int val) : Json(1.0 * val) {};
    Json(double);
    Json(const char* cstr) : Json(std::string(cstr)) {};
    Json(const std::string&);
    Json(std::string&&);
    Json(const _array&);
    Json(_array&&);
    Json(const _obj&);
    Json(_obj&&);

    Json(void*) = delete;

public:
    /**
     * 隐式构造函数
     * map / unordered_map, etc.
     */
    template <
        class M,
        typename std::enable_if<
            std::is_constructible<      // 检查对象是否实现了特定参数的构造函数 
                std::string,
                decltype(std::declval<M>().begin()->first)>::value &&
                std::is_constructible<
                    Json, decltype(std::declval<M>().begin()->second)>::value,
            int>::type = 0>
    Json(const M& m) : Json(_obj(m.begin(), m.end())) {}

    /**
     * 隐式构造函数
     * set / unordered_set, etc.
     */
    template < class V,
                typename std::enable_if<
                    std::is_constructible<
                        Json, decltype(* std::declval<V>().begin())>::value,
                            int>::type = 0>
    Json(const V& v) : Json(_array(v.begin(), v.end())) {}

public:
    /**
     * 析构函数
     */
    ~Json();

public:
    /**
     * 拷贝构造 / 拷贝赋值
     */
    Json(const Json&);
    Json& operator= (const Json&) noexcept;     // noexcept -> std: c++11
    Json(Json&&) noexcept;
    Json& operator= (const Json&&) noexcept;

    /**
     * Notes:
     * 拷贝构造函数通常伴随着内存分配操作，因此很可能会抛出异常；
     * 移动构造函数一般是移动内存的所有权，所以一般不会抛出异常；
     * 所以，C++11中新引入了一个noexcept关键字，用来向程序员/编译器来表明这种情况。
     * 对于永远不会抛出异常的函数，可以声明为noexcept的。
     */

public:
    /**
     * parse()      -> 解析接口
     * serialize()  -> 序列化接口
     * errMsg       -> 存储异常消息
     */
    static Json parse(const std::string& content, std::string& errMsg) noexcept;
    std::string serialize() const noexcept;

public:
    /**
     * 类型接口
     */
    JsonType getType() const noexcept;
    bool isNull() const noexcept;
    bool isBool() const noexcept;
    bool isnumber() const noexcept;
    bool isString() const noexcept;
    bool isArray() const noexcept;
    bool isObject() const noexcept;

public:
    /**
     * 类型转换接口
     */
    bool toBool() const;
    double toDouble() const;
    const std::string& toString() const;
    const _array& toArray() const;
    const _obj& toObj() const;

public:
    /**
     * 重载 -> operator[]
     */
    size_t size() const;
    Json& operator[] (size_t);              // operator[] for array
    const Json& operator[] (size_t) const;

    Json& operator[] (const std::string&);  // operator[] for obj
    const Json& operator[] (const std::string&) const;

private:
    void swap(Json&) noexcept;
    std::string SerializeString() const noexcept;
    std::string SerializeArray() const noexcept;
    std::string SerializeObject() const noexcept;

private:
    /**
     * 数据成员
     */
    std::unique_ptr<JsonValue> _jsonVal;
};

inline
std::ostream& operator<<(std::ostream& os, const Json& json) {
    return os << json.serialize();
}

bool operator==(const Json&, const Json&);

inline
bool operator!=(const Json& old_val, const Json& new_val) {
    return !(old_val == new_val);
}

};  // ------------------- namespace zzjson



#endif // JSON_H__