/**
 * @file network.c
 * @brief Module réseau pour la gestion de la communication entre clients et serveur.
 *
 * Fournit les fonctions d'initialisation, d'envoi et de réception asynchrones des coups de jeu,
 * ainsi que la logique d'intégration avec l'UI et l'IA.
 *
 * Code réalisé par Marius Keltz
 *
 * @date 2025-09-18
 */

#include "../includes/network.h"

// Variables globales pour l'état réseau
int network_socket = -1;
int is_server = 0;
int is_connected = 0;
int client_socket = -1; // Pour le serveur

int start_server(int port) {
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) return -1;
    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sock);
        return -1;
    }
    if(listen(sock, 1) < 0) {
        close(sock);
        return -1;
    }
    return sock;
}

int wait_for_client(int server_sock, int *client_sock) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    *client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);
    return *client_sock < 0 ? -1 : 0;
}

int connect_to(const char *ip, int port) {
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if(inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) return -1;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) return -1;
    if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sock);
        return -1;
    }
    return sock;
}

// Envoie un coup au joueur adverse (4 octets)
int send_move(int sock, const char *move) {
    // Envoyer directement les 4 caractères 
    int sent = send(sock, move, 4, 0);
    if(sent < 0) {
        printf("Erreur envoi coup réseau\n");
        return -1;
    }
    
    printf("Coup envoyé: %s\n", move);
    return 0;
}

// Reçoit un coup du joueur adverse (4 octets)
int recv_move(int sock, char *buf, int size) {
    printf("Attente du coup adverse...\n");
    if (size < 5) return -1;
    
    memset(buf, 0, size);
    
    // Recevoir directement les 4 caractères
    int received = recv(sock, buf, 4, 0); 
    if(received < 0) {
        perror("Erreur réception coup");
        return -1;
    }
    
    printf("Coup reçu: %s\n", buf);
    return received;
}

// Fonctions pour compatibilité avec l'ancien code
int send_coords(int sock, const char *coords) {
    return send_move(sock, coords);
}

int recv_coords(int sock, char *buf, int size) {
    return recv_move(sock, buf, size);
}

// Fonctions d'interface pour main.c
int run_server(short port) {
    printf("Démarrage serveur sur port %d\n", port);
    
    // Créer le socket serveur
    int server_sock = start_server(port);
    if (server_sock < 0) {
        printf("Échec démarrage serveur\n");
        return -1;
    }
    
    printf("Serveur en écoute sur port %d\n", port);
    
    // Attendre une connexion client
    int client_sock;
    if (wait_for_client(server_sock, &client_sock) < 0) {
        printf("Échec connexion client\n");
        close(server_sock);
        return -1;
    }
    
    printf("Client connecté\n");
    
    // Boucle de jeu simple
    char buffer[16];
    while (1) {
        // Attendre un coup du client
        int len = recv_move(client_sock, buffer, sizeof(buffer));
        if (len <= 0) {
            printf("Client déconnecté\n");
            break;
        }
        
        // Echo le coup (pour test)
        if (send_move(client_sock, buffer) < 0) {
            printf("Échec envoi coup\n");
            break;
        }
    }
    
    close(client_sock);
    close(server_sock);
    return 0;
}

int run_client(const char *ip, short port) {
    printf("Connexion au serveur %s:%d\n", ip, port);
    
    // Se connecter au serveur
    int server_sock = connect_to(ip, port);
    if (server_sock < 0) {
        printf("Échec connexion serveur\n");
        return -1;
    }
    
    printf("Connecté au serveur\n");
    
    // Boucle de jeu simple
    char buffer[16] = "A1B2"; // Coup test
    while (1) {
        // Envoyer un coup
        if (send_move(server_sock, buffer) < 0) {
            printf("Échec envoi coup\n");
            break;
        }
        
        // Attendre la réponse
        int len = recv_move(server_sock, buffer, sizeof(buffer));
        if (len <= 0) {
            printf("Serveur déconnecté\n");
            break;
        }
        
        // Attendre 2 secondes avant le prochain coup
        sleep(2);
    }
    
    close(server_sock);
    return 0;
}

// ===== NOUVELLES FONCTIONS POUR INTÉGRATION GUI =====

// Initialise le serveur (non-bloquant)
int network_init_server(short port) {
    network_socket = start_server(port);
    if (network_socket < 0) {
        printf("Échec démarrage serveur réseau\n");
        return -1;
    }
    
    // Rendre le socket non-bloquant
    int flags = fcntl(network_socket, F_GETFL, 0);
    fcntl(network_socket, F_SETFL, flags | O_NONBLOCK);
    
    is_server = 1;
    is_connected = 0;
    client_socket = -1;
    printf("Serveur en écoute sur port %d\n", port);
    
    return 0;
}

