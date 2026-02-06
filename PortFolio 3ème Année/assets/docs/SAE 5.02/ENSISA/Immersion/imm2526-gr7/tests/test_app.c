#include <stdio.h>
#include "../includes/app.h"
#include <assert.h>

void test_in_bounds() {
    assert(in_bounds(0, 0) == TRUE);
    assert(in_bounds(8, 8) == TRUE);
    assert(in_bounds(-1, 0) == FALSE);
    assert(in_bounds(0, 9) == FALSE);
    printf("in_bounds OK\n");
}

void test_is_blue_piece() {
    assert(is_blue_piece(BLUE_SOLDIER) == TRUE);
    assert(is_blue_piece(BLUE_KING) == TRUE);
    assert(is_blue_piece(RED_SOLDIER) == FALSE);
    printf("is_blue_piece OK\n");
}

void test_is_red_piece() {
    assert(is_red_piece(RED_SOLDIER) == TRUE);
    assert(is_red_piece(RED_KING) == TRUE);
    assert(is_red_piece(BLUE_SOLDIER) == FALSE);
    printf("is_red_piece OK\n");
}

void test_is_ally_enemy() {
    assert(is_ally(BLUE_SOLDIER, BLUE) == TRUE);
    assert(is_ally(RED_SOLDIER, RED) == TRUE);
    assert(is_ally(RED_SOLDIER, BLUE) == FALSE);
    assert(is_enemy(RED_SOLDIER, BLUE) == TRUE);
    assert(is_enemy(BLUE_SOLDIER, RED) == TRUE);
    assert(is_enemy(BLUE_SOLDIER, BLUE) == FALSE);
    printf("is_ally/is_enemy OK\n");
}

void test_is_king() {
    assert(is_king(BLUE_KING) == TRUE);
    assert(is_king(RED_KING) == TRUE);
    assert(is_king(BLUE_SOLDIER) == FALSE);
    printf("is_king OK\n");
}

void test_new_game_setup() {
    new_game_setup();
    // Vérifie que le plateau est bien initialisé
    assert(board[1][1] == BLUE_KING);
    assert(board[7][7] == RED_KING);
    assert(current_player == BLUE);
    assert(turn_count == 1);
    printf("new_game_setup OK\n");
}

void test_try_move() {
    new_game_setup();
    // Déplacement d'un soldat bleu
    int r0 = 0, c0 = 2, r1 = 1, c1 = 2;
    assert(board[r0][c0] == BLUE_SOLDIER);
    assert(board[r1][c1] == BLUE_SOLDIER);
    // On vide la case d'arrivée pour le test
    board[r1][c1] = EMPTY;
    gboolean res = try_move(r0, c0, r1, c1);
    assert(res == TRUE);
    assert(board[r1][c1] == BLUE_SOLDIER);
    assert(board[r0][c0] == EMPTY);
    printf("try_move OK\n");
}

void test_is_city() {
    assert(is_city(0, 0) == TRUE);
    assert(is_city(8, 8) == TRUE);
    assert(is_city(4, 4) == FALSE);
    printf("is_city OK\n");
}

void test_is_game_over() {
    new_game_setup();
    // Simule la mort du roi bleu
    blue_king_alive = FALSE;
    assert(is_game_over() == TRUE);
    // Simule la mort du roi rouge
    new_game_setup();
    red_king_alive = FALSE;
    assert(is_game_over() == TRUE);
    // Simule la capture de 8 soldats bleus
    new_game_setup();
    captured_blue_soldiers = 8;
    assert(is_game_over() == TRUE);
    // Simule la capture de 8 soldats rouges
    new_game_setup();
    captured_red_soldiers = 8;
    assert(is_game_over() == TRUE);
    // Simule le dépassement du nombre de tours
    new_game_setup();
    turn_count = MAX_TURNS + 1;
    assert(is_game_over() == TRUE);
    printf("is_game_over OK\n");
}

int main() {
    test_in_bounds();
    test_is_blue_piece();
    test_is_red_piece();
    test_is_ally_enemy();
    test_is_king();
    test_new_game_setup();
    test_try_move();
    test_is_city();
    test_is_game_over();
    printf("Tous les tests app.c passés !\n");
    return 0;
}
