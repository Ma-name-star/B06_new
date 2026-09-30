# 编译器与选项
CXX = g++
CXXFLAGS = -g -Wall
LIBS = -lncurses
BUILD_DIR = build

# 自动获取当前目录下所有的 .cpp 文件
SRCS = $(wildcard *.cpp)
# 将 .cpp 替换为 build/ 目录下的目标文件 (例如: build/snake, build/hello)
TARGETS = $(patsubst %.cpp, $(BUILD_DIR)/%, $(SRCS))

# 默认目标：编译所有程序
all: $(TARGETS)

# 核心规则：build/xxx 依赖于 xxx.cpp
$(BUILD_DIR)/%: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

# 一键运行贪吃蛇
run: $(BUILD_DIR)/snake
	./$(BUILD_DIR)/snake

# 一键清理：删掉 build 文件夹（不会删你的代码！）
clean:
	rm -rf $(BUILD_DIR)

# 声明伪目标
.PHONY: all run clean