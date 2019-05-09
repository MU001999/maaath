#include "tnode.hpp"
#include "varindep.hpp"
#include "priority.hpp"

using namespace std;

string a;

void expr_tree::parse_expr(list<string> str, Priority *pri)
{
    stack<base_node *> stack_node;
    for (auto c : str)
    {
        base_node *node = new base_node(c);
        if (!pri->is_optr(c))
        {
            stack_node.push(node);
        }
        else if (pri->is_optr(c))
        {
            if (c == "#" || c == "\\vert")
            {
                if (!stack_node.empty())
                {
                    node->rc = stack_node.top();
                    stack_node.pop();
                }
            }
            else
            {
                if (!stack_node.empty())
                {
                    node->rc = stack_node.top();
                    stack_node.pop();
                }
                if (!stack_node.empty())
                {
                    node->lc = stack_node.top();
                    stack_node.pop();
                }
            }
            stack_node.push(node);
        }
        else return;
    }
    if (!stack_node.empty())
        root = stack_node.top();
    while (!stack_node.empty())
        stack_node.pop();
}

string expr_tree::inorder_return(base_node *n)
{
    if (n)
    {
        if (n->c >= "0" && n->c <= "9")
        {
            a += n->c;
        }
        else
        {
            a += '(';
            inorder_return(n->lc);
            a = a + ' ' + n->c + ' ';
            inorder_return(n->rc);
            a += ')';
        }
    }
    return a;
}

void expr_tree::clone_tree(Priority *pri, base_node *n, list<string> &sub)
{
    if (n)
    {
        if (pri->is_optr(n->c))
        {
            sub.push_back(inorder_return(n));
            a = "";
        }
        clone_tree(pri, n->lc, sub);
        clone_tree(pri, n->rc, sub);
    }
}
void expr_tree::set_pri(Priority *pri, base_node *n)
{
    if (n)
    {
        if (pri->is_optr(n->c))
        {
            n->prio = pri->get_pri(n->c);
        }
        set_pri(pri, n->lc);
        set_pri(pri, n->rc);
    }
}
void expr_tree::copy_tree(base_node *n, base_node *&nn)
{
    if (n == NULL)
        return;
    else
    {
        nn = (base_node *)malloc(sizeof(root));
        nn->c = n->c;
        copy_tree(n->lc, nn->lc);
        copy_tree(n->rc, nn->rc);
    }
}

void expr_tree::show(Priority *pri, VarIndep *a, list<string> &sub)
{
    set_pri(pri, this->root);
    set_ord(pri, this->root, a);
    clone_tree(pri, this->root, sub);
}

void expr_tree::swap_child(base_node *n)
{

    if (n == NULL)
        return;
    if (n->c == "+" || n->c == "*")
    {
        swap(n->lc, n->rc);
        swap_child(n->lc);
        swap_child(n->rc);
    }
}

void expr_tree::set_ord(Priority *pri, base_node *n, VarIndep *a)
{

    if (n && (n->lc || n->rc))
    {
        if (n->lc && n->rc)
        {
            if (pri->is_optr(n->lc->c) && pri->is_optr(n->rc->c))
            {
                if (n->lc->prio > n->rc->prio)
                {
                    swap(n->lc, n->rc);
                }
            }
            else if (!pri->is_optr(n->lc->c) && pri->is_optr(n->rc->c))
            {
                n->lc->c = a->GetSymbol(n->lc->c);
                a->GetSymbol(n->lc->c);
                swap(n->lc, n->rc);
            }
            else if (pri->is_optr(n->lc->c) && !pri->is_optr(n->rc->c))
            {
                n->rc->c = a->GetSymbol(n->rc->c);
            }
            else if (!pri->is_optr(n->lc->c) && !pri->is_optr(n->rc->c))
            {
                string lr, rr;
                lr = a->GetSymbol(n->lc->c);
                rr = a->GetSymbol(n->rc->c);
                n->lc->c = lr;
                n->rc->c = rr;
                if (lr > rr)
                    swap(n->lc, n->rc);
            }
        }
        else
        {
            if (n->rc) n->rc->c = a->GetSymbol(n->rc->c);
            else n->lc->c = a->GetSymbol(n->lc->c);
        }
        set_ord(pri, n->lc, a);
        set_ord(pri, n->rc, a);
    }
}

void expr_tree::destory(base_node *&root)
{
    if (root != nullptr)
    {
        destory(root->lc);
        destory(root->rc);
        delete root;
    }
}