#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

typedef struct s_TrieNode
{
  struct s_TrieNode *children[26];
  int8_t is_value;

} t_TrieNode;

typedef struct s_Trie
{
  t_TrieNode *entry_point;
} t_Trie;

t_TrieNode *
init_node ()
{
  t_TrieNode *node = malloc (sizeof (t_TrieNode));
  if (node == NULL)
    return NULL;
  for (int i = 0; i < 26; ++i)
    node->children[i] = NULL;
  node->is_value = 0;

  return node;
}

t_Trie *
init_trie ()
{
  t_Trie *root = malloc (sizeof (t_Trie));
  if (root == NULL)
    return NULL;
  root->entry_point = init_node ();
  return root;
}

void
insert_node (const char *target, int index, int len, t_TrieNode *current)
{
  if (index == len)
  {
    current->is_value = 1;
    printf ("Activated: %s\n", target);
    return;
  }

  int current_char = isupper (target[index]) ? tolower (target[index]) - 'a'
					     : target[index] - 'a';

  if (!current->children[current_char])
  {
    current->children[current_char] = init_node ();
    if (current->children[current_char] == NULL)
      printf ("handle malloc error\n");
    insert_node (target, index + 1, len, current->children[current_char]);
  }
  else
    insert_node (target, index + 1, len, current->children[current_char]);
}

void
free_node (t_TrieNode *node)
{
  if (node == NULL)
    return;

  for (int i = 0; i < 26; ++i)
    free_node (node->children[i]);
  free (node);
}

void
free_trie (t_Trie *trie)
{
  if (trie == NULL)
    return;

  free_node (trie->entry_point);
  free (trie);
}

int
main ()
{
  t_Trie *trie = init_trie ();
  char *hola = "hola";
  char *hoy = "hoy";
  char *hominido = "hominido";

  insert_node (hola, 0, strlen (hola), trie->entry_point);
  insert_node (hominido, 0, strlen (hominido), trie->entry_point);
  insert_node (hoy, 0, strlen (hoy), trie->entry_point);

  free_trie (trie);
  return 1;
}
