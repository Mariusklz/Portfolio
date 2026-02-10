/**
 * @file ai.c
 * @brief Module Intelligence Artificielle (IA) pour le jeu.
 *
 * Implémente l’évaluation heuristique, l’algorithme Minimax avec élagage alpha-bêta,
 * la génération de coups, et la logique de capture simulée.
 *
 * Code réalisé par Hermés Noumbog, Nour Karbil et Zyad Idelkaid
 *
 * @date 2025-09-18
 */

// ai.c
#include "ai.h"
#include "network.h"


// ----------------------------------------------------
// Externs (depuis app.c / autres fichiers)
// ----------------------------------------------------
extern Piece board[N][N];
extern Control control_grid[N][N];
extern Player current_player;
extern int turn_count;
extern int captured_blue_soldiers;
extern int captured_red_soldiers;
extern gboolean blue_king_alive;
extern gboolean red_king_alive;
extern gboolean is_city(int r, int c);
extern void compute_possible_moves(int r0, int c0); // restant pour l'UI, NE PAS utiliser dans l'IA
extern gboolean try_move(int r0, int c0, int r1, int c1);
extern gboolean highlight_moves[N][N];
gboolean is_game_over();

// ----------------------------------------------------
// Configuration IA
// ----------------------------------------------------
#define MAX_MOVES 100  // Réduit à 100 pour éviter stack overflow
#define AI_DEFAULT_DEPTH 3  // Réduit à 3 pour éviter stack overflow
#define AI_MAX_DEPTH 4      // Réduit à 4 pour éviter stack overflow

// Debugging désactivé pour une expérience utilisateur propre
#define DEBUG_LEVEL 0 // 0 = off, 1 = info, 2 = detailed
#define DEBUG_PRINT(level, fmt, ...) \
    do { if (DEBUG_LEVEL >= level) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)

#define AI_INF 100000000

// Macros utilitaires
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

// ----------------------------------------------------
// Types locaux
// ----------------------------------------------------
/*typedef struct { int r; int c; } Position;
typedef struct { int r0, c0, r1, c1; } Move;*/
typedef struct { Move m; int score; } MoveScore;

// ----------------------------------------------------
// Variables IA
// ----------------------------------------------------
Position ai_pieces_jouables[20];
Position ai_posi_jouables[N * N];
gboolean ai_enabled = FALSE;
Player ai_player;

// ----------------------------------------------------
// Déclarations statiques
// ----------------------------------------------------
void ai_init(void);

/** @brief Évalue heuristiquement le plateau pour un joueur. */
static int evaluate_board(Piece current_board[N][N], Player player);
/** @brief Algorithme Minimax avec élagage alpha-bêta. */
static int minimax_ab(Piece current_board[N][N], int depth, gboolean is_maximizing_player, int alpha, int beta);
/** @brief Calcule le meilleur coup pour le joueur IA. */
static Move get_best_move(int depth);
/** @brief Génère tous les coups possibles pour un joueur. */
static int ai_get_all_possible_moves(Piece board_state[N][N], Player player, Move *moves);
/** @brief Compare deux coups (MoveScore) pour un tri décroissant. */
static int compare_move_score_desc(const void *a, const void *b);
/** @brief Vérifie si un roi est menacé par un joueur. */
static gboolean is_king_threatened_by(Piece board_state[N][N], int king_r, int king_c, Player threat_player);

/** @brief Vérifie que le chemin est libre entre deux cases. */
static gboolean path_clear_for_ai(Piece board_state[N][N], int r0, int c0, int r1, int c1);
/** @brief Simule un mouvement sur un plateau donné. */
static gboolean try_move_for_ai(Piece board_state[N][N], int r0, int c0, int r1, int c1, Player player);
/** @brief Applique la règle Linca sur un plateau simulé. */
static void try_linca_for_ai(Piece board_state[N][N], int r, int c, Player pl);
/** @brief Applique la règle Seultou sur un plateau simulé. */
static void try_seultou_for_ai(Piece board_state[N][N], int dest_r, int dest_c, int dr, int dc, Player mover);
/** @brief Vérifie et applique les captures après un mouvement. */
static int check_captures_for_ai(Piece board_state[N][N], int r0, int c0, int r1, int c1, Player mover);
/** @brief Vérifie si la partie est terminée sur un plateau simulé. */
static gboolean is_game_over_for_ai(Piece board_state[N][N]);

