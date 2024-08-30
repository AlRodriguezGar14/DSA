#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MAX_SNOWFLAKES 10000

typedef struct s_snowflake_node
{
	int *flake;
	struct s_snowflake_node *next;
} t_snowflake_node;

void free_snowflakes(t_snowflake_node **hashmap, int size)
{
	for (int i = 0; i < size; ++i)
	{
		t_snowflake_node *current = hashmap[i];
		while (current != NULL)
		{
			t_snowflake_node *next = current->next;
			free(current);
			current = next;
		}
	}
	free(hashmap);
}
int **parse_snowflakes(char *filename, int *number_of_snowflakes,
					   t_snowflake_node **hashmap)

{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		fprintf(stderr, "file not found\n");
		return NULL;
	}
	char *line = calloc(1025, sizeof(char));
	size_t len = 1024;

	if (getline(&line, &len, file) < 0)
	{
		printf("Incatch size");
		free(line);
		fclose(file);
		return NULL;
	}
	*number_of_snowflakes = atoi(line);
	if (*number_of_snowflakes <= 0)
	{
		printf("Number of snowflakes < 0)");
		free(line);
		fclose(file);
		return NULL;
	}

	int **list_of_snowflakes = calloc(*number_of_snowflakes, sizeof(int *));
	for (int i = 0; i < *number_of_snowflakes; ++i)
		list_of_snowflakes[i] = calloc(6, sizeof(int));
	int idx = 0;
	while (getline(&line, &len, file) != -1)
	{
		sscanf(line, "%d %d %d %d %d %d", &list_of_snowflakes[idx][0],
			   &list_of_snowflakes[idx][1], &list_of_snowflakes[idx][2],
			   &list_of_snowflakes[idx][3], &list_of_snowflakes[idx][4],
			   &list_of_snowflakes[idx][5]);
		int sum = 0;
		for (int i = 0; i < 6; ++i)
			sum += list_of_snowflakes[idx][i];
		sum = sum % MAX_SNOWFLAKES;
		if (hashmap[sum] == NULL)
		{
			hashmap[sum] = malloc(sizeof(t_snowflake_node));
			hashmap[sum]->flake = list_of_snowflakes[idx];
			hashmap[sum]->next = NULL;
		}
		else
		{
			t_snowflake_node *set = malloc(sizeof(t_snowflake_node));
			set->flake = list_of_snowflakes[idx];
			set->next = hashmap[sum];
			hashmap[sum] = set;
		}
		idx++;
	}
	free(line);
	fclose(file);
	return list_of_snowflakes;
}

int check_right_side(t_snowflake_node *prev, t_snowflake_node *next, int offset)
{
	for (int i = 0; i < 6; ++i)
	{
		if (prev->flake[i] != next->flake[(i + offset) % 6])
			return -1;
	}
	return 1;
}

int check_left_side(t_snowflake_node *prev, t_snowflake_node *next, int offset)
{
	for (int i = 0; i < 6; ++i)
	{
		int next_idx = (i - offset + 6) % 6;
		if (prev->flake[i] != next->flake[next_idx])
			return -1;
	}
	return 1;
}

int check_identical(t_snowflake_node *prev, t_snowflake_node *next)
{
	if (prev == NULL || next == NULL)
		return -1;
	int offset = -1;
	for (int i = 0; i < 6; ++i)
	{
		if (next->flake[i] == prev->flake[0])
		{
			offset = i;
			break;
		}
	}
	if (offset == -1)
		return -1;
	if (check_right_side(prev, next, offset) > 0)
		return 1;
	if (check_left_side(prev, next, offset) > 0)
		return 1;
	return -1;
}

int main()
{
	t_snowflake_node **hashmap =
		calloc(MAX_SNOWFLAKES, sizeof(t_snowflake_node));
	int number_of_snowflakes;
	int **list_of_snowflakes = parse_snowflakes("large_snowflakes.txt",
												&number_of_snowflakes, hashmap);
	if (!list_of_snowflakes)
	{
		free(hashmap);
		fprintf(stderr, "not enough snowflakes\n");
		return 1;
	}

	for (int i = 0; i < MAX_SNOWFLAKES; ++i)
	{
		t_snowflake_node *current = hashmap[i];
		while (current != NULL)
		{
			if (check_identical(current, current->next) == 1)
			{
				printf("There are two identical snowflakes: \n");
				for (int i = 0; i < 6; ++i)
					printf("%d ", current->flake[i]);
				printf(" | ");
				for (int i = 0; i < 6; ++i)
					printf("%d ", current->next->flake[i]);
				printf("\n");
			}
			current = current->next;
		}
	}

	for (int i = 0; i < number_of_snowflakes; ++i)
		free(list_of_snowflakes[i]);
	free_snowflakes(hashmap, MAX_SNOWFLAKES);
	free(list_of_snowflakes);
	return 0;
}
