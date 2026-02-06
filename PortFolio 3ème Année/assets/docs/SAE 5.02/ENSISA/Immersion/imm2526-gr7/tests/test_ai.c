#include "../includes/ai.h"
#include "../includes/app.h"
#include <stdio.h>
#include <string.h>

// Mock des variables globales et fonctions externes
Piece board[N][N];
Control control_grid[N][N];
Player current_player;
int turn_count;
int captured_blue_soldiers;
int captured_red_soldiers;
gboolean blue_king_alive = TRUE;
gboolean red_king_alive = TRUE;
gboolean highlight_moves[N][N];
gboolean is_city(int r, int c) { return FALSE; }
gboolean try_move(int r0, int c0, int r1, int c1) {
    // Mouvement simple pour test
    if (board[r0][c0] == EMPTY || board[r1][c1] != EMPTY) return FALSE;
    board[r1][c1] = board[r0][c0];
    board[r0][c0] = EMPTY;
    return TRUE;
}
void compute_possible_moves(int r0, int c0) {}
gboolean is_game_over() { return FALSE; }
void network_send_move_async(int r0, int c0, int r1, int c1) {}

// Test initialisation IA
void test_ai_init() {
    ai_init();
    printf("test_ai_init OK\n");
}

// Test activation IA
void test_ai_set_enabled() {
    ai_set_enabled(TRUE);
    if (ai_enabled) printf("test_ai_set_enabled OK\n");
    else printf("test_ai_set_enabled FAIL\n");
}

// Test choix joueur IA
void test_ai_set_player() {
    ai_set_player(BLUE);
    if (ai_player == BLUE) printf("test_ai_set_player OK\n");
    else printf("test_ai_set_player FAIL\n");
}

// Test mouvement IA sur plateau simple
void test_ai_move_simple() {
    memset(board, 0, sizeof(board));
    board[0][0] = BLUE_SOLDIER;
    board[7][7] = RED_KING;
    current_player = BLUE;
    ai_set_enabled(TRUE);
    ai_set_player(BLUE);
    gboolean res = ai_move();
    if (res && board[0][0] == EMPTY) printf("test_ai_move_simple OK\n");
    else printf("test_ai_move_simple FAIL\n");
}

int main() {
    test_ai_init();
    test_ai_set_enabled();
    test_ai_set_player();
    test_ai_move_simple();
    return 0;
}
