/**
 * @file main.c
 * @brief Fichier de code principal qui centralise les différents fichiers
 *
 * Code réalisé par l'équipe le groupe 7 des 1ère années Informatique et Réseau de l'ENSISA.
 *
 * @date 2025-09-16
 * @version 1.0
 */

#include "gui.h"
#include "ai.h"
#include "app.h"
#include "../includes/network.h"

// Variables de app.c
extern Piece board[N][N];
extern Player current_player;

// Variables de ai.c
extern gboolean ai_enabled;
extern Player ai_player;

/**
 * @param argc Nombre d’arguments en ligne de commande.
 * @param argv Tableau des arguments en ligne de commande.
 * @return Code de retour du programme.
 *
 * @details
 * Les modes disponibles sont :
 * - `-l` : jeu local (avec option `-ai` pour jouer contre l’IA rouge).
 * - `-s [-ia] <port>` : mode serveur (rouge), option IA possible.
 * - `-c [-ia] <ip:port>` : mode client (bleu), option IA possible.
 *
 * Selon le mode, initialise le réseau avec @ref network_init_server ou @ref network_init_client,
 * configure l’IA si demandée, et lance l’application GTK avec @ref gui_activate.
 */

int main(int argc, char **argv) {
    GtkApplication *app;
    const char *app_id = "dev.krojanty.game"; // ID par défaut

    if (argc < 2) {
        fprintf(stderr,
            "Usage:\n"
            "  %s -l\n"
            "  %s -l -ia\n"
            "  %s -s [-ia] <port>\n"
            "  %s -c [-ia] <ip:port>\n",
            argv[0], argv[0], argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    // --- Mode local ---
    if (strcmp(argv[1], "-l") == 0) {
        app_id = "dev.krojanty.game.local";
        if (argc ==3 && (strcmp(argv[2], "-ia") == 0)){
            ai_enabled = TRUE;
            ai_player = RED;
        }
        // Mode local configuré silencieusement
    } 

    // --- Mode serveur ---
    else if (strcmp(argv[1], "-s") == 0) {
        if (argc < 3 || argc > 4) {
            fprintf(stderr, "Usage: %s -s [-ia] <port>\n", argv[0]);
            return EXIT_FAILURE;
        }
        
        gboolean ai_requested = FALSE;
        short port;
        
        if (argc == 4 && strcmp(argv[2], "-ia") == 0) {
            ai_requested = TRUE;
            port = atoi(argv[3]);
        } else if (argc == 3) {
            port = atoi(argv[2]);
        } else {
            fprintf(stderr, "Usage: %s -s [-ia] <port>\n", argv[0]);
            return EXIT_FAILURE;
        }
        
        if (ai_requested) {
            ai_enabled = TRUE;
            ai_player = RED; // L'IA joue rouge (serveur)
            printf("Mode serveur avec IA sur port %d\n", port);
        } else {
            printf("Mode serveur sur port %d\n", port);
        }
        
        if (network_init_server(port) < 0) {
            fprintf(stderr, "Erreur: Impossible de démarrer le serveur\n");
            return EXIT_FAILURE;
        }
        app_id = "dev.krojanty.game.server";
    } 

    // --- Mode client ---
    else if (strcmp(argv[1], "-c") == 0) {
        if (argc < 3 || argc > 4) {
            fprintf(stderr, "Usage: %s -c [-ia] <ip:port>\n", argv[0]);
            return EXIT_FAILURE;
        }
        
        gboolean ai_requested = FALSE;
        char *addr;
        
        if (argc == 4 && strcmp(argv[2], "-ia") == 0) {
            ai_requested = TRUE;
            addr = strdup(argv[3]);
        } else if (argc == 3) {
            addr = strdup(argv[2]);
        } else {
            fprintf(stderr, "Usage: %s -c [-ia] <ip:port>\n", argv[0]);
            return EXIT_FAILURE;
        }
        
        char *colon = strchr(addr, ':');
        if (!colon) {
            fprintf(stderr, "Format attendu: <ip:port>\n");
            free(addr);
            return EXIT_FAILURE;
        }
        *colon = '\0';
        const char *ip = addr;
        short port = atoi(colon + 1);
        
        if (ai_requested) {
            ai_enabled = TRUE;
            ai_player = BLUE; // L'IA joue bleu (client)
            printf("Mode client avec IA vers %s:%d\n", ip, port);
        } else {
            printf("Mode client vers %s:%d\n", ip, port);
        }

        if (network_init_client(ip, port) < 0) {
            fprintf(stderr, "Erreur: Impossible de se connecter au serveur\n");
            free(addr);
            return EXIT_FAILURE;
        }
        
        free(addr);
        app_id = "dev.krojanty.game.client";
    }

    // --- Lancer GTK ---
    app = gtk_application_new(app_id, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(gui_activate), NULL);
    
    atexit(network_cleanup);
    
    int status = g_application_run(G_APPLICATION(app), 1, argv); // argc=1 pour ignorer les args GTK
    g_object_unref(app);
    
    return status;
}
