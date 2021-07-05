#include "parse.h"
#include <cassert>    // assert
#include <cmath>      // Huge_Val
#include <cstdlib>    // strtod
#include <cstring>    // strncmp
#include <stdexcept>  // runtime_error

namespace zzjson {  // ------------------- namespace zzjson

/**
 * 跳过所有的空格
 */
void Parser::ParserSpace() noexcept {
    while (*_cur == ' ' || *_cur == '\t' || *_cur == '\r' ||
           *_cur == '\n') {
        ++_cur;
    }
    _start = _cur;
}

/**
 * 读取4 位 16 进制数字
 */
unsigned Parser::Parser4Hex() {
    unsigned u = 0;
    for (int i = 0; i != 4; ++i) {
        auto ch = static_cast<unsigned>(toupper(*++_cur));
        u <<= 4;
        if (ch >= '0' && ch <= '9') {
            u |= (ch - '0');
        } else if (ch >= 'A' && ch <= 'F') {
            u |= ch - 'A' + 10;
        } else {
            error("INVALID UNICODE HEX");
        }
    }
    return u;
}

/**
 * 详见： UTF-8 编码表
 */
std::string Parser::EncoddeUTF8(unsigned u) noexcept {
    std::string utf8;
    if (u <= 0x7F) {  // 0111,1111
        utf8.push_back(static_cast<char>(u & 0xff));
    } else if (u <= 0x7FF) {
        utf8.push_back(static_cast<char>(0xc0 | ((u >> 6) & 0xff)));
        utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
    } else if (u <= 0xFFFF) {
        utf8.push_back(static_cast<char>(0xe0 | ((u >> 12) & 0xff)));
        utf8.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3f)));
        utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
    } else {
        assert(u <= 0x10FFFF);
        utf8.push_back(static_cast<char>(0xf0 | ((u >> 18) & 0xff)));
        utf8.push_back(static_cast<char>(0x80 | ((u >> 12) & 0x3f)));
        utf8.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3f)));
        utf8.push_back(static_cast<char>(0x80 | (u & 0x3f)));
    }
    return utf8;
}

/**
 * 转义序列的解析
 */
std::string Parser::ParserRowString() {
    std::string str;
    while (true) {
        switch (*++_cur) {
            case '\"':
                _start = ++_cur;
                return str;
            case '\0':
                error("MISS QUOTATION MARK");
            default:
                if (static_cast<unsigned char>(*_cur) < 0x20)
                    error("INVALID STRING CHAR");
                str.push_back(*_cur);
                break;
            case '\\':
                switch (*++_cur) {
                    case '\"':
                        str.push_back('\"');
                        break;
                    case '\\':
                        str.push_back('\\');
                        break;
                    case '/':
                        str.push_back('/');
                        break;
                    case 'b':
                        str.push_back('\b');
                        break;
                    case 'f':
                        str.push_back('\f');
                        break;
                    case 'n':
                        str.push_back('\n');
                        break;
                    case 't':
                        str.push_back('\t');
                        break;
                    case 'r':
                        str.push_back('\r');
                        break;
                    case 'u': {
                        /**
                         * 代理对的处理
                         * 遇到高代理项，就需要把低代理项 \uxxxx 也解析进来.
                         */
                        unsigned u1 = Parser4Hex();
                        if (u1 >= 0xd800 && u1 <= 0xdbff) {  // high surrogate
                            if (*++_cur != '\\')
                                error("INVALID UNICODE SURROGATE");
                            if (*++_cur != 'u')
                                error("INVALID UNICODE SURROGATE");
                            unsigned u2 = Parser4Hex();  // low surrogate
                            if (u2 < 0xdc00 || u2 > 0xdfff)
                                error("INVALID UNICODE SURROGATE");
                            u1 = (((u1 - 0xd800) << 10) | (u2 - 0xdc00)) +
                                 0x10000;
                        }
                        str += EncoddeUTF8(u1);
                    } break;
                    default:
                        error("INVALID STRING ESCAPE");
                }
                break;
        }
    }
}

/**
 * throw 错误的位置
 */
void Parser::error(const std::string& msg) const {
    throw JsonExcept(msg + ": " + _start);
}

