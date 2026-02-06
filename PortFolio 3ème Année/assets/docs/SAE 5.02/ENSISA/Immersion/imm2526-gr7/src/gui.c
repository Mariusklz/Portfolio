/**
 * @file gui.c
 * @brief Gestion de l'interface graphique du jeu Krojanty avec GTK4
 *
 * Ce module s'occupe de :
 * - L'affichage du plateau de jeu et des pièces.
 * - La gestion des clics de l'utilisateur.
 * - La synchronisation avec le réseau et l'IA.
 * - La mise à jour des labels de scores et de tours.
 *
 * Code réalisé par Yunus Demirel
 *
 * @date 2025-09-16
 */

#include "gui.h"
#include "ai.h"
#include "network.h"

#define WINDOWS_LONG 800
#define WINDOWS_HEIGHT 500
#define CELL_SIZE 48
#define GRID_SIZE 9
#define LINE_WIDTH 4
#define MARGIN 25

// Widgets globaux
GtkWidget *drawing_area;
GtkWidget *blue_score_label, *blue_turn_label;
GtkWidget *red_score_label, *red_turn_label;
GtkWidget *turn_center_label; // label centré

// Fonctions externes
extern Piece board[N][N];
extern Control control_grid[N][N];
extern Player current_player;
extern Player local_player;  // Joueur local (BLUE pour client, RED pour serveur)
extern int turn_count;
extern gboolean has_selection;
extern int sel_r, sel_c;

extern gboolean try_move(int r0, int c0, int r1, int c1);
extern void new_game_setup();
extern void update_labels(GtkWidget *blue_score_label, GtkWidget *blue_turn_label,
                   GtkWidget *red_score_label, GtkWidget *red_turn_label);
extern void check_end_and_update(GtkWidget *status_label, GtkWidget *drawing_area);

// Helpers locaux
gboolean is_local_player_turn() {
    return (network_socket < 0) || (current_player == local_player);
}

// Coupes possibles
extern gboolean highlight_moves[N][N];
extern void compute_possible_moves(int r0, int c0);

// Wrapper pour ai_move compatible avec GSourceFunc
static gboolean ai_move_wrapper(gpointer user_data) {
    (void)user_data;
    gboolean result = ai_move();
    
    // Mettre à jour l'interface graphique après le coup de l'IA
    if (result) {
        turn_count++; // Incrémenter le compteur de tours
        update_labels(blue_score_label, blue_turn_label, red_score_label, red_turn_label);
        check_end_and_update(NULL, drawing_area);
        gtk_widget_queue_draw(drawing_area);
    }
    
    return FALSE; // Toujours arrêter le timer après un seul appel
}

// Fonction pour vérifier les coups reçus via réseau
gboolean check_network_moves(gpointer user_data) {
    (void)user_data;
    
    int from_r, from_c, to_r, to_c;
    int result = network_check_move(&from_r, &from_c, &to_r, &to_c);
    
    if (result == 1) {
        // Coup reçu, l'appliquer
        if (try_move(from_r, from_c, to_r, to_c)) {
            turn_count++;
            current_player = (current_player == BLUE) ? RED : BLUE;
            update_labels(blue_score_label, blue_turn_label, red_score_label, red_turn_label);
            check_end_and_update(NULL, drawing_area);
            gtk_widget_queue_draw(drawing_area);
            
            // Vérifier si c'est le tour de l'IA après avoir reçu un coup réseau
            if (!is_game_over() && ai_enabled && current_player == ai_player) {
                g_timeout_add(300, ai_move_wrapper, NULL); // Petit délai pour l'animation
            }
        } else {
            printf("[DEBUG][GUI] Échec application coup reçu\n");
        }
    } else if (result == -1) {
        printf("[DEBUG][GUI] Connexion fermée détectée\n");
    }
    
    return TRUE; // Continuer le timer
}

