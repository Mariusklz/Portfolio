/**
 * @file app.c
 * @brief Logique de jeu et intégration UI (GTK4) pour un plateau 9x9.
 *
 * Implémente l'état du jeu, les règles de déplacement/capture, le calcul des scores,
 * et la mise à jour des labels GTK.
 *
 * Code réalisé par Grégory Runser  et Guillaume Schossig
 *
 * @date 2025-09-16
 */


#include <string.h>
#include <stdio.h>
#include "app.h"

/**
 * @defgroup GameState État global du jeu
 * @brief Variables globales représentant le plateau, les contrôles, le tour et les compteurs.
 * @{
 */
// Variables globales
Piece board[N][N];  /**< Plateau de jeu 9x9 : type de pièce par case. */
Control control_grid[N][N];  /**< Contrôle de chaque case (Bleu/Rouge/Neutre), cités exclues. */
Player current_player = BLUE;  /**< Joueur courant. */
Player local_player = BLUE;   /**< Joueur local (BLUE pour client, RED pour serveur, BLUE par défaut en local). */
int turn_count = 1;  /**< Compteur de tour (1..MAX_TURNS). */

int captured_blue_soldiers = 0;  /**< Nombre de soldats bleus capturés. */
int captured_red_soldiers = 0;  /**< Nombre de soldats rouges capturés. */
gboolean blue_king_alive = TRUE;  /**< Indique si le roi bleu est vivant. */
gboolean red_king_alive = TRUE;  /**< Indique si le roi rouge est vivant. */

gboolean has_selection = FALSE;  /**< Une case est-elle actuellement sélectionnée ? */
int sel_r = -1, sel_c = -1;  /**< Coordonnées de la sélection courante. */
int hover_r = -1, hover_c = -1;  /**< Coordonnées de survol (hover). */

/** @} */

// Coups possibles
gboolean highlight_moves[N][N];  /**< Masque de cases surlignées pour les coups possibles. */

// Externe : label central
#ifndef NO_GUI
extern GtkWidget *turn_center_label;  /**< Label central "Au tour de..." (fourni par l'UI). */
#endif

// --------- Fonctions utilitaires ---------
/**
 * @brief Indique si la case (r,c) est une cité (coins A9 et I1).
 * @param r Ligne.
 * @param c Colonne.
 * @return TRUE si (r,c) correspond à une cité.
 * @note Exportée pour gui.c.
 */
gboolean is_city(int r, int c) { return (r == 0 && c == 0) || (r == N-1 && c == N-1); } // Export pour gui.c

// --------- Calcul des coups possibles ---------
/**
 * @brief Calcule les cases atteignables depuis (r0,c0) en ligne/colonne.
 * @param r0 Ligne de départ.
 * @param c0 Colonne de départ.
 * @details Parcourt les 4 directions jusqu'à rencontrer une pièce/limite et
 *          renseigne @ref highlight_moves.
 */
void compute_possible_moves(int r0, int c0) {
    memset(highlight_moves, 0, sizeof(highlight_moves));

    Piece p = board[r0][c0];
    if (p == EMPTY || !is_ally(p, current_player)) return;

    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (int k=0; k<4; k++) {
        int r = r0 + dirs[k][0];
        int c = c0 + dirs[k][1];
        while (in_bounds(r,c) && board[r][c] == EMPTY) {
            highlight_moves[r][c] = TRUE;
            r += dirs[k][0];
            c += dirs[k][1];
        }
    }
}

// --------- Comptages dynamiques ---------
/**
 * @brief Calcule les scores "vivants" (contrôle & soldats).
 * @param blue_ctrl [out] Nombre de cases contrôlées par Bleu (hors cités).
 * @param red_ctrl  [out] Nombre de cases contrôlées par Rouge (hors cités).
 * @param blue_soldiers [out] Soldats bleus restants.
 * @param red_soldiers  [out] Soldats rouges restants.
 */
