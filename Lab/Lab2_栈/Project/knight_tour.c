#include <stdio.h>

#define MAX_BOARD 8
#define MOVE_COUNT 8

int board_recursive[MAX_BOARD][MAX_BOARD];
int board_iterative[MAX_BOARD][MAX_BOARD];
int dx[MOVE_COUNT] = {2, 1, -1, -2, -2, -1, 1, 2};
int dy[MOVE_COUNT] = {1, 2, 2, 1, -1, -2, -2, -1};

void reset_board(int board[MAX_BOARD][MAX_BOARD], int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            board[i][j] = 0;
        }
    }
}

void print_board(int board[MAX_BOARD][MAX_BOARD], int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%2d ", board[i][j]);
        }
        printf("\n");
    }
}

int inside_board(int n, int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < n;
}

int dfs_knight(int n, int x, int y, int step) {
    int k;
    board_recursive[x][y] = step;
    if (step == n * n) {
        return 1;
    }
    for (k = 0; k < MOVE_COUNT; k++) {
        int nx = x + dx[k];
        int ny = y + dy[k];
        if (inside_board(n, nx, ny) && board_recursive[nx][ny] == 0) {
            if (dfs_knight(n, nx, ny, step + 1)) {
                return 1;
            }
        }
    }
    board_recursive[x][y] = 0;
    return 0;
}

void run_recursive_knight() {
    int n, sx, sy;
    printf("请输入棋盘大小n(<=8): ");
    if (scanf("%d", &n) != 1) {
        printf("输入错误。\n");
        return;
    }
    if (n <= 0 || n > MAX_BOARD) {
        printf("棋盘尺寸超出范围。\n");
        return;
    }
    printf("请输入骑士起点的行列: ");
    if (scanf("%d %d", &sx, &sy) != 2) {
        printf("输入错误。\n");
        return;
    }
    if (sx < 1 || sx > n || sy < 1 || sy > n) {
        printf("起点超出棋盘。\n");
        return;
    }

    reset_board(board_recursive, n);
    if (dfs_knight(n, sx - 1, sy - 1, 1)) {
        printf("找到了一条路线:\n");
        print_board(board_recursive, n);
    } else {
        printf("没有找到路线。\n");
    }
}

typedef struct {
    int x;
    int y;
    int next_move;
} Frame;

void run_iterative_knight() {
    int n, sx, sy;
    Frame stack[MAX_BOARD * MAX_BOARD];
    int top;
    int step;

    printf("请输入棋盘大小n(<=8): ");
    if (scanf("%d", &n) != 1) {
        printf("输入错误。\n");
        return;
    }
    if (n <= 0 || n > MAX_BOARD) {
        printf("棋盘尺寸超出范围。\n");
        return;
    }
    printf("请输入骑士起点的行列: ");
    if (scanf("%d %d", &sx, &sy) != 2) {
        printf("输入错误。\n");
        return;
    }
    if (sx < 1 || sx > n || sy < 1 || sy > n) {
        printf("起点超出棋盘。\n");
        return;
    }

    reset_board(board_iterative, n);
    top = 0;
    step = 1;
    stack[0].x = sx - 1;
    stack[0].y = sy - 1;
    stack[0].next_move = 0;
    board_iterative[sx - 1][sy - 1] = 1;

    while (top >= 0) {
        if (step == n * n) {
            printf("非递归方法找到了一条路线:\n");
            print_board(board_iterative, n);
            return;
        }

        if (stack[top].next_move >= MOVE_COUNT) {
            board_iterative[stack[top].x][stack[top].y] = 0;
            top--;
            step--;
            continue;
        }

        {
            int move_id = stack[top].next_move;
            int nx = stack[top].x + dx[move_id];
            int ny = stack[top].y + dy[move_id];
            stack[top].next_move++;
            if (inside_board(n, nx, ny) && board_iterative[nx][ny] == 0) {
                top++;
                step++;
                stack[top].x = nx;
                stack[top].y = ny;
                stack[top].next_move = 0;
                board_iterative[nx][ny] = step;
            }
        }
    }

    printf("非递归方法没有找到路线。\n");
}

void clear_line() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

int main() {
    int choice;
    while (1) {
        printf("====== 骑士巡逻 ======\n");
        printf("1. 递归回溯\n");
        printf("2. 栈模拟非递归\n");
        printf("0. 退出\n");
        printf("请选择: ");
        if (scanf("%d", &choice) != 1) {
            printf("请输入数字选项。\n");
            clear_line();
            continue;
        }
        clear_line();
        if (choice == 1) {
            run_recursive_knight();
        } else if (choice == 2) {
            run_iterative_knight();
        } else if (choice == 0) {
            printf("程序结束。\n");
            break;
        } else {
            printf("没有这个选项。\n");
        }
        printf("\n");
    }
    return 0;
}