// ----------------------------------------------------
// Initialisation IA (appeler une seule fois au lancement)
// ----------------------------------------------------
/**
 * @brief Initialise le module IA.
 * @details À appeler une seule fois au lancement.
 */
void ai_init(void) {
    srand((unsigned)time(NULL));
    // IA initialisée silencieusement
}

// ----------------------------------------------------
// Détection de fin de partie pour un plateau simulé
// ----------------------------------------------------
/** @brief Vérifie si la partie est terminée sur un plateau simulé. */
static gboolean is_game_over_for_ai(Piece board_state[N][N]) {
    gboolean blue_king_present = FALSE;
    gboolean red_king_present = FALSE;

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (board_state[r][c] == BLUE_KING) blue_king_present = TRUE;
            if (board_state[r][c] == RED_KING) red_king_present = TRUE;
        }
    }
    if (!blue_king_present || !red_king_present) return TRUE;

    // Si un joueur n'a aucun coup légal -> terminé (pat/affaire de règle)
    Move moves_tmp[1];
    int mblue = ai_get_all_possible_moves(board_state, BLUE, moves_tmp);
    int mred  = ai_get_all_possible_moves(board_state, RED, moves_tmp);
    if (mblue == 0 || mred == 0) return TRUE;

    return FALSE;
}

// ----------------------------------------------------
// Génération des coups (type tour : orthogonal, sans sauts)
// Utilise UNIQUEMENT board_state simulé
// ----------------------------------------------------
/** @brief Génère tous les coups possibles pour un joueur. */
static int ai_get_all_possible_moves(Piece board_state[N][N], Player player, Move *moves) {
    int count = 0;
    const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            Piece p = board_state[r][c];
            if ((player == BLUE && (p == BLUE_SOLDIER || p == BLUE_KING)) ||
                (player == RED && (p == RED_SOLDIER || p == RED_KING))) {
                for (int d = 0; d < 4; d++) {
                    int dr = dirs[d][0], dc = dirs[d][1];
                    int r1 = r + dr, c1 = c + dc;
                    while (r1 >= 0 && r1 < N && c1 >= 0 && c1 < N) {
                        if (board_state[r1][c1] != EMPTY) break;
                        if (count < MAX_MOVES) {
                            moves[count].r0 = r;
                            moves[count].c0 = c;
                            moves[count].r1 = r1;
                            moves[count].c1 = c1;
                        }
                        count++;
                        r1 += dr; c1 += dc;
                        if (count >= MAX_MOVES) return count;
                    }
                }
            }
        }
    }
    // Si count > MAX_MOVES, on retourne MAX_MOVES pour limiter la boucle appelante
    return MIN(count, MAX_MOVES);
}

// ----------------------------------------------------
// Vérifie si le chemin entre deux cases est libre (orthogonal uniquement)
// ----------------------------------------------------
/** @brief Vérifie que le chemin est libre entre deux cases. */
static gboolean path_clear_for_ai(Piece board_state[N][N], int r0, int c0, int r1, int c1) {
    if (r0 != r1 && c0 != c1) return FALSE;
    int dr = (r1 > r0) ? 1 : (r1 < r0 ? -1 : 0);
    int dc = (c1 > c0) ? 1 : (c1 < c0 ? -1 : 0);
    int r = r0 + dr, c = c0 + dc;
    while (!(r == r1 && c == c1)) {
        if (board_state[r][c] != EMPTY) return FALSE;
        r += dr; c += dc;
    }
    return TRUE;
}

// ----------------------------------------------------
// Simulation d'un mouvement sur board_state (ne touche pas au global)
// ----------------------------------------------------
/** @brief Simule un mouvement sur un plateau donné. */
static gboolean try_move_for_ai(Piece board_state[N][N], int r0, int c0, int r1, int c1, Player player) {
    (void)player;
    if (r0 < 0 || r0 >= N || c0 < 0 || c0 >= N) return FALSE;
    if (r1 < 0 || r1 >= N || c1 < 0 || c1 >= N) return FALSE;
    Piece p = board_state[r0][c0];
    if (p == EMPTY) return FALSE;
    if (board_state[r1][c1] != EMPTY) return FALSE;
    if (!path_clear_for_ai(board_state, r0, c0, r1, c1)) return FALSE;

    board_state[r1][c1] = p;
    board_state[r0][c0] = EMPTY;
    return TRUE;
}

