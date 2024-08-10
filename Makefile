CXX = gcc -g3 -fsanitize=address

CXXFLAGS = -Wall -Wextra -Werror -I ./

SRCS =	main.c \
		array.c \
		mergesort.c

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

.PHONY: all clean fclean re
	
