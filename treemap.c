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

void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
  if(searchTreeMap(tree, key) != NULL)
    return;

  TreeNode* parent = NULL;
  TreeNode* current = tree->root;

  // Encuentra el lugar donde se debe insertar el nuevo nodo
  while(current != NULL)
  {
    parent = current;
    if(tree->lower_than(key, current->pair->key))
      current = current->left;
    else
      current = current->right;
  }

  // Crea el nuevo nodo y establece su parent
  TreeNode* nuevo = createTreeNode(key, value);
  nuevo->parent = parent;

  // Establece el nuevo nodo en el árbol
  if(parent == NULL)
    tree->root = nuevo;
  else if (tree->lower_than(key, parent->pair->key))
    parent->left = nuevo;
  else
    parent->right = nuevo;

  tree->current = nuevo;
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
        //sin hijos
        if (node->parent == NULL) {
            tree->root = NULL;
        } else {
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        }
    } else if (node->left != NULL && node->right != NULL) {
        //dos hijos
        TreeNode* successor = minimum(node->right);
        node->pair = successor->pair;
        removeNode(tree, successor);
    } else {
        //un hijo
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        if (node->parent == NULL) {
            tree->root = child;
            child->parent = NULL;
        } else {
            if (node->parent->left == node) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
            child->parent = node->parent;
        }
    }
    // Liberar memoria
    free(node->pair->key);
    free(node->pair->value);
    free(node->pair);
    free(node);
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
    TreeNode* aux = NULL;  // Nodo auxiliar para el upperBound

    while(current != NULL)
    {
      if(tree->lower_than(current->pair->key, key))
          current = current->right;
      else if(is_equal(tree, current->pair->key, key))
          return current->pair;
      else
      {
          aux = current;  // Actualiza el nodo auxiliar al moverse hacia la izquierda
          current = current->left;
      }      
    }

    if(aux != NULL)
    {
        tree->current = aux;
        return aux->pair;
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