// ----------------------------------------------------
// Fonctions de capture (modifient board_state)
// ----------------------------------------------------
/** @brief Applique la règle Linca sur un plateau simulé. */
static void try_linca_for_ai(Piece board_state[N][N], int r, int c, Player pl) {
    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (int k = 0; k < 4; k++) {
        int r1 = r + dirs[k][0];
        int c1 = c + dirs[k][1];
        int r2 = r1 + dirs[k][0];
        int c2 = c1 + dirs[k][1];
        if (r1 < 0 || r1 >= N || c1 < 0 || c1 >= N) continue;
        if (r2 < 0 || r2 >= N || c2 < 0 || c2 >= N) continue;
        Piece p1 = board_state[r1][c1];
        if ((pl == BLUE && (p1 == RED_SOLDIER || p1 == RED_KING)) ||
            (pl == RED && (p1 == BLUE_SOLDIER || p1 == BLUE_KING))) {
            Piece p2 = board_state[r2][c2];
            if ((pl == BLUE && (p2 == BLUE_SOLDIER || p2 == BLUE_KING)) ||
                (pl == RED && (p2 == RED_SOLDIER || p2 == RED_KING))) {
                board_state[r1][c1] = EMPTY;
            }
        }
    }
}

/** @brief Applique la règle Seultou sur un plateau simulé. */
static void try_seultou_for_ai(Piece board_state[N][N], int dest_r, int dest_c, int dr, int dc, Player mover) {
    if (dr == 0 && dc == 0) return;
    int r1 = dest_r + dr, c1 = dest_c + dc;
    if (r1 < 0 || r1 >= N || c1 < 0 || c1 >= N) return;
    Piece p1 = board_state[r1][c1];
    if ((mover == BLUE && (p1 == RED_SOLDIER || p1 == RED_KING)) ||
        (mover == RED && (p1 == BLUE_SOLDIER || p1 == BLUE_KING))) {
        int r2 = r1 + dr, c2 = c1 + dc;
        gboolean supported = FALSE;
        if (r2 >= 0 && r2 < N && c2 >= 0 && c2 < N) {
            Piece behind = board_state[r2][c2];
            supported = (mover == BLUE && (behind == RED_SOLDIER || behind == RED_KING)) ||
                        (mover == RED && (behind == BLUE_SOLDIER || behind == BLUE_KING));
        }
        if (!supported) {
            board_state[r1][c1] = EMPTY;
        }
    }
}

// ----------------------------------------------------
// Applique les règles de capture après un mouvement et renvoie le nombre de captures
// ----------------------------------------------------
/** @brief Vérifie et applique les captures après un mouvement. */
static int check_captures_for_ai(Piece board_state[N][N], int r0, int c0, int r1, int c1, Player mover) {
    int captures = 0;
    Piece before[N][N];
    memcpy(before, board_state, sizeof(Piece) * N * N);

    // Appliquer les règles de capture sur la position destination
    try_linca_for_ai(board_state, r1, c1, mover);
    int dr = (r1 > r0) ? 1 : (r1 < r0 ? -1 : 0);
    int dc = (c1 > c0) ? 1 : (c1 < c0 ? -1 : 0);
    try_seultou_for_ai(board_state, r1, c1, dr, dc, mover);

    // Compter les différences (captures)
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (before[r][c] != board_state[r][c] && before[r][c] != EMPTY) {
                captures++;
            }
        }
    }
    return captures;
}

