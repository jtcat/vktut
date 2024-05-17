CXX := g++

LD := g++

CXXFLAGS := -O2

CPPFLAGS := -std=c++17

DEPDIR := .deps

DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.d

LDLIBS := -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

LDFLAGS =

COMPILE.cc = $(CXX) $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c -o $@

LINK.o = $(LD) $(LDFLAGS) $(LDLIBS) -o $@

NAME = VulkanTest

SRCS = main.cpp HelloTriApp.cpp readfile.cpp

OBJS = $(SRCS:.cpp=.o)

DEPFILES := $(SRCS:%.cpp=$(DEPDIR)/%.d)

$(DEPDIR): ; @mkdir -p $@

all: $(NAME)

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	$(COMPILE.cc) $<

$(NAME): $(OBJS)
	$(LINK.o) $(OBJS)

test:	$(NAME)
	./$(NAME)

clean:
	$(RM) -r $(NAME) $(OBJS) $(DEPDIR)

re:	clean all

.PRECIOUS: $(DEPDIR)/%.d
$(DEPDIR)/%.d: ;

.PHONY: all test clean re

-include $(wildcard $(DEPFILES))
