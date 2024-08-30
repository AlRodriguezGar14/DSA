#include <stdio.h>
#include <stdlib.h>

int get_shortest_line(int *lines, int number_of_lines)
{
	int smallest = 0;
	for (int i = 0; i < number_of_lines; i++)
	{
		if (lines[i] < lines[smallest])
			smallest = i;
	}
	return smallest;
}

int main(int argc, char **argv)
{
	if (argc < 3)
		return 1;
	int idx = 1;
	int number_of_lines = atoi(argv[idx++]);
	if (argc < number_of_lines + 1)
		return 1;
	int newcomer = atoi(argv[idx++]);

	int *lines = malloc(number_of_lines * sizeof(int));
	for (int i = 0; i < number_of_lines; ++i)
	{
		lines[i] = atoi(argv[idx++]);
		printf("line %d, has %d people\n", i, lines[i]);
	}

	for (int i = 0; i < newcomer; ++i)
	{
		int shortest = get_shortest_line(lines, number_of_lines);
		printf("%d\n", lines[shortest]);
		lines[shortest]++;
	}
	for (int i = 0; i < number_of_lines; i++)
		printf("line %d, has %d people\n", i, lines[i]);
	free(lines);
	return 0;
}
