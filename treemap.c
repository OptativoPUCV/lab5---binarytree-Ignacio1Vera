#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  Pair *pair;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
};

struct TreeMap {
  TreeNode *root;
  TreeNode *current;
  int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2) {
  if (tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0)
    return 1;
  else
    return 0;
}

TreeNode *createTreeNode(void *key, void *value) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL)
    return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

TreeMap *createTreeMap(int (*lt)(void *key1, void *key2)) {
  TreeMap *map = (TreeMap *)malloc(sizeof(TreeMap));
  if (map == NULL)
    return NULL;

  map->root = NULL;
  map->current = NULL;
  map->lower_than = lt;

  return map;
}

void insertTreeMap(TreeMap* tree, void* key, void* value) {
    if (searchTreeMap(tree, key) != NULL) {
        return;
    }

    TreeNode* newNode = createTreeNode(key, value);
    if (newNode == NULL) {
        return;
    }

    if (tree->root == NULL) 
    {
        tree->root = newNode;
        tree->current = newNode;
    } else {
        TreeNode* current = tree->root;

        while (1) 
        {
            int comparison = tree->lower_than(key, current->pair->key);

            if (comparison < 0) {
                if (current->left == NULL) 
                {
                    newNode->parent = current;
                    current->left = newNode;
                    tree->current = newNode;
                    break;
                } 
                else 
                {
                    current = current->left;
                }
            } else if (comparison > 0) 
            {
                if (current->right == NULL) 
                {
                    newNode->parent = current;
                    current->right = newNode;
                    tree->current = newNode;
                    break;
                } 
                else 
                    current = current->right;
            } 
            else 
                break;
        }
    }
}


TreeNode* minimum(TreeNode* x) {
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap* tree, TreeNode* node) {
    if (node == NULL || tree == NULL) {
        return;
    }

    if (node->left == NULL && node->right == NULL) {
        // Nodo sin hijos
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        } else {
            // Nodo raíz sin hijos
            tree->root = NULL;
        }
        free(node->pair->key);
        free(node->pair->value);
        free(node->pair);
        free(node);
    } else if (node->left != NULL && node->right != NULL) {
        // Nodo con dos hijos
        TreeNode* successor = minimum(node->right);
        node->pair = successor->pair;
        removeNode(tree, successor);
    } else {
        // Nodo con un hijo
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
            child->parent = node->parent;
        } else {
            // Nodo raíz con un hijo
            tree->root = child;
            child->parent = NULL;
        }
        free(node->pair->key);
        free(node->pair->value);
        free(node->pair);
        free(node);
    }
}


void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair* searchTreeMap(TreeMap* tree, void* key) {
    TreeNode* current = tree->root;
    TreeNode* found = NULL;

    while (current != NULL) {
        int comparison = tree->lower_than(key, current->pair->key);

        if (comparison == 0) {
            found = current;
            break;
        } else if (comparison < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (found != NULL) {
        tree->current = found;
        return found->pair;
    } else {
        tree->current = NULL;
        return NULL;
    }
}


Pair* upperBound(TreeMap* tree, void* key) {
    TreeNode* current = tree->root;
    TreeNode* ub_node = NULL;

    while (current != NULL) {
        int comparison = tree->lower_than(key, current->pair->key);

        if (comparison == 0) {
            tree->current = current;
            return current->pair;
        } else if (comparison < 0) {
            ub_node = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (ub_node != NULL) {
        tree->current = ub_node;
        return ub_node->pair;
    }

    return NULL;
}


Pair* firstTreeMap(TreeMap* tree) {
    if (tree == NULL) {
        return NULL;
    }
    TreeNode* minNode = minimum(tree->root);
    if (minNode != NULL) {
        tree->current = minNode;
        return minNode->pair;
    }
    return NULL;
}

Pair* nextTreeMap(TreeMap* tree) {
    if (tree == NULL || tree->current == NULL) {
        return NULL;
    }

    TreeNode* current = tree->current;
    if (current->right != NULL) {
        tree->current = minimum(current->right);
        return tree->current->pair;
    } else {
        while (current->parent != NULL && current == current->parent->right) {
            current = current->parent;
        }
        tree->current = current->parent;
        if (current->parent != NULL) {
            return current->parent->pair;
        }
    }

    return NULL;
}