// ----------- Dessin du plateau classique -----------
static void draw_grid(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    (void)area; (void)width; (void)height; (void)user_data;

    // Arrière-plan blanc classique
    cairo_set_source_rgb(cr, 0.98, 0.98, 0.98);
    cairo_paint(cr);

    double offset_x = MARGIN;
    double offset_y = MARGIN;

    // Coordonnées avec style classique
    cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
    cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16);
    
    // Lettres A-I en haut
    for (int i = 0; i < GRID_SIZE; i++) {
        char letter[2] = {'A' + i, '\0'};
        cairo_text_extents_t extents;
        cairo_text_extents(cr, letter, &extents);
        
        double x = offset_x + i * CELL_SIZE + CELL_SIZE/2.0 - extents.width/2.0;
        double y = 20;
        
        cairo_move_to(cr, x, y);
        cairo_show_text(cr, letter);
    }
    
    // Chiffres 9-1 à gauche
    for (int i = 0; i < GRID_SIZE; i++) {
        char number[2] = {'9' - i, '\0'};
        cairo_text_extents_t extents;
        cairo_text_extents(cr, number, &extents);
        
        double x = 10;  // Plus d'espacement depuis le bord
        double y = offset_y + i * CELL_SIZE + CELL_SIZE/2.0 + extents.height/2.0;
        
        cairo_move_to(cr, x, y);
        cairo_show_text(cr, number);
    }

    // Cases contrôlées avec couleurs subtiles
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (control_grid[r][c] == CTRL_BLUE) {
                cairo_set_source_rgba(cr, 0.7, 0.85, 1.0, 0.4);
                cairo_rectangle(cr, offset_x + c*CELL_SIZE, offset_y + r*CELL_SIZE, CELL_SIZE, CELL_SIZE);
                cairo_fill(cr);
            } else if (control_grid[r][c] == CTRL_RED) {
                cairo_set_source_rgba(cr, 1.0, 0.7, 0.7, 0.4);
                cairo_rectangle(cr, offset_x + c*CELL_SIZE, offset_y + r*CELL_SIZE, CELL_SIZE, CELL_SIZE);
                cairo_fill(cr);
            }
        }
    }

    // Surbrillance de la pièce sélectionnée
    if (has_selection) {
        cairo_set_source_rgba(cr, 1.0, 1.0, 0.0, 0.6); // Jaune doré
        cairo_set_line_width(cr, 4);
        cairo_rectangle(cr, offset_x + sel_c*CELL_SIZE + 2, offset_y + sel_r*CELL_SIZE + 2, 
                       CELL_SIZE - 4, CELL_SIZE - 4);
        cairo_stroke(cr);
        
        // Effet de pulsation avec un second contour
        cairo_set_source_rgba(cr, 1.0, 0.8, 0.0, 0.4);
        cairo_set_line_width(cr, 2);
        cairo_rectangle(cr, offset_x + sel_c*CELL_SIZE + 1, offset_y + sel_r*CELL_SIZE + 1, 
                       CELL_SIZE - 2, CELL_SIZE - 2);
        cairo_stroke(cr);
    }

    // Surbrillance des mouvements possibles
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (highlight_moves[r][c]) {
                // Fond gris foncé translucide pour les cases de destination
                cairo_set_source_rgba(cr, 0.4, 0.4, 0.4, 0.4);
                cairo_rectangle(cr, offset_x + c*CELL_SIZE, offset_y + r*CELL_SIZE, CELL_SIZE, CELL_SIZE);
                cairo_fill(cr);
                
                // Bordure gris foncé pour bien marquer la case
                cairo_set_source_rgba(cr, 0.3, 0.3, 0.3, 0.8);
                cairo_set_line_width(cr, 3);
                cairo_rectangle(cr, offset_x + c*CELL_SIZE + 3, offset_y + r*CELL_SIZE + 3, 
                               CELL_SIZE - 6, CELL_SIZE - 6);
                cairo_stroke(cr);
                
                // Point central pour indiquer la destination
                double cx = offset_x + c * CELL_SIZE + CELL_SIZE/2.0;
                double cy = offset_y + r * CELL_SIZE + CELL_SIZE/2.0;
                cairo_set_source_rgba(cr, 0.2, 0.2, 0.2, 0.9);
                cairo_arc(cr, cx, cy, 6, 0, 2 * M_PI);
                cairo_fill(cr);
            }
        }
    }

    // Grille classique
    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_set_line_width(cr, 2);
    
    // Lignes horizontales
    for (int i = 0; i <= GRID_SIZE; i++) {
        cairo_move_to(cr, offset_x, offset_y + i * CELL_SIZE);
        cairo_line_to(cr, offset_x + GRID_SIZE * CELL_SIZE, offset_y + i * CELL_SIZE);
        cairo_stroke(cr);
    }
    
    // Lignes verticales
    for (int j = 0; j <= GRID_SIZE; j++) {
        cairo_move_to(cr, offset_x + j * CELL_SIZE, offset_y);
        cairo_line_to(cr, offset_x + j * CELL_SIZE, offset_y + GRID_SIZE * CELL_SIZE);
        cairo_stroke(cr);
    }

    // Cités avec style classique
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (is_city(r, c)) {
                double cx = offset_x + c * CELL_SIZE + CELL_SIZE/2.0;
                double cy = offset_y + r * CELL_SIZE + CELL_SIZE/2.0;
                
                // Fond doré pour la cité
                cairo_set_source_rgba(cr, 1.0, 0.84, 0.0, 0.3);
                cairo_arc(cr, cx, cy, 20, 0, 2 * M_PI);
                cairo_fill(cr);
                
                // Bordure dorée
                cairo_set_source_rgb(cr, 0.8, 0.65, 0.0);
                cairo_set_line_width(cr, 2);
                cairo_arc(cr, cx, cy, 18, 0, 2 * M_PI);
                cairo_stroke(cr);
                
                // Symbole de la cité avec image SVG
                cairo_set_source_rgb(cr, 0.6, 0.4, 0.0);
                
                // Essayer de charger l'image SVG
                GError *error = NULL;
                GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size("assets/city_icon.svg", 24, 24, &error);
                
                if (pixbuf && !error) {
                    // Afficher l'image SVG
                    gdk_cairo_set_source_pixbuf(cr, pixbuf, cx - 12, cy - 12);
                    cairo_paint(cr);
                    g_object_unref(pixbuf);
                } else {
                    // Fallback: dessiner le caractère directement
                    cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
                    cairo_set_font_size(cr, 20);
                    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Noir
                    
                    cairo_text_extents_t extents;
                    cairo_text_extents(cr, "市", &extents);
                    
                    if (extents.width > 0) {
                        double text_x = cx - extents.width/2.0 - extents.x_bearing;
                        double text_y = cy - extents.height/2.0 - extents.y_bearing;
                        cairo_move_to(cr, text_x, text_y);
                        cairo_show_text(cr, "市");
                    } else {
                        // Fallback final: carré stylisé
                        cairo_set_source_rgb(cr, 0.6, 0.4, 0.0);
                        cairo_rectangle(cr, cx - 8, cy - 8, 16, 16);
                        cairo_fill(cr);
                        cairo_set_source_rgb(cr, 1.0, 0.84, 0.0);
                        cairo_rectangle(cr, cx - 6, cy - 6, 12, 12);
                        cairo_stroke(cr);
                    }
                    
                    if (error) g_error_free(error);
                }
            }
        }
    }

    // Pièces avec style classique
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            Piece p = board[r][c];
            if (p == EMPTY) continue;

            double cx = offset_x + c * CELL_SIZE + CELL_SIZE/2.0;
            double cy = offset_y + r * CELL_SIZE + CELL_SIZE/2.0;
            double radius = CELL_SIZE / 3.2;

            // Ombre légère
            cairo_set_source_rgba(cr, 0, 0, 0, 0.2);
            cairo_arc(cr, cx + 1, cy + 1, radius, 0, 2 * M_PI);
            cairo_fill(cr);

            // Effet supplémentaire pour la pièce sélectionnée
            gboolean is_selected = (has_selection && sel_r == r && sel_c == c);
            if (is_selected) {
                // Halo doré autour de la pièce sélectionnée
                cairo_set_source_rgba(cr, 1.0, 0.8, 0.0, 0.5);
                cairo_arc(cr, cx, cy, radius + 6, 0, 2 * M_PI);
                cairo_fill(cr);
                
                // Second halo plus fin
                cairo_set_source_rgba(cr, 1.0, 1.0, 0.0, 0.3);
                cairo_arc(cr, cx, cy, radius + 3, 0, 2 * M_PI);
                cairo_fill(cr);
            }

            // Couleur de la pièce
            if (p == BLUE_SOLDIER || p == BLUE_KING) {
                if (is_selected) {
                    cairo_set_source_rgb(cr, 0.3, 0.5, 0.9); // Plus vif si sélectionnée
                } else {
                    cairo_set_source_rgb(cr, 0.2, 0.4, 0.8);
                }
            } else {
                if (is_selected) {
                    cairo_set_source_rgb(cr, 0.9, 0.3, 0.3); // Plus vif si sélectionnée
                } else {
                    cairo_set_source_rgb(cr, 0.8, 0.2, 0.2);
                }
            }

            cairo_arc(cr, cx, cy, radius, 0, 2 * M_PI);
            cairo_fill(cr);

            // Bordure
            cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
            cairo_set_line_width(cr, is_selected ? 3 : 2); // Bordure plus épaisse si sélectionnée
            cairo_arc(cr, cx, cy, radius, 0, 2 * M_PI);
            cairo_stroke(cr);

            // Couronne pour les rois
            if (p == BLUE_KING || p == RED_KING) {
                cairo_set_source_rgb(cr, 1.0, 0.84, 0.0);
                cairo_set_line_width(cr, is_selected ? 4 : 3); // Plus épaisse si sélectionnée
                cairo_arc(cr, cx, cy, radius - 4, 0, 2 * M_PI);
                cairo_stroke(cr);
                
                // Points de couronne
                for (int i = 0; i < 6; i++) {
                    double angle = i * M_PI / 3;
                    double px = cx + (radius - 8) * cos(angle);
                    double py = cy + (radius - 8) * sin(angle);
                    
                    cairo_arc(cr, px, py, is_selected ? 3 : 2, 0, 2 * M_PI); // Plus gros si sélectionnée
                    cairo_fill(cr);
                }
            }
        }
    }
}