// ----------------------------------------------------
// Vérifie si un roi est menacé par "threat_player" sur board_state
// ----------------------------------------------------
/** @brief Vérifie si un roi est menacé par un joueur. */
static gboolean is_king_threatened_by(Piece board_state[N][N], int king_r, int king_c, Player threat_player) {
    // Vérification type LINCA (encadrement)
    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (int k = 0; k < 4; k++) {
        int r1 = king_r + dirs[k][0], c1 = king_c + dirs[k][1];
        int r2 = r1 + dirs[k][0], c2 = c1 + dirs[k][1];
        if (r1 < 0 || r1 >= N || c1 < 0 || c1 >= N) continue;
        if (r2 < 0 || r2 >= N || c2 < 0 || c2 >= N) continue;
        Piece p1 = board_state[r1][c1], p2 = board_state[r2][c2];
        if ((threat_player == BLUE && p1 == BLUE_SOLDIER && p2 == BLUE_SOLDIER) ||
            (threat_player == RED && p1 == RED_SOLDIER && p2 == RED_SOLDIER)) {
            return TRUE;
        }
    }

    // Vérification SEULTOU (un pion peut se placer et capturer)
    // On vérifie si un pion adverse peut se déplacer sur la case du roi (chemin clear)
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            Piece p = board_state[r][c];
            if ((threat_player == RED && (p == RED_SOLDIER || p == RED_KING)) ||
                (threat_player == BLUE && (p == BLUE_SOLDIER || p == BLUE_KING))) {
                if (path_clear_for_ai(board_state, r, c, king_r, king_c)) {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

// ----------------------------------------------------
// Heuristique d'évaluation
// - valeur des pièces
// - progression des pions (vers l'adversaire)
// - mobilité
// - sécurité des rois (distance des villes / centre)
// - petit bruit aléatoire pour casser les égalités
// ----------------------------------------------------
/** @brief Évalue heuristiquement le plateau pour un joueur. */
static int evaluate_board(Piece current_board[N][N], Player player) {
    int score = 0;
    int red_soldiers = 0, blue_soldiers = 0;
    int red_king_r = -1, red_king_c = -1;
    int blue_king_r = -1, blue_king_c = -1;

    Player opponent = (player == BLUE ? RED : BLUE);

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            Piece p = current_board[r][c];
            if (p == RED_SOLDIER) red_soldiers++;
            if (p == BLUE_SOLDIER) blue_soldiers++;
            if (p == RED_KING) { red_king_r = r; red_king_c = c; }
            if (p == BLUE_KING) { blue_king_r = r; blue_king_c = c; }
        }
    }

    // Conditions terminales fortes
    if (blue_king_r == -1) return (player == RED ? AI_INF : -AI_INF);
    if (red_king_r == -1)  return (player == BLUE ? AI_INF : -AI_INF);

    // Différence de matériel (poids de base)
    int material = ( (player == RED) ? (red_soldiers - blue_soldiers) : (blue_soldiers - red_soldiers) );
    score += material * 100;

    // Progression des pions (pousser vers l'adversaire)
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            Piece p = current_board[r][c];
            if (p == RED_SOLDIER) {
                score += (N - 1 - r) * 5 * (player == RED ? 1 : -1);
            }
            if (p == BLUE_SOLDIER) {
                score += r * 5 * (player == BLUE ? 1 : -1);
            }
        }
    }

    // Mobilité : nombre de coups
    Move tmp_moves[MAX_MOVES];
    int ai_moves = ai_get_all_possible_moves(current_board, player, tmp_moves);
    int opp_moves = ai_get_all_possible_moves(current_board, opponent, tmp_moves);
    score += (ai_moves - opp_moves) * 10;

    // Bonus pour menacer le roi adverse
    if (player == RED && blue_king_r != -1) {
        if (is_king_threatened_by(current_board, blue_king_r, blue_king_c, RED)) score += 400;
    }
    if (player == BLUE && red_king_r != -1) {
        if (is_king_threatened_by(current_board, red_king_r, red_king_c, BLUE)) score += 400;
    }

    // Sécurité des rois : plus proche des bords/coin = souvent mieux/proche de ville
    if (blue_king_r != -1) {
        int dist_blue_corners = MIN(abs(blue_king_r - 0) + abs(blue_king_c - 0),
                                    abs(blue_king_r - (N-1)) + abs(blue_king_c - (N-1)));
        score += (player == BLUE ? (50 - dist_blue_corners * 3) : -(50 - dist_blue_corners * 3));
    }
    if (red_king_r != -1) {
        int dist_red_corners = MIN(abs(red_king_r - 0) + abs(red_king_c - 0),
                                   abs(red_king_r - (N-1)) + abs(red_king_c - (N-1)));
        score += (player == RED ? (50 - dist_red_corners * 3) : -(50 - dist_red_corners * 3));
    }

    // Petit bruit aléatoire pour briser les égalités (−3..+3)
    int noise = (rand() % 7) - 3;
    score += noise;

    return score;
}

