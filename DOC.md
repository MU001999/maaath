# A组部分

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

## 倒排索引

## 检索服务器

segmentation.cpp:

提供的接口：
    std::vector<Utf8String> Segmentation::segment(const Utf8String & sentence)

功能:
    传入一个UTF-8编码的字符串，将这个字符串按照中文语义切割，返回一个中文词向量

算法:

    1.按词频词典找出中文字串中存在的歧义区间

    2.歧义区间中选出若干种词的组合，每种组合要消除歧义

    3.计算出选出的组合中信息量最小的，这个组合可以认为是最接近语义的组合

    如 记者读报纸
    分割的结果是 记者 读 报纸

searchserver.cpp:

提供的类:

    Server

提供的接口:

    Server::listen()
    Server::run()
功能:

    侦听来自前端服务器的检索请求，将检索结果返回给前端服务器
    每个请求用一个线程处理

infoquantity.cpp：

功能：
    读取词频文件，将文件内容存入词频列表
    提取某个词的词频

提供的接口：
    double get_infoquantity(const Utf8String & word)：根据参数输入的词提取该词词频
    bool count(const Utf8String & word) const：判断参数输入的词在词频列表中是否只存在一次

invertedindex.cpp：

功能：
    将字符串进行分词，建立倒排索引

提供的类：
    InvertedIndex

提供的接口：
    InvertedIndex(const std::string& filepath)：接收序列化文件，构建倒排索引
    bool ready() const：判断倒排索引是否建立完成
    bool serialize()：判断输出到序列化文件是否完成
    bool unserialize()：判断由文件反序列化输出到结构体是否完成
    std::vector<FileInfoWithAllKeywords> get_fileinfos(const std::vector<key_type>& keywords, int pagenum = 1, int perpage = 10)：由输入的关键词返回文件信息
    std::vector<std::string> get_filepaths(const std::string &keywords)
    std::vector<std::string> get_filepaths(const std::vector<key_type>& keywords) ：由输入的关键词返回文件路径
    void add_files(const std::string& folderpath)：根据输入的文件路径添加文件
    void add_file(const key_type& sentence, const std::string& filepath)：将一个句子中所有的词添加到倒排索引

commucation.cpp：

功能：
    将web页面的请求保存至结构体以用于与后端沟通

提供的类：
    Request

提供的接口：
    Request(const std::string &raw)：将web页面的请求存入结构体
    Request::CommType Request::type() const：返回类型
    std::string Request::keywords() const：返回关键词
