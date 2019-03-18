# 代码风格更改/重点以及补充

## 命名

### 一般命名规则

名称应该是描述性的，避免缩写。

在合理范围内尽可能给出描述性的名称。不要担心水平长度，也不要通过删除单词中的字母来缩写，当缩写在(维基)百科中列出时，则缩写被允许。

```cpp
// √
int price_count_reader; // 没有缩写
int num_errors; // "num"是一种普遍的惯例
int num_dns_connections; // 大多数人都知道"DNS"的含义
int lstm_size; // “LSTM”是常见的机器学习缩写
```
```cpp
// ×
int n; // 无意义
int nerr; // 不明确的缩写
int n_comp_conns; // 不明确的缩写
int wgc_connections; // 只有你的小组才知道这代表什么
int pc_reader; // 很多东西都可以缩写为"pc"
int cstmr_id; // 删除内部字母
FooBarRequestInfo fbri; // 甚至不是一个单词
```

需要注意的是，某些普遍已知的缩写是可以的，比如i作为迭代变量，T作为模板参数。

模板参数应遵循其类别的命名样式：类型模板参数应遵循类型名称的规则，非类型模板参数应遵循变量名称的规则。

### 文件名称

文件名应全部为小写，并且可以包含下划线（_）。

可接受的文件名示例：
* my_useful_class.cpp
* myusefullclass.cpp
* myusefullclass_test.cpp

C++文件应该以.cpp为后缀名，头文件应该以.hpp为后缀名。

通常应使文件名非常具体。比如应使用http_server_logs.hpp而不是logs.hpp。一个很常见的情况是一对文件,foo_bar.hpp和foo_bar.cpp，定义了一个名为FooBar的类。

### 类型名称

类型名称应以大写字母开头，每个新单词都应以一个新的大写字母开头，没有下划线：MyExcitingClass，MyExcitingEnum。

所有类型（类，结构，类型别名，枚举和类型模板参数）具有相同的命名规范。例如：

```cpp
// √
// class and struct
class UrlTable;
class UrlTableTester;
struct UrlTableProperties;

// typedefs
typedef hash_map<UrlTableProperties *, string> PropertiesMap;

// using aliases
using PropertiesMap = hash_map<UrlTableProperties *, string>;

// enums
enum UrlTableErrors;
```

### 变量名称

变量（包括函数参数）和数据成员的名称都应是小写的，单词之间有下划线。例如：a_local_variable，a_struct_data_member，a_class_data_member。

例如：
```cpp
// √
string table_name; // OK - uses underscore
string tablename; // OK - all lowercase
```
```cpp
string tableName; // Bad - mixed case
```

### 常量名称

常量应声明为constexpr，以'k'开头命名，后每个单词以一个大写字母开头，在不能区分的时候可使用下划线。例如：
```cpp
constexpr int kDaysInAWeek = 7;
constexpr int kAndroid8_0_0 = 24; // Android 8.0.0
```

静态和全局变量，也应以此方式命名。

### 函数名称

像变量一样命名。

## 头文件

### include的顺序

在整个项目中，除去项目的汇总头文件外（如本项目中的src/dse.hpp）所有的头文件应避免include其它头文件，避免产生不必要的依赖错误。所有的实现文件(\*.cpp)，除在汇总头文件中已包含的include外，若需要引用其余头文件，则应在其文件中单独include，而不应添加在汇总头文件中。

includes的顺序如下
1. 项目头文件
1. 空行
1. C system files
1. 空行
1. C++ system files
1. 空行
1. Other libraries' .h(pp) files
1. 空行
1. Your project's .h(pp) files

### 避免在头文件中using namespace xxx

避免在头文件中`using namespace xxx`，使得C++文件中的作用域被污染。

## 杂项

花括号应换行。
