CXX := g++

CXXFLAGS := -O2

CPPFLAGS := -std=c++17

DEPDIR := .deps

DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.d

LDFLAGS := -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

COMPILE.cc = $(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c

NAME = VulkanTest

SRCS = main.cpp HelloTriApp.cpp readfile.cpp

OBJS = $(SRCS:.cpp=.o)

DEPFILES := $(SRCS:%.cpp=$(DEPDIR)/%.d)
$(DEPDIR)/%.d: ;

$(DEPDIR): ; @mkdir -p $@

all: $(NAME)

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	$(COMPILE.cc) -o $@ $<

$(NAME): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

test:	$(NAME)
	./$(NAME)

clean:
	$(RM) -r $(NAME) $(OBJS) $(DEPDIR)

re:	clean all

.PHONY: all test clean re

-include $(wildcard $(DEPFILES))
