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

/*  Visual explanation of the logic for removing a node:
 graph TD
    A[Start: delete_node] --> B{Is root NULL?}
    B -->|Yes| C[Return NULL]
    B -->|No| D{Is value > root->value?}
    D -->|Yes| E[Recursively delete from right subtree]
    D -->|No| F{Is value < root->value?}
    F -->|Yes| G[Recursively delete from left subtree]
    F -->|No| H{Node found: value == root->value}
    H --> I{Does node have left child?}
    I -->|No| J[Replace with right child]
    I -->|Yes| K{Does node have right child?}
    K -->|No| L[Replace with left child]
    K -->|Yes| M[Node has two children]
    M --> N[Find inorder successor]
    N --> O[Replace node's value with successor's value]
    O --> P[Recursively delete successor from right subtree]
    J --> Q[Return modified root]
    L --> Q
    P --> Q
    E --> Q
    G --> Q
 *
 * */

t_TreeNode *
smallest_node (t_TreeNode *root)
{
  t_TreeNode *curr = root;

  while (curr && curr->left)
    curr = curr->left;
  return curr;
}

t_TreeNode *
delete_node (t_TreeNode *root, int value)
{
  if (root == NULL)
    return NULL;

  if (value > root->value)
    root->right = delete_node (root->right, value);
  else if (value < root->value)
    root->left = delete_node (root->left, value);
  else
    {
      // Node found, perform deletion
      if (root->left == NULL)
	{
	  t_TreeNode *tmp = root->right;
	  free (root);
	  return tmp;
	}
      if (root->right == NULL)
	{
	  t_TreeNode *tmp = root->left;
	  free (root);
	  return tmp;
	}
      /*
       * Node with two children:
       * Find the smallest node in the right subtree. Replace the node to delete
       * with this one - remove the successor from the current position
       * */
      t_TreeNode *min = smallest_node (root->right);
      root->value = min->value;
      root->right = delete_node (root->right, min->value);
    }

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

/* in order */
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
  puts ("");
  delete_node (head, 24);
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
