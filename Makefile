
NAME        := bsq
CXX         := g++

OPTFLAGS    := -O3 \
               -march=native \
               -flto=auto \
               -fno-fat-lto-objects \
               -falign-functions=32 \
               -falign-loops=32 \
               -fno-plt \
               -DNDEBUG \

CXXFLAGS    := -std=c++20 -Wall -Wextra $(OPTFLAGS)
LDFLAGS     := $(OPTFLAGS)

SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := inc

SRCS        := $(shell find $(SRC_DIR) -type f -name "*.cpp" 2>/dev/null)
ifeq ($(strip $(SRCS)),)
SRCS        := $(wildcard *.cpp)
endif
OBJS        := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CXXFLAGS    += -I$(INC_DIR) -I$(SRC_DIR) -I.

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
