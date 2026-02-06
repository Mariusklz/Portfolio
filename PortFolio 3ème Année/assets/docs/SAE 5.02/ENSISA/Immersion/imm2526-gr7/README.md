# Krojanty - Groupe 7 - ENSISA - PROJET 

```
██╗  ██╗██████╗  ██████╗      ██╗ █████╗ ███╗   ██╗████████╗██╗   ██╗  
██║ ██╔╝██╔══██╗██╔═══██╗     ██║██╔══██╗████╗  ██║╚══██╔══╝╚██╗ ██╔╝  
█████╔╝ ██████╔╝██║   ██║     ██║███████║██╔██╗ ██║   ██║    ╚████╔╝   
██╔═██╗ ██╔══██╗██║   ██║██   ██║██╔══██║██║╚██╗██║   ██║     ╚██╔╝    
██║  ██╗██║  ██║╚██████╔╝╚█████╔╝██║  ██║██║ ╚████║   ██║      ██║     
╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝  ╚════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝      ╚═╝     

       /\                                                 /\  
_      )( ______________________   ______________________ )(      _  
(_)////(**)______________________> <______________________(**)\\(_)  
       )(                                                 )(  
       \/                                                 \/  
```

---

## Groupe 7
- Grégory Runser  
- Marius Keltz  
- Guillaume Schossig  
- Yunus Demirel  
- Hermès Noumbogo
- Nour Karbil
- Zyad Idelkaid

---

## Compilation

### Jeux
La compilation produit l’exécutable `./game` :
```bash
make
```

### Tests
Compile et exécute les tests sous `./tests` :
```bash
make tests
```

### Documentation
Production de la documentation, ouvrir le fichier `./html/index.html` dans son navigateur.
```bash
make docs
```

---

## Utilisation

Exécution du programme avec différentes options :
```bash
./game
```

### Options disponibles
- `-l` : mode local (une seule interface)  
- `-c <hhh.hhh.hhh.hhh:pppp>` : IP et port sous la forme `hhh.hhh.hhh.hhh:pppp`  
- `-s <pppp>` : port sous la forme numérique `pppp`  
- `-c -ia <hhh.hhh.hhh.hhh:pppp>` : ajout d'une IA côté client pour calculer les coups  
- `-s -ia <pppp>` : ajout d’une IA côté serveur pour calculer les coups  

---

## Règles du jeu

### 1. Plateau
- Le jeu se joue sur un plateau **9x9**.  
- Chaque joueur possède : **1 roi et 9 soldats**.  
- Les coins du plateau représentent les **cités des camps**.  

### 2. Mise en place
- Chaque joueur place son roi et ses soldats selon la configuration initiale.  
- Le joueur **bleu commence**.  

### 3. Déplacements
- Les pièces (roi et soldats) se déplacent en ligne droite (horizontale ou verticale) d’autant de cases que voulu.  
- Elles ne peuvent pas sauter ni se poser sur une autre pièce.  

### 4. Objectifs et conditions de victoire
- **Conquête** : amener son roi dans la cité adverse.  
- **Extermination** : capturer 8 soldats adverses.  
- **Exécution** : capturer le roi adverse.  
- Si la partie atteint **64 tours (32 par joueur)** : contrôle du plateau.  
  - +1 point par case contrôlée.  
  - +1 point par soldat encore vivant (le roi ne compte pas).  
  - Le joueur avec le plus haut score gagne.  

### 5. Contrôle du plateau
- Chaque joueur contrôle **10 cases au départ**.  
- Une case est contrôlée quand une pièce s’y déplace (même après qu’elle en soit partie).  
- Les cités ne peuvent pas être contrôlées.  

### 6. Capture des pièces
- Une capture ne se fait qu’après un mouvement (**pas de capture passive**).  
- Deux types de capture :  
  1. **Linca (prise sandwich)** : une pièce adverse est prise entre deux pièces alliées.  
  2. **Seultou (poussée)** : une pièce est poussée sans soutien derrière et disparaît.  

### 7. Fin de partie
- Victoire immédiate par conquête, extermination ou exécution.  
- Sinon, victoire aux points après **64 tours**.  

---

## Résumé
Protégez votre roi, tentez de conquérir la cité ennemie ou d’éliminer ses forces !  
