// TIC TAC AND TOE GAME!!!!!

/*
  Tic-Tac-Toe (C)
  - Modes: 2-player local, or Player vs Computer ( Minimax -> unbeatable)
  - Portable `clear` (Windows and Unix)
  - Simple text UI, numbered cells 1..9
  - Compile: gcc tictactoe.c -o tictactoe
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef _WIN32
  #define CLEAR_CMD "cls"
#else
  #define CLEAR_CMD "clear"
#endif

// Board is a 9-char array: indices 0..8 map to positions:
// 0 1 2
// 3 4 5
// 6 7 8

// Function prototypes
void clear_screen();
void print_board(const char board[9]);
int check_winner(const char board[9]); // returns 'X' or 'O' as int, or 0 for none, 2 for draw
int is_moves_left(const char board[9]);
int minimax(char board[9], int depth, int isMaximizing);
int find_best_move(char board[9]);
void player_move(char board[9], char player);
void ai_move(char board[9]);
void play_pvp();
void play_pvai();

void clear_screen() {
    system(CLEAR_CMD);
}

void print_board(const char board[9]) {
    // Show positions 1..9 for empty cells to help player
    printf("\n");
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            int i = r*3 + c;
            char ch = board[i];
            if (ch == ' ') printf(" %d ", i+1);
            else printf(" %c ", ch);
            if (c < 2) printf("|");
        }
        printf("\n");
        if (r < 2) printf("---+---+---\n");
    }
    printf("\n");
}

// Return ASCII code of winner 'X' or 'O', or 0 for no winner yet,
// or 2 if it's a draw.
int check_winner(const char board[9]) {
    int lines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8}, // rows
        {0,3,6},{1,4,7},{2,5,8}, // cols
        {0,4,8},{2,4,6}          // diagonals
    };
    for (int i = 0; i < 8; ++i) {
        int a = lines[i][0], b = lines[i][1], c = lines[i][2];
        if (board[a] != ' ' && board[a] == board[b] && board[b] == board[c]) {
            return (int)board[a];
        }
    }
    if (!is_moves_left(board)) return 2; // draw marker
    return 0;
}

int is_moves_left(const char board[9]) {
    for (int i = 0; i < 9; ++i) if (board[i] == ' ') return 1;
    return 0;
}

/*
  Minimax:
  - Returns score: +10 - depth if AI ('O') wins, -10 + depth if player ('X') wins, 0 for draw.
  - Using depth to prefer faster wins and slower losses.
*/
int minimax(char board[9], int depth, int isMaximizing) {
    int result = check_winner(board);
    if (result == 'O') return 10 - depth;
    if (result == 'X') return -10 + depth;
    if (result == 2) return 0; // draw

    if (isMaximizing) {
        int best = INT_MIN;
        for (int i = 0; i < 9; ++i) {
            if (board[i] == ' ') {
                board[i] = 'O';
                int val = minimax(board, depth + 1, 0);
                board[i] = ' ';
                if (val > best) best = val;
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < 9; ++i) {
            if (board[i] == ' ') {
                board[i] = 'X';
                int val = minimax(board, depth + 1, 1);
                board[i] = ' ';
                if (val < best) best = val;
            }
        }
        return best;
    }
}

// Find the best move for AI ('O')
int find_best_move(char board[9]) {
    int bestVal = INT_MIN;
    int bestMove = -1;
    for (int i = 0; i < 9; ++i) {
        if (board[i] == ' ') {
            board[i] = 'O';
            int moveVal = minimax(board, 0, 0);
            board[i] = ' ';
            if (moveVal > bestVal) {
                bestVal = moveVal;
                bestMove = i;
            }
        }
    }
    return bestMove;
}

void player_move(char board[9], char player) {
    int pos = -1;
    while (1) {
        printf("Player %c, enter position (1-9): ", player);
        if (scanf("%d", &pos) != 1) {
            // bad input, clear stdin
            while (getchar() != '\n');
            printf("Invalid input. Try again.\n");
            continue;
        }
        if (pos < 1 || pos > 9) {
            printf("Position must be 1..9. Try again.\n");
            continue;
        }
        if (board[pos-1] != ' ') {
            printf("Cell already taken. Pick another.\n");
            continue;
        }
        break;
    }
    board[pos-1] = player;
}

void ai_move(char board[9]) {
    int mv = find_best_move(board);
    if (mv == -1) {
        // fallback: pick first empty
        for (int i = 0; i < 9; ++i) if (board[i] == ' ') { mv = i; break; }
    }
    if (mv != -1) {
        printf("Computer chooses position %d\n", mv+1);
        board[mv] = 'O';
    }
}

void play_pvp() {
    char board[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};
    char current = 'X';
    while (1) {
        clear_screen();
        printf("Tic-Tac-Toe  (Player vs Player)\n");
        print_board(board);
        int res = check_winner(board);
        if (res == 'X' || res == 'O') {
            printf("Player %c wins!\n", (char)res);
            break;
        } else if (res == 2) {
            printf("It's a draw!\n");
            break;
        }
        player_move(board, current);
        // switch
        current = (current == 'X') ? 'O' : 'X';
    }
}

void play_pvai() {
    char board[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};
    char human = 'X';
    char turn = 'X'; // human always starts; could let user choose
    while (1) {
        clear_screen();
        printf("Tic-Tac-Toe  (Player X vs Computer O)\n");
        print_board(board);
        int res = check_winner(board);
        if (res == 'X' || res == 'O') {
            if (res == 'X') printf("You (X) win! Congratulations!\n");
            else printf("Computer (O) wins. Better luck next time.\n");
            break;
        } else if (res == 2) {
            printf("It's a draw!\n");
            break;
        }

        if (turn == human) {
            player_move(board, human);
        } else {
            // AI move
            ai_move(board);
            // small pause to let player see
            printf("Press Enter to continue...");
            while (getchar() != '\n'); // consume leftover newline if any
            getchar();
        }
        turn = (turn == 'X') ? 'O' : 'X';
    }
}

int main() {
    while (1) {
        clear_screen();
        printf("=== Tic-Tac-Toe ===\n");
        printf("1) Player vs Player\n");
        printf("2) Player vs Computer (unbeatable AI)\n");
        printf("3) Quit\n");
        printf("Select option: ");
        int opt = 0;
        if (scanf("%d", &opt) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (opt == 1) {
            play_pvp();
        } else if (opt == 2) {
            play_pvai();
        } else if (opt == 3) {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid option.\n");
        }
        printf("Play again? (y/n): ");
        char c;
        while (getchar() != '\n'); // flush newline
        c = getchar();
        if (c == 'y' || c == 'Y') continue;
        else break;
    }
    return 0;
}