static void compute_live_scores(int *blue_ctrl, int *red_ctrl, int *blue_soldiers, int *red_soldiers){
    int bc=0, rc=0, bs=0, rs=0;
    for(int r=0;r<N;r++){
        for(int c=0;c<N;c++){
            if(!is_city(r,c)){
                if(control_grid[r][c]==CTRL_BLUE) bc++;
                else if(control_grid[r][c]==CTRL_RED) rc++;
            }
            if(board[r][c]==BLUE_SOLDIER) bs++;
            else if(board[r][c]==RED_SOLDIER) rs++;
        }
    }
    if(blue_ctrl) *blue_ctrl = bc;
    if(red_ctrl) *red_ctrl = rc;
    if(blue_soldiers) *blue_soldiers = bs;
    if(red_soldiers) *red_soldiers = rs;
}

/**
 * @brief Met à jour les labels GTK (scores, tour, centre) et classes CSS.
 * @param blue_score_label Label score Bleu.
 * @param blue_turn_label  Label tour Bleu.
 * @param red_score_label  Label score Rouge.
 * @param red_turn_label   Label tour Rouge.
 */
#ifndef NO_GUI
void update_labels(GtkWidget *blue_score_label, GtkWidget *blue_turn_label, GtkWidget *red_score_label, GtkWidget *red_turn_label){
    int bc, rc, bs, rs;
    compute_live_scores(&bc, &rc, &bs, &rs);

    char buf[512];

    // Score Bleu
    snprintf(buf, sizeof(buf),
        "<span size='18000' weight='bold' foreground='#1E90FF'>Contrôle: %d | Soldats: %d</span>", bc, bs);
    gtk_label_set_markup(GTK_LABEL(blue_score_label), buf);

    // Score Rouge
    snprintf(buf, sizeof(buf),
        "<span size='18000' weight='bold' foreground='#FF3030'>Contrôle: %d | Soldats: %d</span>", rc, rs);
    gtk_label_set_markup(GTK_LABEL(red_score_label), buf);

    // Label central "Au tour de ..."
    snprintf(buf, sizeof(buf),
        "<span size='22000' weight='bold'>%s</span>",
        current_player == BLUE ? "Tour des Bleus" : "Tour des Rouges");
    gtk_label_set_markup(GTK_LABEL(turn_center_label), buf);
    
    // Changer la classe CSS selon le joueur actuel
    gtk_widget_remove_css_class(turn_center_label, "turn-blue");
    gtk_widget_remove_css_class(turn_center_label, "turn-red");
    if (current_player == BLUE) {
        gtk_widget_add_css_class(turn_center_label, "turn-blue");
    } else {
        gtk_widget_add_css_class(turn_center_label, "turn-red");
    }

    // Tour Bleu
    snprintf(buf, sizeof(buf),
        "<span size='16000' foreground='#1E90FF'>Tour %d / %d</span>", turn_count, MAX_TURNS);
    gtk_label_set_markup(GTK_LABEL(blue_turn_label), buf);

    // Tour Rouge
    snprintf(buf, sizeof(buf),
        "<span size='16000' foreground='#FF3030'>Tour %d / %d</span>", turn_count, MAX_TURNS);
    gtk_label_set_markup(GTK_LABEL(red_turn_label), buf);
}
#endif

// --------- Placement initial ---------
/**
 * @brief Initialise une nouvelle partie : plateau, contrôle, compteurs et joueur.
 * @details Place les rois, soldats et contrôle initial (cités incluses),
 *          réinitialise les masques et variables d'état.
 */
