# TinyJSON
**简单的 *Json* 解释器，使用了 Modern C++ (C++11 / C++ 17 新标准) 和 [Google C++ 风格指南](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/).** 



***JSON*(JavaScript Object Notation, JS 对象简谱) 是一种轻量级的数据交换格式。**

**TinyJSON 支持的 Json值包括：**

- null(std::nullptr_t)、
- bool、
- number（int / double）、
- string(std::string)、
- array(std::vector) 
- object(std::unordered_map)

**TinyJSON 支持编码格式包括：**

- utf-8



**Highlights**

- .h头文件的防卫式声明 .

```c++
#ifndef PARSE_H__
#define PARSE_H__

/* */

#endif // PARSE_H__
```

- 智能指针的使用. (since C++11)

```c++
// JSON_H__
std::unique_ptr<JsonValue> _jsonVal;

// JSON_CPP__
Json::Json(std::nullptr_t) : _jsonVal(std::make_unique<JsonValue>(nullptr)) {}

// etc.
```

- 右值引用和移动语义. (since C++11)

```c++
// JSON_CPP__
Json::Json(_array&& val)
    : _jsonVal(std::make_unique<JsonValue>(std::move(val))) {}

// etc.
```

- 强制类型转换. (since C++11)

```c++
// JSON_VAL_CPP__
Json& JsonValue::operator[](size_t pos) {
    return const_cast<Json&>(static_cast<const JsonValue&>(*this)[pos]);
}

// etc.
```

- auto和decltype. (since C++11)

```c++
// PARSE_CPP__
auto ch = static_cast<unsigned>(toupper(*++_cur));

// JSON_H__
/**/ decltype(std::declval<M>().begin()->first)>::value /**/
    
// etc.
```

- std::variant(). (since C++17)

```C++
// JSON_VAL_H__
// 当前值的类型总是已知的;
// 可以有任何指定类型的成员;
// 可以派生类;
std::variant<std::nullptr_t, bool, double, 
                 std::string, Json::_array, Json::_obj>
        _val;

/* std::holds_alternative<T>(v) 可查询变体类型 v 是否存放了 T 类型的数据. */
if (std::holds_alternative<std::nullptr_t>(_val)) {
        return JsonType::m_nullptr;
```







## [Google Test](https://github.com/google/googletest/tree/dcc92d0ab6c4ce022162a23566d44f673251eee4)

> Googletest helps you write better C++ tests.
>
> Googletest is a testing framework developed by the Testing Technology team with Google's specific requirements and constraints in mind. No matter whether you work on Linux, Windows, or a Mac, if you write C++ code, googletest can help you. And it supports any kind of tests, not just unit tests.

**Google Test的安装与使用**:

1. 下载googletest.

```CMD
(base) mike@myubuntu:~/下载$ git clone https://github.com/Yuan-Hang/Json
```

2. 编译

```CMD
(base) mike@myubuntu:~/下载$ cd googletest/
```

编辑CMakeLists.txt文件，将option(BUILD_SHARED_LIBS “Build shared libraries (DLLs).” OFF)中的OFF改为ON

生存Makefile文件（请确保正确的安装了Cmake）

```cmd
 (base) mike@myubuntu:~/下载/googletest$ cmake CMakeLists.txt
```

执行make，生成 libgtest_main.so和libgtest.so.

```cmd
 (base) mike@myubuntu:~/下载/googletest$ make
```

拷贝到系统目录 usr/local/lib/.

```cmd
 (base) mike@myubuntu:~/下载/googletest$ sudo cp lib/libgtest*.so /usr/lib
 (base) mike@myubuntu:~/下载/googletest$ sudo cp –a include/gtest /usr/include
 (base) mike@myubuntu:~/下载/googletest$ sudo ldconfig
```

检测是否安装成功，编辑以下测试脚本 test.cpp.

```c++
/*  test.cpp  */
#include<gtest/gtest.h>

int add(int a,int b){
    return a+b;
}

TEST(testCase,test0){
    EXPECT_EQ(add(2,3),5);
}

int main(int argc,char**argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
```

编译并执行.

```cmd
 (base) mike@myubuntu:~/下载/googletest$ g++ *.cpp -o test -l gtest -l pthread
 (base) mike@myubuntu:~/下载/googletest$ ./test 
```

得到以下结果表示安装成功.
![install_gtest](https://github.com/M3stark/TinyJson/blob/main/source/install_gtest.png)







**Ref:**  [Milo Yip](https://github.com/miloyip/json-tutorial)'s JSON project
