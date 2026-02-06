#ifndef NET_H
#define NET_H
#include "../includes/network.h"
#include "../includes/header_commun.h"
#include <arpa/inet.h>      // htonl(), ntohl()
#include <sys/socket.h>
#include <netinet/in.h>     // struct sockaddr_in, htons()
#include <fcntl.h>
#include <errno.h>

// Définition du type SOCKET et des constantes d’erreur
typedef int SOCKET;
#define INVALID_SOCKET  (-1)
#define SOCKET_ERROR    (-1)

// Variables globales pour l'état réseau
extern int network_socket;
extern int is_server;
extern int is_connected;

// -- Serveur TCP ---------------------------------

int run_server(short port);
int run_client(const char *ip, short port);


// Fonctions réseau manquantes pour les tests
int start_server(int port);
int wait_for_client(int server_sock, int *client_sock);
int connect_to(const char *ip, int port);
int send_move(int sock, const char *move);
int recv_move(int sock, char *buf, int size);
int send_coords(int sock, const char *coords);
int recv_coords(int sock, char *buf, int size);

// Nouvelles fonctions pour intégration GUI
int network_init_server(short port);
int network_init_client(const char *ip, short port);
int network_send_move_async(int from_r, int from_c, int to_r, int to_c);
int network_check_move(int *from_r, int *from_c, int *to_r, int *to_c);
void network_cleanup();


SOCKET  TCP_Create_Server(short port);
void    server_exchange(SOCKET s);
int     TCP_String_Reader(int sock, char *buffer);
int     TCP_Long_Writer(SOCKET s, long value);

// -- Client TCP ------------------------
// // Démarre le mode client (connexion + GUI)
int client_run(const char *host, short port);
SOCKET  TCP_Create_Client(const char *server_ip, short port);
int     TCP_String_Writer(SOCKET s, const char *output);
int     TCP_Long_Reader(SOCKET s, long *value);

#endif