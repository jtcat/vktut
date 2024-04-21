CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

NAME = VulkanTest

all:	$(NAME)

$(NAME):	main.cpp
	g++ $(CFLAGS) -o $@ main.cpp $(LDFLAGS)

test:	$(NAME)
	./$(NAME)

clean:
	rm -f $(NAME)

re:	clean all

.PHONY: test clean