void new_game_setup(){
    memset(board, 0, sizeof(board));
    memset(control_grid, 0, sizeof(control_grid));
    memset(highlight_moves, 0, sizeof(highlight_moves));

    // Contrôle des cités (cases spéciales vides)
    control_grid[0][0] = CTRL_BLUE; // A9 contrôlée par les bleus
    control_grid[8][8] = CTRL_RED;  // I1 contrôlée par les rouges

    // BLEU
    board[1][1] = BLUE_KING;
    board[0][2] = BLUE_SOLDIER;
    board[0][3] = BLUE_SOLDIER;
    board[1][2] = BLUE_SOLDIER;
    board[1][3] = BLUE_SOLDIER;
    board[2][0] = BLUE_SOLDIER;
    board[2][1] = BLUE_SOLDIER;
    board[2][2] = BLUE_SOLDIER;
    board[3][0] = BLUE_SOLDIER;
    board[3][1] = BLUE_SOLDIER;

    // ROUGE
    board[7][7] = RED_KING;
    board[5][7] = RED_SOLDIER;
    board[5][8] = RED_SOLDIER;
    board[6][6] = RED_SOLDIER;
    board[6][7] = RED_SOLDIER;
    board[6][8] = RED_SOLDIER;
    board[7][5] = RED_SOLDIER;
    board[7][6] = RED_SOLDIER;
    board[8][5] = RED_SOLDIER;
    board[8][6] = RED_SOLDIER;

    for(int r=0;r<N;r++){
        for(int c=0;c<N;c++){
            if(!is_city(r,c)){ // Protéger les cités
                if(board[r][c]==BLUE_SOLDIER || board[r][c]==BLUE_KING)
                    control_grid[r][c] = CTRL_BLUE;
                else if(board[r][c]==RED_SOLDIER || board[r][c]==RED_KING)
                    control_grid[r][c] = CTRL_RED;
            }
        }
    }

    current_player = BLUE;
    turn_count = 1;
    captured_blue_soldiers = 0;
    captured_red_soldiers = 0;
    blue_king_alive = TRUE;
    red_king_alive = TRUE;
    has_selection = FALSE; sel_r = sel_c = -1;
    hover_r = hover_c = -1;
}



// (les fonctions try_move, captures, popup de fin, etc. restent inchangées mais continuent d’appeler update_labels)

// --------- Gestion captures ---------
/**
 * @brief Vérifie que le chemin rectiligne entre deux cases est libre.
 * @param r0,c0 Case source.
 * @param r1,c1 Case destination.
 * @return TRUE si aucune pièce n'obstrue la trajectoire.
 */
static gboolean path_clear(int r0,int c0,int r1,int c1){
    if(r0!=r1 && c0!=c1) return FALSE;
    int dr = (r1>r0) ? 1 : (r1<r0 ? -1 : 0);
    int dc = (c1>c0) ? 1 : (c1<c0 ? -1 : 0);
    int r = r0 + dr, c = c0 + dc;
    while(!(r==r1 && c==c1)){
        if(board[r][c]!=EMPTY) return FALSE;
        r += dr; c += dc;
    }
    return TRUE;
}

/**
 * @brief Retire une pièce du plateau et met à jour les compteurs/stats.
 * @param r Ligne.
 * @param c Colonne.
 * @note Met également à jour @ref control_grid pour la case, sauf cités.
 */
static void remove_piece(int r,int c){
    if(!in_bounds(r,c)) return;
    Piece p = board[r][c];
    if(p==EMPTY) return;

    if(p==BLUE_SOLDIER) captured_blue_soldiers++;
    else if(p==RED_SOLDIER) captured_red_soldiers++;
    else if(p==BLUE_KING) blue_king_alive = FALSE;
    else if(p==RED_KING) red_king_alive = FALSE;

    board[r][c] = EMPTY;
    if(!is_city(r,c)) control_grid[r][c] = CTRL_NEUTRAL;
}

/**
 * @brief Applique la règle "Linca" autour de (r,c) après un déplacement.
 * @param r Ligne de la pièce qui vient de bouger.
 * @param c Colonne de la pièce qui vient de bouger.
 * @param pl Joueur qui a effectué le déplacement.
 * @details Capture un ennemi adjacent si celui-ci est encadré (allié-ennemi-allié).
 */
