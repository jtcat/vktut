CXX = g++
#CFLAGS = -std=c++17 -g -O0 -fsanitize=address
CFLAGS = -std=c++17 -g -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

NAME = VulkanTest

all:	$(NAME)

readfile.o:	readfile.cpp readfile.h
	$(CXX) $(CFLAGS) -c $< -o $@

main.o:	main.cpp HelloTriApp.h

$(NAME):	main.o readfile.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test:	$(NAME)
	./$(NAME)

clean:
	rm -f $(NAME)

re:	clean all

.PHONY: test clean