// ----------- Gestion clic -----------
static gboolean on_click(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data) {

    (void)gesture; (void)n_press; (void)user_data;

    int col = (int)((x - MARGIN) / CELL_SIZE);
    int row = (int)((y - MARGIN) / CELL_SIZE);

    if (row >= 0 && row < N && col >= 0 && col < N) {
        if (has_selection) {
            if (try_move(sel_r, sel_c, row, col)) {
                // Coup valide, l'envoyer via réseau si connecté
                network_send_move_async(sel_r, sel_c, row, col);
                
                has_selection = FALSE;
                memset(highlight_moves, 0, sizeof(highlight_moves));
                turn_count++;
                current_player = (current_player==BLUE)?RED:BLUE;
                if(!is_game_over() && ai_enabled && current_player == ai_player){ //on passe la main à l'ia si c'est son tour
                    g_timeout_add(300, ai_move_wrapper, NULL); // Utiliser le timer pour la cohérence
                }
            } else {
                has_selection = FALSE;
                memset(highlight_moves, 0, sizeof(highlight_moves));
            }
        } else {
            // Ne permettre de sélectionner que les pièces du joueur local ET du joueur courant
            if (board[row][col] != EMPTY &&
                is_local_player_turn() &&  // C'est le tour du joueur local
                ((current_player==BLUE && is_blue_piece(board[row][col])) ||
                 (current_player==RED && is_red_piece(board[row][col])))) {
                has_selection = TRUE;
                sel_r = row;
                sel_c = col;
                compute_possible_moves(sel_r, sel_c);
            }
        }
        update_labels(blue_score_label, blue_turn_label, red_score_label, red_turn_label);
        check_end_and_update(NULL, drawing_area);
        gtk_widget_queue_draw(drawing_area);
    }
    return TRUE;
}