Json Parser::ParserValue() {
    switch (*_cur) {
        case 'n':
            return ParserLiteral("null");
        case 't':
            return ParserLiteral("true");
        case 'f':
            return ParserLiteral("false");
        case '\"':
            return ParserString();
        case '[':
            return ParserArray();
        case '{':
            return ParserObj();
        case '\0':
            error("EXPECT VALUE");
        default:
            return ParserNumber();
    }
}

/**
 * 解析 true / false / null
 */
Json Parser::ParserLiteral(const std::string& literal) {
    // try to parse null && true && false
    if (strncmp(_cur, literal.c_str(), literal.size()) != 0)
        error("INVALID VALUE");
    _cur += literal.size();
    _start = _cur;
    switch (literal[0]) {
        case 't':
            return Json(true);
        case 'f':
            return Json(false);
        default:
            return Json(nullptr);
    }
}

/**
 * 解析数字
 * 详见：
 * https://github.com/miloyip/json-tutorial/blob/master/tutorial02/images/number.png
 */
Json Parser::ParserNumber() {
    // 负号直接跳过.
    if (*_cur == '-') {
        ++_cur;
    }

    // 整数部分有两种合法情况：
    // 1. 出现 0，直接跳过.
    // 2. 1-9 再加上任意数量的 digit.
    if (*_cur == '0')
        ++_cur;
    else {
        // 第一个字符必须为 1-9，如果否定的就是不合法的.
        if (!ISDIGIT1TO9(*_cur)) error("INVALID VALUE");
        while (ISDIGIT(*++_cur))
            ;  // pass all number character
    }
    
    // 有小数点则跳过该小数点
    // 然后检查它至少应有一个 digit，不是 digit 就返回报错.
    if (*_cur == '.') {
        if (!ISDIGIT(*++_cur))  // there must be a number character after '.'
            error("INVALID VALUE");
        while (ISDIGIT(*++_cur))
            ;
    }

    // 如果出现 E，就表示有指数部分.
    // 跳过这个 E 之后，可以有一个正或负号，有的话就跳过.
    // 然后和小数的逻辑是一样的.
    if (toupper(*_cur) == 'E') {
        ++_cur;
        if (*_cur == '-' || *_cur == '+') ++_cur;

        if (!ISDIGIT(*_cur)) {
            error("INVALID VALUE");
        }
        while (ISDIGIT(*++_cur))
            ;
    }
    
    // 十进制的数字转换成二进制的 double
    double val = strtod(_start, nullptr);

    // 数字过大
    if (fabs(val) == HUGE_VAL) {
        error("NUMBER TOO BIG");
    }
    _start = _cur;
    return Json(val);
}

Json Parser::ParserString() { return Json(ParserRowString()); }

/**
 * 解析数组
 */
Json Parser::ParserArray() {
    Json::_array arr;
    ++_cur;  // 跳过 '['
    ParserSpace();
    if (*_cur == ']') {
        _start = ++_cur;
        return Json(arr);
    }
    while (true) {
        ParserSpace();
        arr.push_back(ParserValue());  // recursive
        ParserSpace();
        if (*_cur == ',')
            ++_cur;
        else if (*_cur == ']') {
            _start = ++_cur;
            return Json(arr);
        } else
            error("MISS COMMA OR SQUARE BRACKET");
    }
}

Json Parser::ParserObj() {
    Json::_obj obj;
    ++_cur;
    ParserSpace();
    if (*_cur == '}') {
        _start = ++_cur;
        return Json(obj);
    }
    while (true) {
        ParserSpace();
        if (*_cur != '"') error("MISS KEY");
        std::string key = ParserRowString();
        ParserSpace();
        if (*_cur++ != ':') error("MISS COLON");
        ParserSpace();
        Json val = ParserValue();
        obj.insert({key, val});
        ParserSpace();
        if (*_cur == ',')
            ++_cur;
        else if (*_cur == '}') {
            _start = ++_cur;
            return Json(obj);
        } else
            error("MISS COMMA OR CURLY BRACKET");
    }
}

/**
 * 公共调用的接口
 */
Json Parser::parse() {
    ParserSpace();
    Json json = ParserValue();
    ParserSpace();
    if (*_cur)
        // some character still exists after the end whitespace
        error("ROOT NOT SINGULAR");
    return json;
}

};              // ------------------- namespace zzjson