// ----------------------------------------------------
// Compaison pour qsort (MoveScore descendant)
// ----------------------------------------------------
/** @brief Compare deux coups (MoveScore) pour un tri décroissant. */
static int compare_move_score_desc(const void *a, const void *b) {
    const MoveScore *A = a;
    const MoveScore *B = b;
    return B->score - A->score;
}

// ----------------------------------------------------
// Minimax avec élagage alpha-beta, utilisant la génération basée sur board_state
// ----------------------------------------------------
/** @brief Algorithme Minimax avec élagage alpha-bêta. */
static int minimax_ab(Piece current_board[N][N], int depth, gboolean is_maximizing_player, int alpha, int beta) {
    if (depth == 0 || is_game_over_for_ai(current_board)) {
        return evaluate_board(current_board, ai_player);
    }

    Player current_turn = is_maximizing_player ? ai_player : (ai_player == BLUE ? RED : BLUE);

    Move moves[MAX_MOVES];
    int num_moves = ai_get_all_possible_moves(current_board, current_turn, moves);
    if (num_moves == 0) {
        // Aucun coup : évaluer position comme terminée
        return evaluate_board(current_board, ai_player);
    }

    // Générer des MoveScore pour trier (captures + menace de roi)
    MoveScore ms[MAX_MOVES];
    int ms_count = MIN(num_moves, MAX_MOVES);
    for (int i = 0; i < ms_count; i++) {
        ms[i].m = moves[i];

        // Simuler coup pour estimer captures et menace
        Piece tmp[N][N];
        memcpy(tmp, current_board, sizeof(Piece) * N * N);
        try_move_for_ai(tmp, moves[i].r0, moves[i].c0, moves[i].r1, moves[i].c1, current_turn);
        int cap = check_captures_for_ai(tmp, moves[i].r0, moves[i].c0, moves[i].r1, moves[i].c1, current_turn);

        // Détecter le roi adverse et si menacé
        Player opp = (current_turn == BLUE ? RED : BLUE);
        int king_r = -1, king_c = -1;
        for (int rr = 0; rr < N && king_r == -1; rr++) for (int cc = 0; cc < N; cc++) {
            if ((opp == BLUE && tmp[rr][cc] == BLUE_KING) || (opp == RED && tmp[rr][cc] == RED_KING)) {
                king_r = rr; king_c = cc; break;
            }
        }
        int threat_bonus = 0;
        if (king_r != -1 && is_king_threatened_by(tmp, king_r, king_c, current_turn)) threat_bonus += 150;

        ms[i].score = cap * 1000 + threat_bonus;
    }
    qsort(ms, ms_count, sizeof(MoveScore), compare_move_score_desc);

    if (is_maximizing_player) {
        int max_eval = -AI_INF;
        for (int i = 0; i < ms_count; i++) {
            Piece next_board[N][N];
            memcpy(next_board, current_board, sizeof(Piece) * N * N);
            try_move_for_ai(next_board, ms[i].m.r0, ms[i].m.c0, ms[i].m.r1, ms[i].m.c1, current_turn);
            check_captures_for_ai(next_board, ms[i].m.r0, ms[i].m.c0, ms[i].m.r1, ms[i].m.c1, current_turn);

            int eval = minimax_ab(next_board, depth - 1, FALSE, alpha, beta);
            if (eval > max_eval) max_eval = eval;
            if (eval > alpha) alpha = eval;
            if (beta <= alpha) break; // élagage
        }
        return max_eval;
    } else {
        int min_eval = AI_INF;
        for (int i = 0; i < ms_count; i++) {
            Piece next_board[N][N];
            memcpy(next_board, current_board, sizeof(Piece) * N * N);
            try_move_for_ai(next_board, ms[i].m.r0, ms[i].m.c0, ms[i].m.r1, ms[i].m.c1, current_turn);
            check_captures_for_ai(next_board, ms[i].m.r0, ms[i].m.c0, ms[i].m.r1, ms[i].m.c1, current_turn);

            int eval = minimax_ab(next_board, depth - 1, TRUE, alpha, beta);
            if (eval < min_eval) min_eval = eval;
            if (eval < beta) beta = eval;
            if (beta <= alpha) break; // élagage
        }
        return min_eval;
    }
}

