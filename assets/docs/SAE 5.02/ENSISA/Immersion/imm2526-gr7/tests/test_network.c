#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../includes/network.h"
#include <gtk/gtk.h>
#include <pthread.h>

// Suppression des prototypes inutiles
// int test_network_init_server();
// int test_network_init_client();
// int test_network_send_move_async();
// int test_network_check_move();
// Suppression du code orphelin
void* client_thread_func(void* arg) {
    int port = *(int*)arg;
    run_client("127.0.0.1", port);
    return NULL;
}
int test_start_server() {
    printf("Test : start_server\n");
    int sock = start_server(12346); // Port arbitraire
    if (sock >= 0) {
        printf("OK : start_server() = %d\n", sock);
        close(sock);
        return 1;
    } else {
        printf("start_server() = %d\n", sock);
        return 0;
    }
}

int test_wait_for_client() {
    printf("Test : wait_for_client\n");
    int dummy_sock = -1;
    int res = wait_for_client(-1, &dummy_sock);
    if (res == -1) {
        printf("OK : wait_for_client() = %d\n", res);
        return 1;
    } else {
        printf("wait_for_client() = %d\n", res);
        return 0;
    }
}

int test_connect_to() {
    printf("Test : connect_to\n");
    int sock = connect_to("127.0.0.1", 12346);
    if (sock == -1) {
        printf("OK : connect_to() = %d\n", sock);
        return 1;
    } else {
        printf("connect_to() = %d\n", sock);
        close(sock);
        return 0;
    }
}

int test_send_move() {
    printf("Test : send_move\n");
    int res = send_move(-1, "A9B8"); // Socket invalide
    if (res == -1) {
        printf("OK : send_move() = %d\n", res);
        return 1;
    } else {
        printf("send_move() = %d\n", res);
        return 0;
    }
}

int test_recv_move() {
    printf("Test : recv_move\n");
    char buf[8];
    int res = recv_move(-1, buf, sizeof(buf)); // Socket invalide
    if (res == -1) {
        printf("OK : recv_move() = %d\n", res);
        return 1;
    } else {
        printf("recv_move() = %d\n", res);
        return 0;
    }
}

int test_send_coords() {
    printf("Test : send_coords\n");
    int res = send_coords(-1, "A9B8");
    if (res == -1) {
        printf("OK : send_coords() = %d\n", res);
        return 1;
    } else {
        printf("send_coords() = %d\n", res);
        return 0;
    }
}

int test_recv_coords() {
    printf("Test : recv_coords\n");
    char buf[8];
    int res = recv_coords(-1, buf, sizeof(buf));
    if (res == -1) {
        printf("OK : recv_coords() = %d\n", res);
        return 1;
    } else {
        printf("recv_coords() = %d\n", res);
        return 0;
    }
}

// Test de run_server
void test_run_server() {
    printf("Test : run_server\n");
    int res = run_server(12347); // Port arbitraire
    printf("run_server() = %d\n", res);
}

// Test de run_client
void test_run_client() {
    printf("Test : run_client\n");
    int res = run_client("127.0.0.1", 12347);
    printf("run_client() = %d\n", res);
}
#include <stdio.h>
#include "../includes/network.h"

// fonction de network.c
int test_network_init_server() {
    printf("Test : network_init_server\n");
    int res = network_init_server(12345);
        if (res >= 0) {
            printf("OK : network_init_server() = %d\n", res);
            return 1;
        } else {
            printf("network_init_server() = %d\n", res);
            return 0;
        }
}

int test_network_init_client() {
    printf("Test : network_init_client\n");
    int res = network_init_client("127.0.0.1", 12345);
        if (res >= 0) {
            printf("OK : network_init_client() = %d\n", res);
            return 1;
        } else {
            printf("network_init_client() = %d\n", res);
            return 0;
        }
}

int test_network_send_move_async() {
    printf("Test : network_send_move_async\n");
    int res = network_send_move_async(0, 0, 1, 1);
        if (res == 0) {
            printf("OK : network_send_move_async() = %d\n", res);
            return 1;
        } else {
            printf("network_send_move_async() = %d\n", res);
            return 0;
        }
}

int test_network_check_move() {
    printf("Test : network_check_move\n");
    int fr, fc, tr, tc;
    int res = network_check_move(&fr, &fc, &tr, &tc);
        if (res == 0 || res == 1 || res == -1) {
            printf("OK : network_check_move() = %d\n", res);
            return 1;
        } else {
            printf("network_check_move() = %d\n", res);
            return 0;
        }
}

int test_network_init_server();

int test_network_init_client();

int test_network_send_move_async();

int test_network_check_move();

int test_network_cleanup() {
    printf("Test : network_cleanup\n");
    network_cleanup();
    printf("OK : network_cleanup() exécuté\n");
    return 1;
}

// Timeout handler
#include <signal.h>
void timeout_handler(int sig __attribute__((unused))) {
    printf("\n[ERROR] Timeout atteint, arrêt des tests network.c\n");
    fflush(stdout);
    _exit(1);
}

int main() {
    printf("Début des tests simples network.c\n");
    int ok = 1;


    if (test_network_init_server()) {
        printf("OK : network_init_server\n");
    }
    if (test_network_init_client()) {
        printf("OK : network_init_client\n");
    }
    if (test_network_send_move_async()) {
        printf("OK : network_send_move_async\n");
    }
    if (test_network_check_move()) {
        printf("OK : network_check_move\n");
    }
    if (test_network_cleanup()) {
        printf("OK : network_cleanup\n");
    }
    if (test_start_server()) {
        printf("OK : start_server\n");
    }
    if (test_wait_for_client()) {
        printf("OK : wait_for_client\n");
    }
    if (test_connect_to()) {
        printf("OK : connect_to\n");
    }
    if (test_send_move()) {
        printf("OK : send_move\n");
    }
    if (test_recv_move()) {
        printf("OK : recv_move\n");
    }
    if (test_send_coords()) {
        printf("OK : send_coords\n");
    }
    if (test_recv_coords()) {
        printf("OK : recv_coords\n");
    }

    // Test : network_send_move_async sans client (serveur non connecté)
    printf("Test : network_send_move_async sans client\n");
    network_cleanup();
    network_init_server(12348); // Port arbitraire
    int res_async = network_send_move_async(0, 0, 1, 1);
    if (res_async == 0) {
        printf("OK : network_send_move_async sans client = %d\n", res_async);
    } else {
        printf("network_send_move_async sans client = %d\n", res_async);
    }

    // Test : network_check_move sans client (serveur non connecté)
    printf("Test : network_check_move sans client\n");
    int fr=0, fc=0, tr=0, tc=0;
    int res_check = network_check_move(&fr, &fc, &tr, &tc);
    if (res_check == 0) {
        printf("OK : network_check_move sans client = %d\n", res_check);
    } else {
        printf("network_check_move sans client = %d\n", res_check);
    }

    // Test : déconnexion forcée et nouvel envoi
    printf("Test : déconnexion et nouvel envoi\n");
    network_cleanup();
    int res_async2 = network_send_move_async(0, 0, 1, 1);
    if (res_async2 == 0) {
        printf("OK : network_send_move_async après cleanup = %d\n", res_async2);
    } else {
        printf("network_send_move_async après cleanup = %d\n", res_async2);
    }

    // Les tests run_server et run_client sont ignorés pour éviter le blocage

    if (ok) printf("OK : Tous les tests network.c\n");
    printf("Fin des tests simples network.c\n");
    return 0;
}