static void try_linca_from(int r,int c, Player pl){
    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for(int k=0;k<4;k++){
        int r1 = r + dirs[k][0];
        int c1 = c + dirs[k][1];
        int r2 = r1 + dirs[k][0];
        int c2 = c1 + dirs[k][1];
        if(!in_bounds(r1,c1)) continue;
        if(!in_bounds(r2,c2)) continue;
        Piece p1 = board[r1][c1];
        if(is_enemy(p1, pl)){
            Piece p2 = board[r2][c2];
            if(is_ally(p2, pl)){
                remove_piece(r1,c1);
            }
        }
    }
}

/**
 * @brief Applique la règle "Seultou" dans la direction du mouvement.
 * @param dest_r Ligne d'arrivée.
 * @param dest_c Colonne d'arrivée.
 * @param dr Delta ligne du déplacement (-1,0,1).
 * @param dc Delta colonne du déplacement (-1,0,1).
 * @param mover Joueur qui se déplace.
 * @details Capture un ennemi adjacent si non "supporté" derrière lui.
 */
static void try_seultou(int dest_r,int dest_c,int dr,int dc, Player mover){
    if(dr==0 && dc==0) return;
    int r1 = dest_r + dr, c1 = dest_c + dc;
    if(!in_bounds(r1,c1)) return;
    Piece p1 = board[r1][c1];
    if(!is_enemy(p1, mover)) return;
    int r2 = r1 + dr, c2 = c1 + dc;
    gboolean supported = FALSE;
    if(in_bounds(r2,c2)){
        Piece behind = board[r2][c2];
        supported = is_ally(behind, (mover==BLUE)?RED:BLUE);
    }
    if(!supported){
        remove_piece(r1,c1);
    }
}

/**
 * @brief Tente d'effectuer un déplacement et d'appliquer les règles de capture.
 * @param r0,c0 Case source.
 * @param r1,c1 Case destination.
 * @return TRUE si le mouvement est valide et a été appliqué.
 * @post Met à jour @ref control_grid, vérifie les rois en cité, et appelle
 *       @ref try_linca_from et @ref try_seultou.
 */
gboolean try_move(int r0,int c0,int r1,int c1){
    if(!in_bounds(r0,c0) || !in_bounds(r1,c1)) return FALSE;
    Piece p = board[r0][c0];
    if(p==EMPTY) return FALSE;
    if(!is_ally(p, current_player)) return FALSE;
    if(board[r1][c1]!=EMPTY) return FALSE;
    if(!path_clear(r0,c0,r1,c1)) return FALSE;

    board[r1][c1] = p;
    board[r0][c0] = EMPTY;

 // --- LOG du déplacement ---
    const char *player_str = (current_player == BLUE) ? "Bleu" : "Rouge";
    const char *piece_str  = (p == BLUE_KING || p == RED_KING) ? "Roi" : "Soldat";
    printf("[LOG] %s déplace %s de (%d,%d) vers (%d,%d)\n",
           player_str, piece_str, r0, c0, r1, c1);
    fflush(stdout);
 // ----------------------------
    if(!is_city(r1,c1)) control_grid[r1][c1] = (current_player==BLUE)?CTRL_BLUE:CTRL_RED;

    if(is_king(p)){
        if(current_player==BLUE && r1==N-1 && c1==N-1){
            red_king_alive = FALSE;
        }
        if(current_player==RED && r1==0 && c1==0){
            blue_king_alive = FALSE;
        }
    }

    try_linca_from(r1,c1,current_player);
    int dr = (r1>r0) ? 1 : (r1<r0 ? -1 : 0);
    int dc = (c1>c0) ? 1 : (c1<c0 ? -1 : 0);
    try_seultou(r1,c1,dr,dc,current_player);

    return TRUE;
}

// --------- Popup fin de partie ---------
/**
 * @brief Affiche un dialogue de fin de partie.
 * @param message Message à présenter dans la boîte de dialogue.
 * @param parent  Fenêtre parente (root).
 */
