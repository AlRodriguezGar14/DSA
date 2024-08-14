#include "dsa.h"

t_Node *new_node(int id) {
	t_Node *node = malloc(sizeof(t_Node));
	node->id = id;
	node->left = NULL;
	node->right = NULL;
	return node;
}

t_Node *bst_insert(t_Node *root, int id) {
	if (root == NULL)
		return new_node(id);

	if (root->id < id)
		root->right = bst_insert(root->right, id);
	else if (root->id > id)
		root->left = bst_insert(root->left, id);
	return root;
}

bool bst_find(t_Node *root, int id) {
	if (root == NULL)
		return false;
	if (root->id > id)
		return bst_find(root->left, id);
	else if (root->id < id)
		return bst_find(root->right, id);
	else
		return true;
}

t_Node *bst_get(t_Node *root, int id) {
	if (root == NULL)
		return NULL;
	if (root->id > id)
		return bst_get(root->left, id);
	else if (root->id < id)
		return bst_get(root->right, id);
	else
		return root;
}

void free_tree(t_Node *root) {
	if (root != NULL) {
		free_tree(root->left);
		free_tree(root->right);
		free(root);
	}
}

void print_tree(t_Node *root, int space) {

	int i;

	if (root == NULL)
		return;

	space += 8;

	print_tree(root->right, space);

	printf("\n");
	for (i = 10; i < space; i++)
		printf("~");
	printf("[%d]\n", root->id);

	print_tree(root->left, space);
}

void test_bst() {
	t_Node *root = new_node(10);

	bst_insert(root, 11);
	bst_insert(root, 15);
	bst_insert(root, 15);
	bst_insert(root, 5);
	bst_insert(root, 4);
	bst_insert(root, 2);
	bst_insert(root, 42);
	bst_insert(root, 9);
	bst_insert(root, 20);
	bst_insert(root, 14);
	print_tree(root, 0);

	printf("The search for 42 is: %b\n", bst_find(root, 42));
	printf("The search for 24 is: %b\n", bst_find(root, 24));

	t_Node *retrieved = bst_get(root, 9);
	if (retrieved != NULL)
		printf("Retrieved is: %d\n", retrieved->id);
	t_Node *retrieved2 = bst_get(root, 999);
	if (!retrieved2)
		printf("Id 999 not found\n");
	free_tree(root);
}
