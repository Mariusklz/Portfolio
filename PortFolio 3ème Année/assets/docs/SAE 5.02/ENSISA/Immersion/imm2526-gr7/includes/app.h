#ifndef APP_H
#define APP_H
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef NO_GUI
#include "../includes/header_commun.h"
#else
// Définition minimale pour les tests sans GTK
typedef int gboolean;
// typedef void* GtkWidget; // supprimé pour éviter conflit avec GTK4
#endif
#define N 9
#define MAX_TURNS 64


// Lance le mode local (GUI seule)
int app_run_local(void);

typedef enum {
    EMPTY = 0,
    BLUE_SOLDIER,
    BLUE_KING,
    RED_SOLDIER,
    RED_KING,
    CITY
} Piece;

typedef enum {
    CTRL_NEUTRAL = 0,
    CTRL_BLUE = 1,
    CTRL_RED = 2
} Control;

typedef enum {
    BLUE = 0,
    RED = 1
} Player;

// ---- Variables globales du jeu (définies dans app.c, utilisées dans gui.c) ----
extern Piece board[N][N];
extern Control control_grid[N][N];
extern Player current_player;
extern Player local_player;  // Joueur local (BLUE pour client, RED pour serveur, ou BLUE par défaut en local)
extern int turn_count;

extern int captured_blue_soldiers;
extern int captured_red_soldiers;
extern gboolean blue_king_alive;
extern gboolean red_king_alive;

extern gboolean has_selection;
extern int sel_r, sel_c;
extern int hover_r, hover_c;


// ---- Fonctions utilitaires testables ----
static inline gboolean in_bounds(int r, int c) { return r >= 0 && r < N && c >= 0 && c < N; }
static inline gboolean is_blue_piece(Piece p) { return p == BLUE_SOLDIER || p == BLUE_KING; }
static inline gboolean is_red_piece(Piece p) { return p == RED_SOLDIER || p == RED_KING; }
static inline gboolean is_ally(Piece p, Player pl){ return (pl==BLUE)?is_blue_piece(p):is_red_piece(p); }
static inline gboolean is_enemy(Piece p, Player pl){ return (pl==BLUE)?is_red_piece(p):is_blue_piece(p); }
static inline gboolean is_king(Piece p){ return p == BLUE_KING || p == RED_KING; }

// ---- Fonctions exportées ----
void new_game_setup(void);
gboolean try_move(int r0, int c0, int r1, int c1);
gboolean is_game_over(void);
#ifndef NO_GUI
void check_end_and_update(GtkWidget *status_label, GtkWidget *drawing_area);
void update_labels(GtkWidget *blue_score_label, GtkWidget *blue_turn_label,
                   GtkWidget *red_score_label, GtkWidget *red_turn_label);
#endif
gboolean is_city(int r, int c);





#endif