// ----------------------------------------------------
// Trouver le meilleur coup à partir du plateau GLOBAL 'board'
// ----------------------------------------------------
/** @brief Calcule le meilleur coup pour le joueur IA. */
static Move get_best_move(int depth) {
    Move best_move = {-1, -1, -1, -1};
    int best_eval = -AI_INF;
    int alpha = -AI_INF;
    int beta = AI_INF;

    Move moves[MAX_MOVES];
    int num_moves = ai_get_all_possible_moves(board, ai_player, moves);
    if (num_moves == 0) return best_move;

    // Move ordering sur la position courante
    MoveScore ms[MAX_MOVES];
    int ms_count = MIN(num_moves, MAX_MOVES);
    for (int i = 0; i < ms_count; i++) {
        ms[i].m = moves[i];
        Piece tmp[N][N];
        memcpy(tmp, board, sizeof(Piece) * N * N);
        try_move_for_ai(tmp, moves[i].r0, moves[i].c0, moves[i].r1, moves[i].c1, ai_player);
        int cap = check_captures_for_ai(tmp, moves[i].r0, moves[i].c0, moves[i].r1, moves[i].c1, ai_player);
        // menace du roi adverse
        Player opp = (ai_player == BLUE ? RED : BLUE);
        int king_r = -1, king_c = -1;
        for (int rr = 0; rr < N && king_r == -1; rr++) for (int cc = 0; cc < N; cc++) {
            if ((opp == BLUE && tmp[rr][cc] == BLUE_KING) || (opp == RED && tmp[rr][cc] == RED_KING)) {
                king_r = rr; king_c = cc; break;
            }
        }
        int threat_bonus = 0;
        if (king_r != -1 && is_king_threatened_by(tmp, king_r, king_c, ai_player)) threat_bonus += 150;
        ms[i].score = cap * 1000 + threat_bonus;
    }
    qsort(ms, ms_count, sizeof(MoveScore), compare_move_score_desc);

    for (int i = 0; i < ms_count; i++) {
        Piece next_board[N][N];
        memcpy(next_board, board, sizeof(Piece) * N * N);
        try_move_for_ai(next_board, ms[i].m.r0, ms[i].m.c0, ms[i].m.r1, ms[i].m.c1, ai_player);
        check_captures_for_ai(next_board, ms[i].m.r0, ms[i].m.c0, ms[i].m.r1, ms[i].m.c1, ai_player);

        int eval = minimax_ab(next_board, depth - 1, FALSE, alpha, beta);

        if (eval > best_eval) {
            best_eval = eval;
            best_move = ms[i].m;
        }
        if (eval > alpha) alpha = eval;
        if (beta <= alpha) break;
    }

    // Si pas trouvé (cas improbable), fallback sur le premier coup
    if (best_move.r0 == -1 && num_moves > 0) {
        best_move = moves[0];
    }
    return best_move;
}

