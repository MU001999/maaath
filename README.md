# 《数据结构Ⅱ》课程实践

> 本项目仅包含A组部分实践内容


## [使用文档](./DOC.md)


## 代码规范

Cpp规范参考：[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

Python规范参考：[Google Python Style Guide](https://google.github.io/styleguide/pyguide.html)

更改以及补充参见：[Code Style](./CODESTYLE.md)


## 目录结构
* src --------- 源代码
* test -------- 测试代码
* examples ---- 示例
* resources --- 资源文件


## 需求分析

> 需要提供的功能（不分前后）

+ [x] UTF-8字符串解析
+ [x] 获取信息量
+ [x] 中文分词
+ [x] 倒排索引


## 接口设计

### 结果打分/排序

#### 提供

输入若干关键词返回打分后排序的合并后的文件列表

B组提供合并文件路径列表的接口，提供文件路径列表的列表

根据临时map恢复只保留交集文件列表的map

## 具体实现

### UTF-8字符串解析

### 获取信息量

### 中文分词

根据信息量优化分词质量

### 倒排索引

建立词语与文件列表的关联

### 结果打分/排序

1. 公式优先（包含公式的文章也提高优先级）
1. 主概念优先（在章节名中出现）
1. 定义、性质（与主概念章节距离作为打分标准之一）
1. 文章总信息量（包括关键词信息量以及频度）
1. 五类查询互不干扰

#### 解耦

* 在add_files接口中，根据章节标题排序，保证插入到map中文件列表是有序的
* 通过get_files_list接口获取的文件列表的索引判断章节位置即可
* 计算文章总信息量的接口，总信息量 = 累加（信息量 * 频度），接受map<kw, fileinfolist>
* 计算文章总分数的接口
* 根据上述接口排序

### 前后端分离

> （暂不确定是否为A组部分内容）
