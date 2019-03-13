# 代码风格更改以及补充

## 头文件

### 命名以及include的顺序

头文件应以.hpp为后缀

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
