#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

// ========= 游戏配置 =========
const int BOARD_W = 40;   // 宽度（列）
const int BOARD_H = 20;   // 高度（行）
const int INIT_LEN = 4;   // 初始蛇长
const int TICK_MS = 120;  // 移动间隔（毫秒），越小越快

// 方向
enum Dir { UP, DOWN, LEFT, RIGHT };

// 坐标点
struct Point {
    int x, y;
};

class SnakeGame {
public:
    SnakeGame() {
        // ncurses 初始化
        initscr();            // 启动 ncurses 模式
        cbreak();             // 关闭行缓冲，按键立即返回
        noecho();             // 关闭回显
        curs_set(0);          // 隐藏光标
        keypad(stdscr, TRUE); // 支持方向键
        nodelay(stdscr, TRUE);// getch 非阻塞

        srand((unsigned)time(nullptr));
        reset();
    }

    ~SnakeGame() {
        endwin();  // 退出 ncurses 模式
    }

    void reset() {
        // 蛇初始在中间，向右走
        int cx = BOARD_W / 2;
        int cy = BOARD_H / 2;
        body.clear();
        for (int i = 0; i < INIT_LEN; ++i) {
            body.push_back({cx - i, cy});
        }
        dir = RIGHT;
        score = 0;
        spawnFood();
        gameOver = false;
    }

    void spawnFood() {
        // 在空白处随机生成食物
        while (true) {
            int fx = rand() % BOARD_W;
            int fy = rand() % BOARD_H;
            bool onSnake = false;
            for (auto& s : body) {
                if (s.x == fx && s.y == fy) { onSnake = true; break; }
            }
            if (!onSnake) { food = {fx, fy}; return; }
        }
    }

    void run() {
        while (true) {
            handleInput();
            if (!gameOver) update();
            draw();

            // 用 sleep_for 控制帧率
            std::this_thread::sleep_for(std::chrono::milliseconds(TICK_MS));
        }
    }

private:
    std::vector<Point> body;  // 蛇身，front 是头
    Point food;
    Dir dir;
    int score;
    bool gameOver;

    void handleInput() {
        int ch = getch();
        // 方向键 + WASD
        switch (ch) {
            case KEY_UP:    case 'w': case 'W':
                if (dir != DOWN)  dir = UP;    break;
            case KEY_DOWN:  case 's': case 'S':
                if (dir != UP)    dir = DOWN;  break;
            case KEY_LEFT:  case 'a': case 'A':
                if (dir != RIGHT) dir = LEFT;  break;
            case KEY_RIGHT: case 'd': case 'D':
                if (dir != LEFT)  dir = RIGHT; break;
            case 'r': case 'R':
                if (gameOver) reset(); break;
            case 'q': case 'Q':
                endwin();
                exit(0);
        }
    }

    void update() {
        Point head = body.front();
        switch (dir) {
            case UP:    head.y--; break;
            case DOWN:  head.y++; break;
            case LEFT:  head.x--; break;
            case RIGHT: head.x++; break;
        }

        // 碰墙检测
        if (head.x < 0 || head.x >= BOARD_W ||
            head.y < 0 || head.y >= BOARD_H) {
            gameOver = true; return;
        }
        // 碰自己检测
        for (auto& s : body) {
            if (s.x == head.x && s.y == head.y) {
                gameOver = true; return;
            }
        }

        body.insert(body.begin(), head);

        // 吃食物
        if (head.x == food.x && head.y == food.y) {
            score += 10;
            spawnFood();
        } else {
            body.pop_back();  // 没吃到就去尾，保持长度
        }
    }

    void draw() {
        // 先清屏
        clear();

        // 画顶部边框 + 分数
        mvprintw(0, 0, "+");
        for (int x = 0; x < BOARD_W; ++x) {
            mvprintw(0, x + 1, "-");
            mvprintw(BOARD_H + 1, x + 1, "-");
        }
        mvprintw(0, BOARD_W + 1, "+");
        mvprintw(BOARD_H + 1, 0, "+");
        mvprintw(BOARD_H + 1, BOARD_W + 1, "+");

        // 左右边框
        for (int y = 0; y < BOARD_H; ++y) {
            mvprintw(y + 1, 0, "|");
            mvprintw(y + 1, BOARD_W + 1, "|");
        }

        // 画食物
        mvprintw(food.y + 1, food.x + 1, "*");

        // 画蛇
        for (size_t i = 0; i < body.size(); ++i) {
            const char* ch = (i == 0) ? "@" : "o";  // 头 @，身 o
            mvprintw(body[i].y + 1, body[i].x + 1,"%s", ch);
        }

        // 画底部信息行
        mvprintw(BOARD_H + 3, 0, "Score: %d", score);
        mvprintw(BOARD_H + 4, 0, "WASD/方向键 移动 | R 重玩 | Q 退出");

        if (gameOver) {
            mvprintw(BOARD_H / 2, BOARD_W / 2 - 5, "GAME OVER!");
            mvprintw(BOARD_H / 2 + 1, BOARD_W / 2 - 9, "Press R to restart, Q to quit");
        }

        refresh();  // 真正把缓冲区内容画到屏幕上
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
