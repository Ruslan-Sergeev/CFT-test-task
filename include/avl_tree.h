#ifndef AVL_TREE_H
#define AVL_TREE_H

/* Структура для представления узла дерева */
typedef struct _node {
    unsigned long key;
    void *value;
    int height;
    struct _node *left;
    struct _node *right;
} node;

/* Вставка нового элемента */
node* insert(node *p, unsigned long k, void* value, void *(*merge)(void *, void *), unsigned long *num_el);

/* Проверяет, находится ли элемент k в дереве */
node* find_node(node *n, unsigned long k);

/* Обход каждого узла дерева и сохранение его значения в массив */
void traverse_and_fill(node *root, void *array, unsigned long *index_ptr, void (*copy_fn)(void *, void *, unsigned long));

#endif /* AVL_TREE_H */
