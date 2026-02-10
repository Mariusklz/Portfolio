#ifdef NO_GUI
typedef int gboolean;
typedef void* gpointer;
#endif

extern gboolean check_network_moves(gpointer user_data);

#include "../includes/app.h"
#include "../includes/gui.h"

// Suppression du mock network_check_move (plus de warning)

void test_check_network_moves() {
    printf("Test : check_network_moves\n");
    printf("Test : check_network_moves\n");
    gboolean res = check_network_moves(NULL);
    printf("Résultat check_network_moves() = %d\n", res);
}

extern int network_socket;
extern Player current_player;
extern Player local_player;
extern gboolean is_local_player_turn();

void test_is_local_player_turn() {
    printf("Test : is_local_player_turn\n");
    int ok = 1;
    // Cas 1 : socket fermé
    network_socket = -1;
    current_player = BLUE;
    local_player = BLUE;
    gboolean res1 = is_local_player_turn();
    if (res1 == TRUE) {
        printf("OK : is_local_player_turn() socket fermé\n");
    } else {
        printf("ERREUR : is_local_player_turn() socket fermé\n");
        ok = 0;
    }
    // Cas 2 : socket ouvert, joueur courant = local
    network_socket = 5;
    current_player = RED;
    local_player = RED;
    gboolean res2 = is_local_player_turn();
    if (res2 == TRUE) {
        printf("OK : is_local_player_turn() socket ouvert, joueur local = courant\n");
    } else {
        printf("ERREUR : is_local_player_turn() socket ouvert, joueur local = courant\n");
        ok = 0;
    }
    // Cas 3 : socket ouvert, joueur courant != local
    network_socket = 5;
    current_player = BLUE;
    local_player = RED;
    gboolean res3 = is_local_player_turn();
    if (res3 == FALSE) {
        printf("OK : is_local_player_turn() socket ouvert, joueur local != courant\n");
    } else {
        printf("ERREUR : is_local_player_turn() socket ouvert, joueur local != courant\n");
        ok = 0;
    }
    if (ok) printf("OK : Tous les tests is_local_player_turn\n");
}

int main() {
    printf("Début des tests gui.c\n");
    test_is_local_player_turn();
    test_check_network_moves();
    printf("Fin des tests gui.c\n");
    return 0;
}
