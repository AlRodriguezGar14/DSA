#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RED false
#define BLACK true
#define ROOT 0
#define LEFT 1
#define RIGHT 2

typedef struct RBNode RBNode;
struct RBNode
{
  int value;
  bool color;
  int side;
  RBNode *parent, *left, *right;
};

typedef struct
{
  RBNode *root;
} RBTree;

char *
get_color (bool color)
{
  return color == RED ? "Red" : "Black";
}
char *
get_side (int side)
{
  if (side == ROOT)
    return "Root";
  return side == LEFT ? "Left" : "Right";
}

void
print_tree_helper (RBNode *node, int space, int indent_incr)
{
  if (node == NULL)
    return;

  space += indent_incr;
  print_tree_helper (node->right, space, indent_incr);
  printf ("\n");
  for (int i = indent_incr; i < space; i++)
    printf (" ");
  printf ("(%d,%s,%s)\n", node->value, get_color (node->color),
          get_side (node->side));
  print_tree_helper (node->left, space, indent_incr);
}

void
print_tree (RBNode *root)
{
  print_tree_helper (root, 0, 7);
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
  new_node->parent = NULL;
  new_node->side = ROOT;
  return new_node;
}

void
left_rotate (RBNode *node)
{
  RBNode *tmp = node->right;
  node->right = tmp->left;
  if (node->right != NULL)
  {
    node->right->parent = node;
    node->right->side = RIGHT;
  }
  if (!node->parent) // This is the root position
  {
    tmp->parent = NULL;
    tmp->side = ROOT;
    node = tmp;
  }
  else
  {
    tmp->parent = node->parent;
    if (node->side == LEFT)
    {
      tmp->side = LEFT;
      tmp->parent->left = tmp;
    }
    else if (node->side == RIGHT)
    {
      tmp->side = RIGHT;
      tmp->parent->right = tmp;
    }
  }
  tmp->left = node;
  node->side = LEFT;
  node->parent = tmp;
}

void
right_rotate (RBNode *node)
{
  RBNode *tmp = node->left;
  node->left = tmp->right;
  if (node->left != NULL)
  {
    node->left->parent = node;
    node->left->side = LEFT;
  }
  if (!node->parent) // This is the root position
  {
    tmp->parent = NULL;
    tmp->side = ROOT;
    node = tmp;
  }
  else
  {
    tmp->parent = node->parent;
    if (node->side == RIGHT)
    {
      tmp->side = RIGHT;
      tmp->parent->right = tmp;
    }
    else if (node->side == LEFT)
    {
      tmp->side = LEFT;
      tmp->parent->left = tmp;
    }
  }
  tmp->left = node;
  node->side = RIGHT;
  node->parent = tmp;
}

void
rotate (RBNode *child, RBNode *parent, RBNode *grandparent, RBNode *uncle)
{
  // left left
  if (child->side == LEFT && parent->side == LEFT)
  {
    puts ("ll");
    right_rotate (grandparent);
  }

  // righ right
  if (child->side == RIGHT && parent->side == RIGHT)
  {
    puts ("rr");
    left_rotate (grandparent);
  }

  // TODO: left right

  // TODO: right left
}

void
check_violations (RBNode *child)
{
  if (!child || !child->parent)
    return;
  RBNode *parent = child->parent;
  RBNode *grandparent = parent->parent;
  RBNode *uncle;
  if (grandparent == NULL)
    uncle = NULL;
  else
    uncle = parent->side == LEFT ? grandparent->right : grandparent->left;

  if (parent->color == RED && (uncle == NULL || uncle->color == BLACK))
  {
    puts ("rotate");
    rotate (child, parent, grandparent, uncle);
  }
  if (parent->color == RED && (uncle && uncle->color == RED))
  {
    puts ("color");
    parent->color = BLACK;
    uncle->color = BLACK;
    if (grandparent->side != ROOT)
      grandparent->color = RED;
  }
  check_violations (parent);
}

RBNode *
append_node (RBNode *root, int value)
{
  if (root == NULL)
    return new_node (value);

  RBNode *current = root;
  RBNode *parent = NULL;

  while (current != NULL)
  {
    parent = current;

    if (value < current->value)
      current = current->left;
    else if (value > current->value)
      current = current->right;
    else
      return root; // Value already exists in the tree
  }

  RBNode *new = new_node (value);
  new->parent = parent;

  if (value < parent->value)
  {
    new->side = LEFT;
    parent->left = new;
  }
  else
  {
    new->side = RIGHT;
    parent->right = new;
  }

  check_violations (new);
  return root;
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
  tree.root->color = BLACK;
  append_node (tree.root, 40);
  append_node (tree.root, 60);
  append_node (tree.root, 12);
  append_node (tree.root, 10);
  append_node (tree.root, 9);
  append_node (tree.root, 13);
  append_node (tree.root, 39);
  append_node (tree.root, 41);
  append_node (tree.root, 60);
  append_node (tree.root, 48);
  append_node (tree.root, 49);
  append_node (tree.root, 59);
  append_node (tree.root, 58);

  // delete_node (tree.root, 60);
  // delete_node (tree.root, 40);
  // delete_node (tree.root, 40);
  print_tree (tree.root);
  puts ("");
  printf ("smallest node: %d\n", find_min_value (tree.root)->value);
  free_tree (tree.root);
  return 0;
}
