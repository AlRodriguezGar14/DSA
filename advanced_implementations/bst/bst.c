#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_TreeNode
{
  int value;
  struct s_TreeNode *left;
  struct s_TreeNode *right;

} t_TreeNode;

t_TreeNode *
build_node (int value)
{
  t_TreeNode *node = malloc (sizeof (t_TreeNode));
  if (!node)
    return node;

  node->value = value;
  node->left = NULL;
  node->right = NULL;

  return node;
}

t_TreeNode *
add_node (t_TreeNode *root, int value)
{
  if (root == NULL)
    return build_node (value);

  if (value < root->value)
    root->left = add_node (root->left, value);
  if (value > root->value)
    root->right = add_node (root->right, value);

  return root;
}

void
free_tree (t_TreeNode *root)
{
  if (root != NULL)
    {
      free_tree (root->left);
      free_tree (root->right);
      free (root);
    }
}

void
bst_print_traverse (t_TreeNode *root)
{
  if (root == NULL)
    return;
  bst_print_traverse (root->left);
  printf (" [%d] ", root->value);
  bst_print_traverse (root->right);
}

t_TreeNode *
find_value (t_TreeNode *root, int value)
{
  if (root == NULL)
    return NULL;

  if (root->value == value)
    return root;
  if (value < root->value)
    root = find_value (root->left, value);
  if (value > root->value)
    root = find_value (root->right, value);
  return root;
}

int
main ()
{
  t_TreeNode *head = build_node (14);

  add_node (head, 42);
  add_node (head, 22);
  add_node (head, 21);
  add_node (head, 24);
  add_node (head, 12);
  add_node (head, 2);

  bst_print_traverse (head);

  printf ("\n");
  t_TreeNode *found;
  if ((found = find_value (head, 12)))
    printf ("Found value! %d\n", found->value);
  else
    puts ("NULL");
  if ((found = find_value (head, 122)))
    printf ("Found value! %d\n", found->value);
  else
    puts ("NULL");

  free_tree (head);
  return 0;
}
