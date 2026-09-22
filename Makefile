CXX = g++
CXXFLAGS = -g -Wall

# 注意：snake 依赖 ncurses，hello 不依赖。所以把 LIBS 单独放在规则里
snake: snake.cpp
	$(CXX) $(CXXFLAGS) snake.cpp -o snake -lncurses

hello: hello.cpp
	$(CXX) $(CXXFLAGS) hello.cpp -o hello

.PHONY: clean
clean:
	rm -f snake hello