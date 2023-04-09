CC = cc
CXX = c++
OBJCOPY = objcopy
AR = ar
MKDIR = mkdir -p
RM = rm -f

CPPFLAGS += -Wall -Wextra -Werror -fPIC -g -I'./include' -I'./src/include' -pipe
CFLAGS += $(CPPFLAGS)
CXXFLAGS += $(CPPFLAGS)
LDFLAGS += 

