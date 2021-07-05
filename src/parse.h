#ifndef PARSE_H__
#define PARSE_H__

#pragma once

#include "json.h"
#include "json_except.h"

namespace zzjson {  // ------------------- namespace zzjson

constexpr bool ISDIGIT1TO9(char ch) {
    return ch >= '1' && ch <= '9';
}

constexpr bool ISDIGIT(char ch) {
    return ch >= '0' && ch <= '9';
}

class Parser {
public:
    /**
     * 构造函数
     */
    explicit Parser(const char* cstr) noexcept : _start(cstr), _cur(cstr) {}

    explicit Parser(const std::string& content) noexcept
                         : _start(content.c_str()), _cur(content.c_str()) {}

public:
    /**
     * 令其不可拷贝
     */
    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

private:
    /**
     * 封装处理的辅助函数函数
     */
    void ParserSpace() noexcept;
    unsigned Parser4Hex();
    std::string EncoddeUTF8(unsigned u) noexcept;
    std::string ParserRowString();

    /**
     * throw 错误的位置
     */
    void error(const std::string& msg) const;

private:
    /**
     * 封装处理函数
     */
    Json ParserValue();
    Json ParserLiteral(const std::string& literal);
    Json ParserNumber();
    Json ParserString();
    Json ParserArray();
    Json ParserObj();

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