#ifndef NO_GUI
void show_game_over_popup(const char *message, GtkWidget *parent) {
    // Convertir GtkRoot en GtkWidget si nécessaire
    GtkWidget *window_widget = GTK_WIDGET(parent);
    if (GTK_IS_ROOT(parent)) {
        window_widget = GTK_WIDGET(parent);
    }
    
    // Utiliser GtkAlertDialog pour GTK4 moderne
    GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", message);
    gtk_alert_dialog_set_modal(dialog, TRUE);
    gtk_alert_dialog_set_detail(dialog, "Cliquez OK pour continuer.");
    
    // Afficher le dialogue
    gtk_alert_dialog_show(dialog, GTK_WINDOW(window_widget));
    
    // Libérer la référence
    g_object_unref(dialog);
}
#endif

// --------- Fin de partie ---------


/**
 * @brief Indique si une condition de fin est atteinte.
 * @return TRUE si un roi est mort, 8 soldats capturés d'un camp,
 *         ou si le nombre de tours a dépassé MAX_TURNS.
 */
gboolean is_game_over(){
    return !(red_king_alive && blue_king_alive) || (captured_blue_soldiers >= 8) || 
    (captured_red_soldiers >= 8) || (turn_count > MAX_TURNS) ;
}
/**
 * @brief Vérifie la fin de partie, met à jour l'UI et bloque le plateau si nécessaire.
 * @param status_label Label de statut.
 * @param drawing_area Zone de dessin du plateau.
 */
#ifndef NO_GUI
void check_end_and_update(GtkWidget *status_label, GtkWidget *drawing_area){
    if(!blue_king_alive){
        const char *msg = "Fin : Roi Bleu exécuté ou conquête Rouge — Rouge gagne !";
        gtk_label_set_text(GTK_LABEL(status_label), msg);
        gtk_widget_set_sensitive(drawing_area, FALSE);
        show_game_over_popup(msg, GTK_WIDGET(gtk_widget_get_root(drawing_area)));
        return;
    }

    if(!red_king_alive){
        const char *msg = "Fin : Roi Rouge exécuté ou conquête Bleue — Bleu gagne !";
        gtk_label_set_text(GTK_LABEL(status_label), msg);
        gtk_widget_set_sensitive(drawing_area, FALSE);
        show_game_over_popup(msg, GTK_WIDGET(gtk_widget_get_root(drawing_area)));
        return;
    }

    if(captured_red_soldiers >= 8){
        const char *msg = "Fin : 8 soldats rouges capturés — Bleu gagne !";
        gtk_label_set_text(GTK_LABEL(status_label), msg);
        gtk_widget_set_sensitive(drawing_area, FALSE);
        show_game_over_popup(msg, GTK_WIDGET(gtk_widget_get_root(drawing_area)));
        return;
    }

    if(captured_blue_soldiers >= 8){
        const char *msg = "Fin : 8 soldats bleus capturés — Rouge gagne !";
        gtk_label_set_text(GTK_LABEL(status_label), msg);
        gtk_widget_set_sensitive(drawing_area, FALSE);
        show_game_over_popup(msg, GTK_WIDGET(gtk_widget_get_root(drawing_area)));
        return;
    }

    if(turn_count > MAX_TURNS){
        int bc, rc, bs, rs;
        compute_live_scores(&bc,&rc,&bs,&rs);
        int blue_score = bc + bs;
        int red_score = rc + rs;

        char buf[256];
        if(blue_score > red_score)
            snprintf(buf,sizeof(buf),"Fin (64 tours). Score Bleu %d > Rouge %d — Bleu gagne !", blue_score, red_score);
        else if(red_score > blue_score)
            snprintf(buf,sizeof(buf),"Fin (64 tours). Score Rouge %d > Bleu %d — Rouge gagne !", red_score, blue_score);
        else
            snprintf(buf,sizeof(buf),"Fin (64 tours). Égalité %d - %d.", blue_score, red_score);

        gtk_label_set_text(GTK_LABEL(status_label), buf);
        gtk_widget_set_sensitive(drawing_area, FALSE);
        show_game_over_popup(buf, GTK_WIDGET(gtk_widget_get_root(drawing_area)));
        return;
    }
}
#else
void check_end_and_update(void *status_label, void *drawing_area) {
    // Stub pour les tests : ne fait rien
}
#endif
