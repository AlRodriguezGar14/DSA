#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
  RED = 0,
  BLACK,
} Color;

typedef enum
{
  ROOT = 0,
  LEFT,
  RIGHT
} Side;

typedef struct RBNode RBNode;
struct RBNode
{
  int value;
  Color color;
  Side side;
  RBNode *parent, *left, *right;
};

typedef struct
{
  RBNode *root;
} RBTree;

static const char *colors[] = {"Red", "Black"};
static const char *sides[] = {"Root", "Left", "Right"};

void
print_tree_helper (RBNode *node, int space, int indent_incr)
{
  if (node == NULL)
    return;

  space += indent_incr;
  print_tree_helper (node->right, space, indent_incr);
  printf ("\n");
  for (int i = indent_incr; i < space; i++)
    printf ("-");
  printf (">(%d,%s,%s)\n", node->value, colors[node->color], sides[node->side]);
  print_tree_helper (node->left, space, indent_incr);
}

void
print_tree (RBNode *root)
{
  print_tree_helper (root, 0, 12);
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

/* LEFT ROTATION */
RBNode *
update_right_child (RBNode *node)
{
  RBNode *right_child = node->right;
  node->right = right_child->left;
  if (right_child->left != NULL)
  {
    right_child->left->parent = node;
    right_child->left->side = RIGHT;
  }
  return right_child;
}

void
update_parent_after_left_rotate (RBNode *node, RBNode *right_child,
                                 RBNode **root)
{
  right_child->parent = node->parent;

  if (node->parent == NULL)
  {
    *root = right_child;
    right_child->side = ROOT;
  }
  else if (node == node->parent->left)
  {
    node->parent->left = right_child;
    right_child->side = LEFT;
  }
  else
  {
    node->parent->right = right_child;
    right_child->side = RIGHT;
  }
}

RBNode *
left_rotate (RBNode *root, RBNode *node)
{
  RBNode *right_child = update_right_child (node);
  update_parent_after_left_rotate (node, right_child, &root);

  right_child->left = node;
  node->parent = right_child;
  node->side = LEFT;

  return root;
}

/* RIGHT ROTATION */
RBNode *
update_left_child (RBNode *node)
{
  RBNode *left_child = node->left;
  node->left = left_child->right;
  if (left_child->right != NULL)
  {
    left_child->right->parent = node;
    left_child->right->side = LEFT;
  }
  return left_child;
}

void
update_parent_after_right_rotate (RBNode *node, RBNode *left_child,
                                  RBNode **root)
{
  left_child->parent = node->parent;

  if (node->parent == NULL)
  {
    *root = left_child;
    left_child->side = ROOT;
  }
  else if (node == node->parent->right)
  {
    node->parent->right = left_child;
    left_child->side = RIGHT;
  }
  else
  {
    node->parent->left = left_child;
    left_child->side = LEFT;
  }
}

RBNode *
right_rotate (RBNode *root, RBNode *node)
{
  RBNode *left_child = update_left_child (node);
  update_parent_after_right_rotate (node, left_child, &root);

  left_child->right = node;
  node->parent = left_child;
  node->side = RIGHT;

  return root;
}

/* FIX VIOLATION */
bool
is_parent_left_child (RBNode *parent, RBNode *grandparent)
{
  return parent == grandparent->left;
}

bool
is_node_red (RBNode *node)
{
  return node && node->color == RED;
}

void
recolor_nodes (RBNode *parent, RBNode *grandparent, RBNode *uncle)
{
  grandparent->color = RED;
  parent->color = BLACK;
  if (uncle != NULL)
    uncle->color = BLACK;
}

RBNode *
perform_left_rotate_and_recolor (RBNode *root, RBNode *parent,
                                 RBNode *grandparent)
{
  root = left_rotate (root, grandparent);
  bool temp_color = parent->color;
  parent->color = grandparent->color;
  grandparent->color = temp_color;
  return root;
}

RBNode *
handle_left_child_case (RBNode *root, RBNode *node)
{
  RBNode *parent = node->parent;
  RBNode *grandparent = parent->parent;
  RBNode *uncle = grandparent->right;

  if (is_node_red (uncle))
  {
    recolor_nodes (parent, grandparent, uncle);
    node = grandparent;
  }
  else
  {
    if (node == parent->right)
    {
      root = left_rotate (root, parent);
      node = parent;
      parent = node->parent;
    }
    root = right_rotate (root, grandparent);
    bool temp_color = parent->color;
    parent->color = grandparent->color;
    grandparent->color = temp_color;
    node = parent;
  }
  return root;
}

RBNode *
handle_right_child_case (RBNode *root, RBNode *node)
{
  RBNode *parent = node->parent;
  RBNode *grandparent = parent->parent;
  RBNode *uncle = grandparent->left;

  if (is_node_red (uncle))
  {
    recolor_nodes (parent, grandparent, uncle);
    node = grandparent;
  }
  else
  {
    if (node == parent->left)
    {
      root = right_rotate (root, parent);
      node = parent;
      parent = node->parent;
    }
    root = left_rotate (root, grandparent);
    bool temp_color = parent->color;
    parent->color = grandparent->color;
    grandparent->color = temp_color;
    node = parent;
  }
  return root;
}

RBNode *
fix_violation (RBNode *root, RBNode *node)
{
  while (node != root && is_node_red (node->parent))
  {
    RBNode *parent = node->parent;
    RBNode *grandparent = parent->parent;

    if (is_parent_left_child (parent, grandparent))
    {
      root = handle_left_child_case (root, node);
    }
    else
    {
      root = handle_right_child_case (root, node);
    }
  }
  root->color = BLACK;
  return root;
}

RBNode *
insert (RBNode *root, int value)
{
  RBNode *node = new_node (value);

  if (root == NULL)
  {
    node->color = BLACK;
    return node;
  }

  RBNode *parent = NULL;
  RBNode *current = root;

  while (current != NULL)
  {
    parent = current;
    if (node->value < current->value)
    {
      current = current->left;
    }
    else if (node->value > current->value)
    {
      current = current->right;
    }
    else
    {
      // Value already exists
      free (node);
      return root;
    }
  }

  node->parent = parent;

  if (node->value < parent->value)
  {
    parent->left = node;
    node->side = LEFT;
  }
  else
  {
    parent->right = node;
    node->side = RIGHT;
  }

  return fix_violation (root, node);
}

RBNode *
find_min_value (RBNode *node)
{
  while (node->left != NULL)
  {
    node = node->left;
  }
  return node;
}

RBNode *
fix_deletion (RBNode *root, RBNode *node_to_fix)
{
  RBNode *sibling_node;

  while (node_to_fix != root && node_to_fix->color == BLACK)
  {
    if (node_to_fix->side == LEFT)
    {
      sibling_node = node_to_fix->parent->right;

      // Case 1: Sibling is red
      if (sibling_node->color == RED)
      {
        sibling_node->color = BLACK;
        node_to_fix->parent->color = RED;
        root = left_rotate (root, node_to_fix->parent);
        sibling_node = node_to_fix->parent->right;
      }

      // Case 2: Sibling is black and both children are black
      if (sibling_node->left->color == BLACK
          && sibling_node->right->color == BLACK)
      {
        sibling_node->color = RED;
        node_to_fix = node_to_fix->parent;
      }
      else
      {
        // Case 3: Sibling is black, sibling's right child is red and left child
        // is black
        if (sibling_node->right->color == BLACK)
        {
          sibling_node->left->color = BLACK;
          sibling_node->color = RED;
          root = right_rotate (root, sibling_node);
          sibling_node = node_to_fix->parent->right;
        }

        // Case 4: Sibling's right child is red
        sibling_node->color = node_to_fix->parent->color;
        node_to_fix->parent->color = BLACK;
        sibling_node->right->color = BLACK;
        root = left_rotate (root, node_to_fix->parent);
        node_to_fix = root;
      }
    }
    else // node_to_fix->side == RIGHT
    {
      sibling_node = node_to_fix->parent->left;

      // Case 1: Sibling is red
      if (sibling_node->color == RED)
      {
        sibling_node->color = BLACK;
        node_to_fix->parent->color = RED;
        root = right_rotate (root, node_to_fix->parent);
        sibling_node = node_to_fix->parent->left;
      }

      // Case 2: Sibling is black and both children are black
      if (sibling_node->right->color == BLACK
          && sibling_node->left->color == BLACK)
      {
        sibling_node->color = RED;
        node_to_fix = node_to_fix->parent;
      }
      else
      {
        // Case 3: Sibling is black, sibling's left child is red and right child
        // is black
        if (sibling_node->left->color == BLACK)
        {
          sibling_node->right->color = BLACK;
          sibling_node->color = RED;
          root = left_rotate (root, sibling_node);
          sibling_node = node_to_fix->parent->left;
        }

        // Case 4: Sibling's left child is red
        sibling_node->color = node_to_fix->parent->color;
        node_to_fix->parent->color = BLACK;
        sibling_node->left->color = BLACK;
        root = right_rotate (root, node_to_fix->parent);
        node_to_fix = root;
      }
    }
  }

  node_to_fix->color = BLACK;
  return root;
}

/* REPLACE SUBTREES */
RBNode *
transplant (RBNode *root, RBNode *node_to_replace, RBNode *replacement_node)
{
  if (node_to_replace->parent == NULL)
  {
    root = replacement_node;
  }
  else if (node_to_replace == node_to_replace->parent->left)
  {
    node_to_replace->parent->left = replacement_node;
  }
  else
  {
    node_to_replace->parent->right = replacement_node;
  }
  if (replacement_node != NULL)
  {
    replacement_node->parent = node_to_replace->parent;
  }
  return root;
}

RBNode *
delete_node (RBNode *root, int value)
{
  RBNode *target_node = root;
  RBNode *replacement_node, *node_to_fix;

  // Search for the node to delete
  while (target_node != NULL && target_node->value != value)
  {
    if (value < target_node->value)
    {
      target_node = target_node->left;
    }
    else
    {
      target_node = target_node->right;
    }
  }

  if (target_node == NULL)
  {
    return root; // Node not found
  }

  node_to_fix = target_node;
  Color original_color = node_to_fix->color;

  if (target_node->left == NULL)
  {
    replacement_node = target_node->right;
    root = transplant (root, target_node, target_node->right);
  }
  else if (target_node->right == NULL)
  {
    replacement_node = target_node->left;
    root = transplant (root, target_node, target_node->left);
  }
  else
  {
    node_to_fix = find_min_value (target_node->right);
    original_color = node_to_fix->color;
    replacement_node = node_to_fix->right;

    if (node_to_fix->parent == target_node)
    {
      if (replacement_node != NULL)
      {
        replacement_node->parent = node_to_fix;
      }
    }
    else
    {
      root = transplant (root, node_to_fix, node_to_fix->right);
      node_to_fix->right = target_node->right;
      node_to_fix->right->parent = node_to_fix;
    }

    root = transplant (root, target_node, node_to_fix);
    node_to_fix->left = target_node->left;
    node_to_fix->left->parent = node_to_fix;
    node_to_fix->color = target_node->color;
  }

  free (target_node);

  if (original_color == BLACK && replacement_node != NULL)
  {
    root = fix_deletion (root, replacement_node);
  }

  return root;
}

int
main (void)
{
  RBTree tree = {0};
  tree.root = insert (tree.root, 6);
  tree.root = insert (tree.root, 7);
  tree.root = insert (tree.root, 8);
  tree.root = insert (tree.root, 9);
  tree.root = insert (tree.root, 10);
  tree.root = insert (tree.root, 11);
  tree.root = insert (tree.root, 1);
  tree.root = insert (tree.root, 2);
  tree.root = insert (tree.root, 3);
  tree.root = insert (tree.root, 4);
  tree.root = insert (tree.root, 5);
  print_tree (tree.root);
  puts ("======");
  tree.root = delete_node (tree.root, 6);
  tree.root = delete_node (tree.root, 4);
  print_tree (tree.root);

  free_tree (tree.root);
  return 0;
}