// ----------------------------------------------------
// Coup aléatoire plus "safe" (évite de perdre le roi immédiatement si possible)
// ----------------------------------------------------
/** @brief Joue un coup aléatoire « safe » si aucun autre n’est possible. */
static gboolean ai_random_move(void) {
    int nombreDepiece = 0;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            Piece p = board[r][c];
            if ((ai_player == RED && (p == RED_SOLDIER || p == RED_KING)) ||
                (ai_player == BLUE && (p == BLUE_SOLDIER || p == BLUE_KING))) {
                ai_pieces_jouables[nombreDepiece].r = r;
                ai_pieces_jouables[nombreDepiece].c = c;
                nombreDepiece++;
            }
        }
    }
    if (nombreDepiece == 0) return FALSE;

    // Essayer quelques pièces aléatoires pour trouver un move légal
    int tries = MIN(nombreDepiece, 6);
    for (int t = 0; t < tries; t++) {
        int idx = rand() % nombreDepiece;
        Position p = ai_pieces_jouables[idx];

        // Générer moves pour cette pièce (localement) : utiliser ai_get_all_possible_moves sur une copie
        Move local_moves[N*N];
        Piece tmp_board[N][N];
        memcpy(tmp_board, board, sizeof(Piece) * N * N);

        // On construit moves manuellement pour cette pièce
        const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        int nm = 0;
        for (int d = 0; d < 4; d++) {
            int r1 = p.r + dirs[d][0], c1 = p.c + dirs[d][1];
            while (r1 >= 0 && r1 < N && c1 >= 0 && c1 < N && board[r1][c1] == EMPTY) {
                local_moves[nm].r0 = p.r; local_moves[nm].c0 = p.c;
                local_moves[nm].r1 = r1; local_moves[nm].c1 = c1;
                nm++; r1 += dirs[d][0]; c1 += dirs[d][1];
                if (nm >= N*N) break;
            }
        }
        if (nm == 0) continue;

        int choice = rand() % nm;
        Move chosen = local_moves[choice];

        // Vérifier try_move real via try_move (global) pour appliquer règles totales
        if (try_move(chosen.r0, chosen.c0, chosen.r1, chosen.c1)) {
            network_send_move_async(chosen.r0, chosen.c0, chosen.r1, chosen.c1);
            current_player = (current_player == BLUE) ? RED : BLUE;
            return TRUE;
        }
    }
    return FALSE;
}

// ----------------------------------------------------
// Fonction publique : lancer un coup IA (appelée par app.c)
// ----------------------------------------------------
/**
 * @brief Joue un coup IA selon la profondeur adaptative.
 * @return TRUE si un coup a été joué, FALSE sinon.
 */
gboolean ai_move() {
    if (!ai_enabled) {
        return FALSE;
    }
    if (current_player != ai_player) {
        return FALSE;
    }

    // Profondeur adaptative selon le branching
    Move tmp_moves[MAX_MOVES];
    int num_moves = ai_get_all_possible_moves(board, ai_player, tmp_moves);
    int depth = AI_DEFAULT_DEPTH;  // Utiliser seulement la profondeur par défaut pour éviter stack overflow
    // Logique adaptative désactivée temporairement pour éviter crash
    // if (num_moves < 6) depth = MIN(AI_MAX_DEPTH, AI_DEFAULT_DEPTH + 2);
    // else if (num_moves < 12) depth = AI_DEFAULT_DEPTH + 1;
    // else depth = AI_DEFAULT_DEPTH;

    Move best_move = get_best_move(depth);

    if (best_move.r0 != -1) {
        // Coup choisi, affichage pour l'utilisateur
        printf("[IA] Coup joué : (%d,%d) -> (%d,%d)\n",
               best_move.r0, best_move.c0, best_move.r1, best_move.c1);
        // Appliquer via try_move global pour respecter toutes les règles UI/réseau
        if (try_move(best_move.r0, best_move.c0, best_move.r1, best_move.c1)) {
            network_send_move_async(best_move.r0, best_move.c0, best_move.r1, best_move.c1);
            current_player = (current_player == BLUE) ? RED : BLUE;
            return TRUE;
        } else {
            printf("[IA] Erreur lors de l'exécution du coup, tentative de coup alternatif...\n");
        }
    }

    // Fallback : coup aléatoire "safe"
    if (ai_random_move()) return TRUE;

    return FALSE;
}

// ----------------------------------------------------
// Fonctions d'activation / configuration simples
// ----------------------------------------------------
/**
 * @brief Active ou désactive l’IA.
 * @param enabled TRUE pour activer, FALSE pour désactiver.
 */
void ai_set_enabled(gboolean enabled) { ai_enabled = enabled; }
/**
 * @brief Définit le joueur contrôlé par l’IA.
 * @param p Joueur (BLUE ou RED).
 */
void ai_set_player(Player p) { ai_player = p; }
