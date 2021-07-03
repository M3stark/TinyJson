#ifndef PARSE_H__
#define PARSE_H__

#pragma once

#include "json.h"
#include "json_except.h"

namespace zzjson {  // ------------------- namespace zzjson

constexpr bool is1to9(char ch) {
    return ch >= '1' && ch <= '9';
}

constexpr bool is0to9(char ch) {
    return ch >= '0' && ch <= '9';
}

class Paser {
public:
    /**
     * 构造函数
     */
    explicit Paser(const char* cstr) noexcept : _start(cstr), _cur(cstr) {}
    explicit Paser(const std::string& content) noexcept : _start(content.c_str()),
                                                          _cur(content.c_str()) {}

public:
    /**
     * 令其不可拷贝
     */
    Paser(const Paser&) = delete;
    Paser& operator=(const Paser&) = delete;

private:
    /**
     * 封装处理的辅助函数函数
     */
    void PaserSpace() noexcept;
    unsigned Paser4Hex();
    std::string EncoddeUTF8(unsigned u) noexcept;
    std::string PaserRowString();

    void error(const std::string& msg) const;

private:
    /**
     * 封装处理函数
     */
    Json PaserValue();
    Json PaserLiteral(const std::string& literal);
    Json PaserNumber();
    Json PaserString();
    Json PaserArray();
    Json PaserObj();

public:
    /**
     * 公共调用的接口
     */
    Json parse();

private:
    /**
     * 字符串中开始和当前位置的指针
     */
    const char* _start;
    const char* _cur;
};


};              // ------------------- namespace zzjson

#endif