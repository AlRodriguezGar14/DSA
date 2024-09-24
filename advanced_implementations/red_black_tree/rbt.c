#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct RBNode RBNode;
struct RBNode
{
  int value;
  int color;
  RBNode *parent;
  RBNode *left;
  RBNode *right;
};

typedef struct
{
  RBNode *root;
} RBTree;

void
print_tree (RBNode *node)
{
  if (node == NULL)
    return;
  print_tree (node->left);
  printf (" %d ", node->value);
  print_tree (node->right);
}

void
free_tree (RBNode *node)
{
  if (node != NULL)
  {
    free_tree (node->left);
    free_tree (node->right);
    free (node);
  }
}

RBNode *
new_node (int value)
{
  RBNode *new_node = malloc (sizeof (RBNode));
  if (!new_node)
    return NULL;

  new_node->value = value;
  new_node->color = RED;
  new_node->left = NULL;
  new_node->right = NULL;
  return new_node;
}

RBNode *
append_node (RBNode *node, int value)
{
  if (node == NULL)
    return new_node (value);

  if (value < node->value)
    node->left = append_node (node->left, value);
  if (value > node->value)
    node->right = append_node (node->right, value);
  return node;
}

RBNode *
find_min_value (RBNode *node)
{
  if (node->left == NULL)
    return node;

  node = find_min_value (node->left);
  return node;
}

RBNode *
delete_node (RBNode *node, int value)
{
  if (node == NULL)
    return NULL;

  if (value < node->value)
    node->left = delete_node (node->left, value);
  if (value > node->value)
    node->right = delete_node (node->right, value);

  if (value == node->value)
  {
    if (node->right == NULL)
    {
      RBNode *tmp = node->left;
      free (node);
      return tmp;
    }
    if (node->left == NULL)
    {
      RBNode *tmp = node->right;
      free (node);
      return tmp;
    }
    RBNode *min = find_min_value (node->right);
    node->value = min->value;
    node->right = delete_node (node->right, min->value);
  }

  return node;
}

int
main (void)
{
  RBTree tree = {0};
  tree.root = new_node (50);
  append_node (tree.root, 40);
  append_node (tree.root, 39);
  append_node (tree.root, 41);
  append_node (tree.root, 60);
  append_node (tree.root, 48);
  append_node (tree.root, 49);
  append_node (tree.root, 59);

  // delete_node (tree.root, 60);
  // delete_node (tree.root, 40);
  delete_node (tree.root, 40);
  print_tree (tree.root);
  puts ("");
  printf ("smallest node: %d\n", find_min_value (tree.root)->value);
  free_tree (tree.root);
  return 0;
}
