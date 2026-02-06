#ifndef AI_H
#define AI_H
#include "app.h"
#include "../includes/header_commun.h"
#include <time.h>

/*-------------- Structures reqcuises pour stocker par l'IA ------------------*/
typedef struct {
    int r, c;
} Position;  //Pour choisir aléatoirement un pion

typedef struct {
    int r0 ;
    int c0 ;
    int r1 ;
    int c1 ;
} Move; //Pour choisir aléatoirement un mouvement de ce pion



/*
------------------------- FONCTIONS EXPORTEES ------------------------
*/
gboolean is_game_over() ;
gboolean try_move(int r0, int c0, int r1, int c1);

extern gboolean ai_enabled  ;
extern Player ai_player  ;



// Fonction principale IA
int ai_option_piece(); // choisi une pièce au hasard
int ai_option_move(); // --||-- un mouvement au hasard
gboolean ai_move();
extern gboolean ai_gameloop() ;

// Fonctions d'initialisation et configuration IA
void ai_init(void);
void ai_set_enabled(gboolean enabled);
void ai_set_player(Player p);



#endif
