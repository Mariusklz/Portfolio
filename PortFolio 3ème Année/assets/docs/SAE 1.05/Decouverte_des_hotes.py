
import argparse
from scapy.all import *

#Définition des arguments utilisables pour ce script ainsi que des messages d'aides
parser = argparse.ArgumentParser(description="Découverte automatique des hôtes.")
parser.add_argument("-a", metavar="Adresse_IP", help="--> Découverte active pour une adresse IP fournie.")
parser.add_argument("-p", metavar="Adresse_IP", help="--> Découverte passive pour une adresse IP fournie.")
parser.add_argument("-t", metavar="Adresse_réseau", help="--> Découverte des hôtes sur un réseau dont l'adresse est fournie.")
parser.add_argument("-x", metavar="Chemin_du_fichier", help="--> Ecriture de la sortie dans un fichier dont le nom est donné.")

args = parser.parse_args()

# Definition de la fonction pour la découverte active
def decouverte_active(ip_cible, fichier_sortie=None):
    print(f"--> Découverte active pour {ip_cible}...")
    reponse = sr1(IP(dst=ip_cible)/ICMP(), timeout=3, verbose=0) # Envoi d'une requête ICMP à l'adresse ciblée
    if reponse is not None: # Dans le cas où l'on obtient une réponse
        retour_commande = f"--> L'hôte {ip_cible} est connecté."
        print(retour_commande)
    else: # Dans le cas où l'on obtient pas de réponse
        retour_commande = f"--> L'hôte {ip_cible} n'est pas connecté."
        print(retour_commande)
    if fichier_sortie: # Dans le cas où un fichier de sortie est spécifié, écrit le message dans le fichier
        ecriture_fichier(fichier_sortie, retour_commande)

# Definition de la fonction pour la découverte passive
def decouverte_passive(ip_cible, fichier_sortie=None):
    def analyse_paquet_arp(paquet):
        if paquet[ARP].op == 1 and paquet[ARP].psrc == ip_cible: # Dans le cas où le paquet est une requête ARP (op == 1) et provient de l'IP cible
            retour_commande = f"--> L'adresse IP {ip_cible} est active. Adresse MAC : {paquet[ARP].hwsrc}"
            print(retour_commande)
            if fichier_sortie: # Dans le cas où un fichier de sortie est spécifié, écrit le message dans le fichier
                ecriture_fichier(fichier_sortie, retour_commande)
    sniff(prn=analyse_paquet_arp(), filter="arp", store=0) # Capture et traite des paquets ARP


# Definition de la fonction pour incrémenter le dernier octet d'une adresse IP
def increment_ip(ip):
    ip_liste = ip.split('.')
    ip_liste[-1] = str(int(ip_liste[-1]) + 1)
    return '.'.join(ip_liste)

# Definition de la fonction pour scanner le réseau
def scan_reseau(reseau_cible, fichier_sortie=None):
    print(f"--> Découverte du réseau {reseau_cible}...")
    conf.verb = 0
    reseau, _, masque = reseau_cible.partition('/') # Séparation de l'adresse réseau et du masque
    masque = int(masque)
    n_adresse = 2 ** (32 - masque) # Calcul du nombre d'adresses IP possibles dans le réseau en utilisant le masque
    adresses_actives = []
    ip_actuelle = reseau
    for _ in range(n_adresse): # Boucle à travers toutes les adresses IP possibles dans le réseau
        icmp_packet = IP(dst=ip_actuelle)/ICMP()
        ans, _ = sr(icmp_packet, timeout=1, verbose=False) # Envoi d'un paquet ICMP et attente d'une réponse pendant 1 seconde
        for sent, received in ans:
            if received[ICMP].type == 0: # Dans le cas où la réponse est un paquet ICMP de type 0 (réponse au ping), considérer l'adresse IP comme active
                adresses_actives.append(received[IP].src)
        ip_actuelle = increment_ip(ip_actuelle) # Passer à l'adresse IP suivante dans la boucle

    # Affiche un résumé des adresses IP actives
    retour_commande = "--> Résumé des adresses IP actives :"
    print(retour_commande)
    if fichier_sortie: # Dans le cas où un fichier de sortie est spécifié, écrit le message dans le fichier
        ecriture_fichier(fichier_sortie, retour_commande)
    for adresses_actives in adresses_actives:
        retour_commande = f"Adresse IP active : {adresses_actives}"
        print(retour_commande)
        if fichier_sortie: # Dans le cas où un fichier de sortie est spécifié, écrit le message dans le fichier
            ecriture_fichier(fichier_sortie, retour_commande)

# Definition de la fonction pour écrire la sortie dans un fichier
def ecriture_fichier(nom_fichier, retour_commande):
    with open(nom_fichier, "a") as file: # Ouverture du fichier
        file.write(retour_commande + "\n") # Écriture de la chaîne de texte suivie d'un saut de ligne


# Activation des différentes fonctions en fonction des arguments en entrée
if args.a:
    decouverte_active(args.a, args.x)
elif args.p:
    decouverte_passive(args.p, args.x)
elif args.t:
    scan_reseau(args.t, args.x)