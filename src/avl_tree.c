/* АВЛ-дерево (с упрощённой реализацией и адаптицией под нужды работы с dump) */
/* Поиск, Вставка, Удаление узла(не реализовано) выполняются за O(log N)*/
#include <stdio.h>
#include <stdlib.h>
#include "../include/avl_tree.h"

/* Создать новый узел */
static node* create_node(unsigned long k, void *value, unsigned long *num_el)
{
    node *new_node;
    new_node = (node *)malloc(sizeof(node));
    if (new_node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }
    new_node->key = k;
    new_node->height = 1;
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;

    *num_el = *num_el + 1;
    return new_node;
}

/* Получение высоты узла */
static int get_height(node *n)
{
    if (n != NULL)
    {
        return n->height;
    }
    else
    {
        return 0;
    }
}

/* Коэффициент сбалансированности */
static int balance_factor(node *n)
{
    return (get_height(n->right) - get_height(n->left));
}

/* Исправить значение высоты */
static void fix_height(node *n)
{
    int right_height = get_height(n->right);
    int left_heigth = get_height(n->left);

    if (left_heigth > right_height)
    {
        n->height = left_heigth + 1;
    }
    else
    {
        n->height = right_height + 1;
    }
}

/* Правый поворот */
static node* right_rotation(node *x)
{
    node *y = x->left;
    x->left = y->right;
    y->right = x;
    fix_height(x);
    fix_height(y);

    return y;
}

/* Левый поворот */
static node* left_rotation(node *y)
{
    node *x = y->right;
    y->right = x->left;
    x->left = y;
    fix_height(y);
    fix_height(x);

    return x;
}

/* Балансировка узла n */
static node* balance(node *n)
{
    fix_height(n);
    if (balance_factor(n) == 2)
    {
        if (balance_factor(n->right) < 0)
        {
            n->right = right_rotation(n->right);
        }
        return left_rotation(n);
    }
    if (balance_factor(n) == -2)
    {
        if(balance_factor(n->left) > 0)
        {
            n->left = left_rotation(n->left);
        }
        return right_rotation(n);
    }
    return n; // Если ничего балансировать не надо
}

/* Вставка нового элемента */
node* insert(node *p, unsigned long k, void* value, void *(*merge)(void *, void *), unsigned long *num_el)
{
    if (p == NULL)
    {
        return create_node(k, value, num_el);
    }

    if (k < p->key)
    {
        p->left = insert(p->left, k, value, merge, num_el);
    }
    else if (k > p->key)
    {
        p->right = insert(p->right, k, value, merge, num_el);
    }
    else
    {
        if (!merge)
        {
            fprintf(stderr, "Merge function is NULL!\n");
            exit(EXIT_FAILURE);
        }
        p->value = merge(p->value, value);
        return p;
    }

    return balance(p);
}

/* Функция поиска узла по ключу k в дереве */
node* find_node(node *root, unsigned long k)
{
    if (root == NULL || root->key == k)
    {
        return root;
    }
    if (k < root->key)
    {
        return find_node(root->left, k);
    }
    else
    {
        return find_node(root->right, k);
    }
}

/* Обход каждого узла дерева и сохранение его значения в массив */
void traverse_and_fill(node *root, void *array, unsigned long *index_ptr, void (*copy_fn)(void *, void *, unsigned long))
{
    if (root == NULL)
        return;

    traverse_and_fill(root->left, array, index_ptr, copy_fn);
    copy_fn(array, root->value, *index_ptr);
    (*index_ptr)++;
    traverse_and_fill(root->right, array, index_ptr, copy_fn);

    return;
}




