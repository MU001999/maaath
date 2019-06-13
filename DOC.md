# A组部分

## UTF-8字符串解析

按照UTF-8编码解析字符串，并封装为Utf8String类型的对象

### Utf8String

#### 成员类型

成员类型 | 定义
- | -
data_type | std::u32string
value_type | data_type::value_type
raw_type | std::string
size_type | data_type::size_type
reference | value_type&
const_reference | const value_type&
pointer | value_type*
const_pointer | const value_type*

#### 提供的接口

Utf8String()：构造utf8类型结构体
value_type& front()：访问第一个字符
void clear()：清空内容
void push_back(value_type ch)：在末尾增加一个字符
Utf8String substr(size_type pos, size_type count = npos) const：返回截取字符串
size_type size() const noexcept：返回字符数量
size_type length() const noexcept：返回字符数量
bool empty() const noexcept：判断字符串是否为空
raw_type raw()：返回初始字符串
const char* c_str()：返回字符串的不可修改的C标准字符数组的版本
size_type find(const Utf8String& str, size_type pos = 0) const：在字符串中查找字符

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