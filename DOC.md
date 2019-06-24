# A组部分代码文档

## UTF-8字符串解析

按照UTF-8编码解析字符串，并封装为Utf8String类型的对象

### Utf8String

#### 成员类型

成员类型         | 定义
--------------- | ---------------
data_type       | std::u32string
value_type      | data_type::value_type
raw_type        | std::string
size_type       | data_type::size_type
reference       | value_type&
const_reference | const value_type&
pointer         | value_type*
const_pointer   | const value_type*

#### 成员函数

函数名 | 功能
------ | ----
（构造函数） | 构造Utf8String
（析构函数） | 销毁字符串
front      | 访问首字符
back       | 访问最后的字符
clear      | 清除内容
push_back  | 后附字符到结尾
substr     | 返回子串
size       | 返回字符数
length     | 返回字符数
empty      | 检查字符串是否为空
raw        | 返回std::string类型的版本
c_str      | 返回不可修改的C字符数组版本
find       | 于字符串中查找字符(串)
operator=  | 为字符串赋值
operator[] | 访问指定字符
operator+= | 后附字符(串)到结尾
operator+  | 连接两个字符串或者一个字符串和一个字符
operator== | 以字典序比较两个字符串是否相等
operator!= | 以字典序比较两个字符串是否不相等
operator<  | 以字典序比较两个字符串
operator<< | 执行字符串的流输入
operator>> | 执行字符串的流输出

## 中文分词

依赖Utf8String类型,对中文基于信息量实现分词操作,并封装成一个可以直接获取分词结果的接口

### InfoQuantity

封装词语信息量的查询操作

#### 接口

* `double get_infoquantity(const Utf8String &word);`

    接受一个词,并返回该词的信息量

* `bool count(const Utf8String &word);`

    接受一个词,并返回该词在词频文件是否存在

### Segmentation

#### 接口

* `std::vector<Utf8String> segment(const Utf8String &sentence);`

    接口接受一个UTF-8编码的Utf8String类型的字符串,并返回分词之后的词语集合.该接口保证返回词语集合的顺序与其在原句中的次序相同.

#### 算法

1. 按词频词典找出字符串中存在的歧义区间

1. 歧义区间中选出若干种词的组合，每种组合要消除歧义

1. 计算出选出的组合中信息量最小的，这个组合可以认为是最接近语义的组合

## 倒排索引

封装倒排索引,实现如下功能:

1. 倒排索引的建立
1. 根据关键词进行检索
1. 对象到文件的序列化
1. 文件到对象的反序列化

### FileInfo

描述单个关键词与单个文件之间联系的结构体

包含以下信息:

1. 关键词所在文章的文件路径
1. 关键词在该文章中出现的次数和频率
1. 关键词是否出现在文章的标题中

### KeywordInfo

描述单个关键词在单个文件中的信息的结构体

包含以下信息:

1. 关键词在该文章中出现的次数和频率
1. 关键词是否出现在文章的标题中

### FileInfoWithAllKeywords

描述单个文章以及其中出现的所有关键词的联系的结构体

包含以下信息:

1. 该文章的文件路径
1. 类型为`std::map<Utf8String, KeywordInfo>`的关键词与关键词信息的对象

### InvertedIndex

#### 成员类型

成员类型         | 定义
--------------- | ---------------
key_type        | Utf8String
value_type      | std::vector\<FileInfo\>
data_type       | std::map\<key_type, value_type\>

#### 成员函数

函数名 | 功能
------ | ----
（构造函数）    | 建立倒排索引
ready         | 返回该倒排索引是否已建立成功
serialize     | 由对象序列化到文件
unserialize   | 由文件反序列化到对象
get_fileinfos | 检索给定关键词,返回检索结果,包含关键词在文章中的信息
get_filepaths | 检索给定关键词,返回检索结果,只包含结果的文件路径
add_files     | 根据给定文件夹路径,添加新的索引
add_file      | 根据给定文件路径,添加新的索引

## 检索服务器

基于Socket的UNIX域实现进程间通信, 并通过多线程优化多进程访问时的效率

### Request

规定与其它进程通信的细节

包含以下信息:

1. 检索的类型
1. 检索的关键词列表

### SearchServer

封装服务器的行为,只提供监听和启动操作,隐藏实现细节

#### 成员函数

函数名 | 功能
------ | ----
（构造函数） | 构造SearchServer, 初始化套接字
listen     | 开始监听外部请求
run        | 进入主循环,处理外部检索请求
