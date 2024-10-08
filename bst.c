#include "dsa.h"

t_Node *
new_node (int id)
{
  t_Node *node = malloc (sizeof (t_Node));
  node->id = id;
  node->left = NULL;
  node->right = NULL;
  return node;
}

t_Node *
bst_insert (t_Node *root, int id)
{
  if (root == NULL)
    return new_node (id);

  if (root->id < id)
    root->right = bst_insert (root->right, id);
  else if (root->id > id)
    root->left = bst_insert (root->left, id);
  return root;
}

bool
bst_find (t_Node *root, int id)
{
  if (root == NULL)
    return false;
  if (root->id > id)
    return bst_find (root->left, id);
  else if (root->id < id)
    return bst_find (root->right, id);
  else
    return true;
}

t_Node *
bst_get (t_Node *root, int id)
{
  if (root == NULL)
    return NULL;
  if (root->id > id)
    return bst_get (root->left, id);
  else if (root->id < id)
    return bst_get (root->right, id);
  else
    return root;
}

int
bst_smallest_value (t_Node *root)
{
  t_Node *curr = root;

  while (curr != NULL && curr->left != NULL)
    {
      curr = curr->left;
    }
  return curr->id;
}

int
bst_largest_value (t_Node *root)
{
  t_Node *curr = root;

  while (curr && curr->right)
    {
      curr = curr->right;
    }
  return curr->id;
}

t_Node *
bst_smallest_node (t_Node *root)
{
  t_Node *curr = root;

  while (curr != NULL && curr->left != NULL)
    {
      curr = curr->left;
    }
  return curr;
}

t_Node *
bst_remove_node (t_Node *root, int id)
{
  if (!root)
    return NULL;

  if (id > root->id)
    root->right = bst_remove_node (root->right, id);
  else if (id < root->id)
    root->left = bst_remove_node (root->left, id);
  else
    {
      if (!root->left)
	{
	  t_Node *tmp = root->right;
	  free (root);
	  return tmp;
	}
      if (!root->right)
	{
	  t_Node *tmp = root->left;
	  free (root);
	  return tmp;
	}
      t_Node *min = bst_smallest_node (root->right);
      root->id = min->id;
      root->right = bst_remove_node (root->right, min->id);
    }

  return root;
}

// Depth First Search
/* DFS: display the tree ordered */
void
bst_traverse (t_Node *root)
{
  if (!root)
    return;
  bst_traverse (root->left);
  printf ("%d ", root->id);
  bst_traverse (root->right);
}

/* BFS: display the tree by levels */
static inline void
enqueue_children (t_NQueue *queue, t_Node *current)
{
  if (current->left)
    nenqueue (queue, current->left);
  if (current->right)
    nenqueue (queue, current->right);
}

static inline void
process_level (t_NQueue *queue, int len, int level)
{
  printf ("level: %d ->", level);
  for (int i = 0; i < len; ++i)
    {
      enqueue_children (queue, queue->head->value);
      printf (" %d", queue->head->value->id);
      ndequeue (queue);
    }
  puts ("");
}

void
breadth_first_search (t_Node *root)
{
  t_NQueue *queue = new_nqueue ();

  if (root)
    nenqueue (queue, root);

  int level = 0;
  while (queue->len)
    {
      process_level (queue, queue->len, level++);
    }

  free_nq (queue);
}

void
free_tree (t_Node *root)
{
  if (root != NULL)
    {
      free_tree (root->left);
      free_tree (root->right);
      free (root);
    }
}

void
print_tree (t_Node *root, int space)
{
  if (root == NULL)
    return;

  space += 8;
  print_tree (root->right, space);
  printf ("\n%*s[%d]\n", space, "", root->id);
  print_tree (root->left, space);
}
