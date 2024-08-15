CXX = gcc -g3  #-fsanitize=address

CXXFLAGS = -Wall -Wextra -Werror -I ./

SRCS =	main.c \
		test_utils.c \
		array.c \
		queue.c \
		mergesort.c \
		quicksort.c \
		binary_search.c \
		bst.c

OBJS = $(SRCS:.c=.o)

TARGET = program

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking the object files to create the executable..."
	@$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	@echo "Compiling the source file $< into the object file $@..."
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	@echo "Removing the object files..."
	@rm -f $(OBJS) *.gch

fclean: clean
	@echo "Removing the executable..."
	@rm -f $(TARGET)

re: fclean all

sanitize: fclean $(OBJS)
	@echo "Compiling with fsanitize..."
	$(CXX) $(CXXFLAGS) -fsanitize=address -o $(TARGET) $(OBJS)

.PHONY: all clean fclean re
	