void gui_activate(GtkApplication *app, gpointer user_data) {
    (void)user_data;
    GtkWidget *window = gtk_application_window_new(app);
    
    // Définir le titre selon le mode réseau et configurer le joueur local
    const char *title = "Krojanty - Local";
    if (network_socket >= 0) {
        if (is_server) {
            title = "Krojanty - Serveur [ROUGE]";
            local_player = RED;  // Serveur joue rouge
        } else {
            title = "Krojanty - Client [BLEU]";
            local_player = BLUE; // Client joue bleu
        }
    } else {
        local_player = BLUE; // Mode local par défaut: bleu
    }
    
    gtk_window_set_title(GTK_WINDOW(window), title);

    // Style classique et élégant
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, 
        "window { "
        "  background: linear-gradient(to bottom, #f8f9fa, #e9ecef); "
        "  border: 1px solid #dee2e6; "
        "}"
        "label { "
        "  color: #212529; "
        "}"
        ".score-panel { "
        "  background: white; "
        "  border: 2px solid #dee2e6; "
        "  border-radius: 8px; "
        "  box-shadow: 0 2px 8px rgba(0,0,0,0.1); "
        "  padding: 15px; "
        "  margin: 5px 5px 5px 10px; "
        "}"
        ".player-title { "
        "  font-weight: bold; "
        "  font-size: 18px; "
        "  margin-bottom: 12px; "
        "}"
        ".score-info { "
        "  font-size: 14px; "
        "  padding: 8px; "
        "  margin: 5px 0; "
        "  background: #f8f9fa; "
        "  border-radius: 4px; "
        "  border-left: 4px solid #6c757d; "
        "}"
        ".turn-indicator { "
        "  font-size: 16px; "
        "  font-weight: bold; "
        "  padding: 12px 8px; "
        "  margin: 5px 0 15px 0; "
        "  border: 1px solid #dee2e6; "
        "  border-radius: 6px; "
        "  color: white; "
        "}"
        ".turn-blue { "
        "  background: linear-gradient(to bottom, #4a90e2, #357abd); "
        "  border-color: #2c5aa0; "
        "  box-shadow: 0 2px 4px rgba(52, 144, 226, 0.3); "
        "}"
        ".turn-red { "
        "  background: linear-gradient(to bottom, #e74c3c, #c0392b); "
        "  border-color: #a93226; "
        "  box-shadow: 0 2px 4px rgba(231, 76, 60, 0.3); "
        "}"
        ".game-board { "
        "  background: white; "
        "  border: 3px solid #6c757d; "
        "  border-radius: 8px; "
        "  box-shadow: 0 4px 12px rgba(0,0,0,0.15); "
        "  margin: 5px 10px 5px 5px; "
        "}"
    );
    gtk_style_context_add_provider_for_display(
        gtk_widget_get_display(window),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    g_object_unref(provider);

    gtk_window_set_default_size(GTK_WINDOW(window), WINDOWS_LONG, WINDOWS_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // Layout principal : scores à gauche, plateau à droite
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_margin_start(main_box, 8);
    gtk_widget_set_margin_end(main_box, 8);
    gtk_widget_set_margin_top(main_box, 8);
    gtk_widget_set_margin_bottom(main_box, 8);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    // Calculer la taille du plateau pour aligner les panneaux
    int board_size = CELL_SIZE * GRID_SIZE + MARGIN * 2;

    // === PANNEAU DES SCORES À GAUCHE ===
    GtkWidget *scores_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_add_css_class(scores_container, "score-panel");
    gtk_widget_set_size_request(scores_container, 150, board_size);
    gtk_widget_set_valign(scores_container, GTK_ALIGN_START);

    // Titre du jeu
    GtkWidget *game_title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(game_title),
        "<span size='16000' weight='bold' color='#495057'>KROJANTY</span>");
    gtk_widget_set_halign(game_title, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(game_title, 15);
    gtk_box_append(GTK_BOX(scores_container), game_title);

    // Indicateur de tour
    turn_center_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(turn_center_label),
        "<span size='12000' weight='bold'>Tour des Bleus</span>");
    gtk_widget_add_css_class(turn_center_label, "turn-indicator");
    gtk_widget_set_halign(turn_center_label, GTK_ALIGN_FILL);
    gtk_box_append(GTK_BOX(scores_container), turn_center_label);

    // Section Joueur Bleu
    GtkWidget *blue_section = gtk_frame_new(NULL);
    gtk_widget_set_margin_top(blue_section, 5);
    
    GtkWidget *blue_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_start(blue_content, 8);
    gtk_widget_set_margin_end(blue_content, 8);
    gtk_widget_set_margin_top(blue_content, 8);
    gtk_widget_set_margin_bottom(blue_content, 8);
    
    blue_score_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(blue_score_label),
        "<span size='10000'>📍 Cases: 0</span>");
    gtk_widget_add_css_class(blue_score_label, "score-info");
    
    blue_turn_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(blue_turn_label),
        "<span size='10000'>⚔️ Pièces: 0 | T: 0</span>");
    gtk_widget_add_css_class(blue_turn_label, "score-info");

    gtk_box_append(GTK_BOX(blue_content), blue_score_label);
    gtk_box_append(GTK_BOX(blue_content), blue_turn_label);
    gtk_frame_set_child(GTK_FRAME(blue_section), blue_content);
    gtk_box_append(GTK_BOX(scores_container), blue_section);

    // Section Joueur Rouge
    GtkWidget *red_section = gtk_frame_new(NULL);
    gtk_widget_set_margin_top(red_section, 5);
    
    GtkWidget *red_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_start(red_content, 8);
    gtk_widget_set_margin_end(red_content, 8);
    gtk_widget_set_margin_top(red_content, 8);
    gtk_widget_set_margin_bottom(red_content, 8);
    
    red_score_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(red_score_label),
        "<span size='10000'>📍 Cases: 0</span>");
    gtk_widget_add_css_class(red_score_label, "score-info");
    
    red_turn_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(red_turn_label),
        "<span size='10000'>⚔️ Pièces: 0 | T: 0</span>");
    gtk_widget_add_css_class(red_turn_label, "score-info");

    gtk_box_append(GTK_BOX(red_content), red_score_label);
    gtk_box_append(GTK_BOX(red_content), red_turn_label);
    gtk_frame_set_child(GTK_FRAME(red_section), red_content);
    gtk_box_append(GTK_BOX(scores_container), red_section);

    // === PLATEAU DE JEU À DROITE ===
    GtkWidget *board_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, board_size, board_size);
    gtk_widget_add_css_class(drawing_area, "game-board");
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_grid, NULL, NULL);
    gtk_widget_set_valign(drawing_area, GTK_ALIGN_START);
    
    gtk_box_append(GTK_BOX(board_container), drawing_area);

    // Ajout dans le layout principal
    gtk_box_append(GTK_BOX(main_box), scores_container);
    gtk_box_append(GTK_BOX(main_box), board_container);

    // Gestion des clics
    GtkGesture *click = gtk_gesture_click_new();
    gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(click));
    g_signal_connect(click, "pressed", G_CALLBACK(on_click), NULL);

    // Timer pour vérifier les coups réseau (seulement en mode réseau)
    if (network_socket >= 0) {
        g_timeout_add(100, check_network_moves, NULL);
    }

    // Initialisation
    new_game_setup();
    
    // En mode réseau, les bleus commencent toujours en premier
    if (network_socket >= 0) {
        current_player = BLUE;  // Les bleus commencent toujours
        if (is_server) {
            // Serveur (rouge) doit attendre son tour - les bleus (client) commencent
        } else {
            // Client (bleu) commence
            // Si l'IA joue bleu (client), elle doit jouer en premier
            if (ai_enabled && ai_player == BLUE) {
                g_timeout_add(500, ai_move_wrapper, NULL); // Délai pour laisser l'interface se charger
            }
        }
    } else {
        // Mode local: bleu commence
        current_player = BLUE;
        // Si l'IA joue rouge en mode local, le joueur humain commence en bleu
    }
    
    update_labels(blue_score_label, blue_turn_label, red_score_label, red_turn_label);
    
    // Démarrer l'IA si c'est son tour au début
    if (ai_enabled && current_player == ai_player) {
        g_timeout_add(500, ai_move_wrapper, NULL); // Délai pour laisser l'interface se charger
    }

    gtk_window_present(GTK_WINDOW(window));
}
