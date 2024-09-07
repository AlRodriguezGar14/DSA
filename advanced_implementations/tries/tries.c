#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_LEN 26
#define MAX_WORD_LEN 100
#define DEBUG 0

typedef struct s_TrieNode
{
  struct s_TrieNode *children[ALPHABET_LEN];
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
  for (int i = 0; i < ALPHABET_LEN; ++i)
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
    if (DEBUG == 1)
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

  for (int i = 0; i < ALPHABET_LEN; ++i)
    free_node (node->children[i]);
  free (node);
}

int
is_node_empty (t_TrieNode *node)
{
  for (int i = 0; i < ALPHABET_LEN; ++i)
  {
    if (node->children[i] != NULL)
      return 0;
  }
  return 1;
}

t_TrieNode *
delete_node (t_TrieNode *current, const char *target, int target_len, int index)
{
  if (current == NULL)
    return NULL;

  if (index == target_len)
  {
    if (current->is_value == 1)
    {
      current->is_value = 0;
      printf ("Removed: %s\n", target);
    }
    // If this node has no children and is not a word end, we can remove it
    if (is_node_empty (current))
    {
      free_node (current);
      return NULL;
    }
    return current;
  }

  int current_char = isupper (target[index]) ? tolower (target[index]) - 'a'
					     : target[index] - 'a';

  if (current_char < 0 || current_char >= ALPHABET_LEN)
  {
    printf ("Invalid character in word: %c\n", target[index]);
    return current;
  }

  current->children[current_char]
    = delete_node (current->children[current_char], target, target_len,
		   index + 1);

  // After deletion, check if this node should be removed
  if (is_node_empty (current) && !current->is_value)
  {
    free (current);
    return NULL;
  }

  return current;
}

void
free_trie (t_Trie *trie)
{
  if (trie == NULL)
    return;

  free_node (trie->entry_point);
  free (trie);
}

void
print_word (t_TrieNode *node, char *word, int position)
{
  if (node == NULL || position >= MAX_WORD_LEN - 1)
    return;

  if (node->is_value == 1)
  {
    word[position] = '\0';
    printf ("%s\n", word);
  }

  for (int i = 0; i < ALPHABET_LEN; ++i)
  {
    if (node->children[i] != NULL)
    {
      word[position] = i + 'a';
      print_word (node->children[i], word, position + 1);
    }
  }
}

void
print_trie (t_Trie *root)
{
  char word[MAX_WORD_LEN];
  bzero (word, MAX_WORD_LEN);

  print_word (root->entry_point, word, 0);
}

t_TrieNode *
find_node (t_TrieNode *current, char *target, int target_len, int index)
{
  if (current == NULL)
  {
    if (DEBUG == 1)
      puts ("non existent");
    return NULL;
  }

  if (index == target_len)
  {
    if (current->is_value == 1)
    {
      if (DEBUG == 1)
	puts ("target found");
      return current;
    }
    else
    {
      if (DEBUG == 1)
	puts ("end is not activated");
      return NULL;
    }
  }

  int current_char = isupper (target[index]) ? tolower (target[index]) - 'a'
					     : target[index] - 'a';

  return find_node (current->children[current_char], target, target_len,
		    index + 1);
}

t_TrieNode *
reach_node (t_TrieNode *current, char *target, int target_len, int index)
{
  if (current == NULL)
    return NULL;

  if (index == target_len)
    return current;

  int current_char = isupper (target[index]) ? tolower (target[index]) - 'a'
					     : target[index] - 'a';

  return reach_node (current->children[current_char], target, target_len,
		     index + 1);
}

void
print_trie_from (t_TrieNode *root, char *target, int target_len)
{
  char word[MAX_WORD_LEN];
  bzero (word, MAX_WORD_LEN);

  strncpy (word, target, target_len);
  t_TrieNode *start_node = reach_node (root, target, target_len, 0);

  if (start_node != NULL)
    print_word (start_node, word, target_len);
}

int
main ()
{
  t_Trie *trie = init_trie ();

  insert_node ("hola", 0, strlen ("hola"), trie->entry_point);
  insert_node ("hominido", 0, strlen ("hominido"), trie->entry_point);
  insert_node ("hoy", 0, strlen ("hoy"), trie->entry_point);
  insert_node ("ola", 0, strlen ("ola"), trie->entry_point);
  insert_node ("sol", 0, strlen ("sol"), trie->entry_point);

  puts ("print trie");
  print_trie (trie);
  delete_node (trie->entry_point, "ola", strlen ("ola"), 0);
  puts ("print trie");
  print_trie (trie);
  // find_node (trie->entry_point, "sol", strlen ("sol"), 0);
  printf ("\nSearching for related... ho...\n");
  print_trie_from (trie->entry_point, "ho", strlen ("ho"));

  free_trie (trie);
  return 1;
}
