#pragma once

#include <iostream>
#include <stack>
#include <string>
#include <list>

class Priority;
class VarIndep;

class expr_tree
{
    struct base_node
    {
        std::string c;
        int prio;
        base_node *lc, *rc;
        base_node() {}
        base_node(std::string c) : c(c), prio(0), lc(nullptr), rc(nullptr) {}
    } * root;

protected:
    void swap_child(base_node *n);
    void destory(base_node *&n);
    void copy_tree(base_node *n, base_node *&nn);
    void set_pri(Priority *pri, base_node *n);
    void set_ord(Priority *pri, base_node *n, VarIndep *a);
    std::string inorder_return(base_node *n);

public:
    void clone_tree(Priority *pri, base_node *n, std::list<std::string> &sub);
    void show(Priority *pri, VarIndep *a, std::list<std::string> &sub);
    void parse_expr(std::list<std::string> str, Priority *pri);
    expr_tree() = default;
    ~expr_tree()
    {
        destory(this->root);
    }
};