// Initialise le client (non-bloquant)
int network_init_client(const char *ip, short port) {
    network_socket = connect_to(ip, port);
    if (network_socket < 0) {
        printf("Échec connexion serveur réseau\n");
        return -1;
    }
    
    // Rendre le socket non-bloquant
    int flags = fcntl(network_socket, F_GETFL, 0);
    fcntl(network_socket, F_SETFL, flags | O_NONBLOCK);
    
    is_server = 0;
    is_connected = 1;
    printf("Client connecté au serveur %s:%d\n", ip, port);
    
    return 0;
}

// Envoie un coup de manière asynchrone
int network_send_move_async(int from_r, int from_c, int to_r, int to_c) {
    if (!is_connected) {
        // Si serveur, essayer d'accepter une connexion
        if (is_server && client_socket == -1) {
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            client_socket = accept(network_socket, (struct sockaddr*)&client_addr, &addr_len);
            if (client_socket >= 0) {
                // Rendre le socket client non-bloquant
                int flags = fcntl(client_socket, F_GETFL, 0);
                fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);
                is_connected = 1;
                printf("Client connecté!\n");
            } else {
                return 0; // Pas encore de client
            }
        } else {
            return 0; // Pas connecté
        }
    }
    
    // Formater le coup (ex: "A9B8") - lignes de 9 à 1
    char move[5];
    move[0] = 'A' + from_c;
    move[1] = '9' - from_r;  // Inverser: ligne 0 = '9', ligne 8 = '1'
    move[2] = 'A' + to_c;
    move[3] = '9' - to_r;    // Inverser: ligne 0 = '9', ligne 8 = '1'
    move[4] = '\0';
    
    int sock = is_server ? client_socket : network_socket;
    int result = send_move(sock, move);
    
    if (result < 0) {
        printf("Échec envoi coup réseau, déconnexion\n");
        is_connected = 0;
        if (is_server && client_socket >= 0) {
            close(client_socket);
            client_socket = -1;
        }
        return -1;
    }
    
    return 1;
}

// Vérifie s'il y a un coup reçu (non-bloquant)
int network_check_move(int *from_r, int *from_c, int *to_r, int *to_c) {
    if (!is_connected) {
        // Si serveur, essayer d'accepter une connexion
        if (is_server && client_socket == -1) {
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            client_socket = accept(network_socket, (struct sockaddr*)&client_addr, &addr_len);
            if (client_socket >= 0) {
                // Rendre le socket client non-bloquant
                int flags = fcntl(client_socket, F_GETFL, 0);
                fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);
                is_connected = 1;
                printf("Client connecté!\n");
            }
        }
        if (!is_connected) {
            return 0; // Pas connecté
        }
    }
    
    char buffer[16];
    int sock = is_server ? client_socket : network_socket;
    
    // Essayer de recevoir un coup (non-bloquant)
    int received = recv(sock, buffer, 4, 0);
    
    if (received == 4) {
        buffer[4] = '\0';
        
        // Décoder le coup - lignes de 9 à 1
        *from_c = buffer[0] - 'A';
        *from_r = '9' - buffer[1];  // Inverser: '9' = ligne 0, '1' = ligne 8
        *to_c = buffer[2] - 'A';
        *to_r = '9' - buffer[3];    // Inverser: '9' = ligne 0, '1' = ligne 8
        
        printf("Coup réseau reçu: (%d,%d) -> (%d,%d)\n", *from_r, *from_c, *to_r, *to_c);
        return 1; // Coup reçu
    } else if (received == 0) {
        // Connexion fermée
        printf("Connexion réseau fermée\n");
        is_connected = 0;
        if (is_server && client_socket >= 0) {
            close(client_socket);
            client_socket = -1;
        }
        return -1;
    } else if (received < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        // Erreur de connexion
        printf("Erreur réseau, déconnexion\n");
        is_connected = 0;
        if (is_server && client_socket >= 0) {
            close(client_socket);
            client_socket = -1;
        }
        return -1;
    }
    
    return 0; // Pas de coup reçu
}

// Nettoie les ressources réseau
void network_cleanup() {
    if (network_socket >= 0) {
        close(network_socket);
        network_socket = -1;
    }
    if (client_socket >= 0) {
        close(client_socket);
        client_socket = -1;
    }
    is_connected = 0;
    is_server = 0;
}
