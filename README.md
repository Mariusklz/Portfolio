# Portfolio Personnel - KELTZ Marius

Portfolio professionnel présentant mes compétences, projets académiques et expériences en entreprise dans le domaine des Réseaux & Télécommunications.

URL: https://mariusklz.github.io/Portfolio/

## Description

Ce portfolio présente mon parcours en BUT Réseaux & Télécommunications, mes projets SAE, mes réalisations en entreprise chez Wienerberger, ainsi que mes compétences techniques.

## Fonctionnalités

- Design moderne et responsive (6 breakpoints)
- Menu mobile hamburger avec navigation fluide
- Navigation SPA (Single Page Application)
- Animations au scroll (Intersection Observer)
- Formulaire de contact avec validation
- Bouton "Retour en haut"
- Highlight de la section active
- Sections complètes :
  - Accueil
  - À propos
  - Compétences (Référentiel BUT R&T)
  - Projets SAE (Semestres 1 à 5)
  - Projets Personnels
  - Projets Entreprise (Wienerberger)
  - Annexes et justificatifs
  - Contact

## Structure du projet

```
Portfolio/
├── index.html              # Page d'accueil
├── competences.html        # Page compétences
├── projets.html           # Projets SAE
├── perso.html             # Projets personnels
├── entreprise.html        # Projets entreprise
├── annexes.html           # Annexes et justificatifs
├── merci.html             # Page de confirmation
├── css/
│   └── style.css          # Styles globaux
├── js/
│   ├── main.js            # Scripts principaux
│   └── spa.js             # Navigation SPA
├── assets/
│   ├── docs/              # Documents et justificatifs
│   │   ├── SAE 1.01/      # Documents SAE par semestre
│   │   ├── SAE 1.02/
│   │   └── entreprise/    # Documents projets entreprise
│   ├── images/            # Images du portfolio
│   └── icons/             # Icônes et favicon
├── .github/
│   └── copilot-instructions.md
├── .nojekyll              # Pour GitHub Pages
├── README.md              
├── SECURITE.md            # Documentation sécurité
└── nginx.conf             # Configuration nginx
```

## Technologies utilisées

- HTML5 - Structure sémantique
- CSS3 - Styles et animations
  - Variables CSS
  - Flexbox
  - Grid Layout
  - Media Queries (6 breakpoints)
  - Animations avancées
- JavaScript Vanilla - Interactivité
  - ES6+
  - DOM Manipulation
  - Intersection Observer API
  - SPA Navigation
- Font Awesome - Icônes
- GitHub Pages - Hébergement

## Installation et développement local

Clonez le dépôt :

```bash
git clone https://github.com/Mariusklz/Portfolio.git
cd Portfolio
```

Aucune installation de dépendances requise (projet vanilla).

Lancez un serveur local :

```bash
# Avec Python 3
python -m http.server 8000

# Avec Node.js
npx http-server
```

Accédez à http://localhost:8000

## Personnalisation

### Couleurs

Modifiez les variables CSS dans css/style.css :

```css
:root {
    --primary-color: #3498db;
    --secondary-color: #2c3e50;
    --accent-color: #e74c3c;
}
```

### Contenu

1. Informations personnelles : Modifiez le contenu dans index.html
2. Images : Ajoutez vos images dans assets/images/
3. Liens sociaux : Mettez à jour les URLs dans la section hero et footer
4. Projets : Remplacez les projets exemple par vos réalisations

### Formulaire de contact

Pour rendre le formulaire de contact fonctionnel, adaptez le code fetch dans js/main.js pour envoyer les données à votre backend.

## Responsive Design

Le portfolio est entièrement responsive et optimisé pour tous les appareils :

- Mobile XS : <= 360px
- Mobile S  : <= 480px
- Mobile M  : <= 600px
- Mobile L  : <= 768px
- Tablette  : <= 968px
- Desktop   : > 968px

## Déploiement sur GitHub Pages

Le site est automatiquement déployé via GitHub Pages.

Configuration : Settings > Pages > Deploy from branch main / root

Pour mettre à jour le site :

```bash
git add .
git commit -m "Mise à jour du portfolio"
git push origin main
```

## Contact

- LinkedIn: https://www.linkedin.com/in/marius-keltz-18a084257/
- GitHub: https://github.com/Mariusklz
- Email: Via le formulaire de contact du portfolio

## Licence

Copyright 2026 KELTZ Marius. Tous droits réservés.
