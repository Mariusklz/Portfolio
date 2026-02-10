# Portfolio Personnel - KELTZ Marius

Portfolio professionnel présentant mes compétences, projets académiques et expériences en entreprise dans le domaine des Réseaux & Télécommunications.

🌐 **[Voir le portfolio en ligne](https://mariusklz.github.io/Portfolio/)**

## 🚀 Aperçu

Ce portfolio présente mon parcours en BUT Réseaux & Télécommunications, mes projets SAE, mes réalisations en entreprise chez Wienerberger, ainsi que mes compétences techniques.

## ✨ Fonctionnalités

- 🎨 Design moderne et responsive
- 📱 Menu mobile hamburger avec navigation fluide
- 🎯 Navigation SPA (Single Page Application)
- 💫 Animations au scroll
- 📧 Formulaire de contact avec validation
- ⬆️ Bouton "Retour en haut"
- 🎭 Highlight de la section active dans la navigation
- 📊 Sections complètes :
  - 🏠 Accueil
  - 👤 À propos
  - 💼 Compétences (Référentiel BUT R&T)
  - 🚀 Projets SAE (Semestres 1 à 5)
  - 💻 Projets Personnels
  - 🏢 Projets Entreprise (Wienerberger)
  - 📁 Annexes et justificatifs
  - 📧 Contact

## 📁 Structure du projet

```
Portfolio/
│
├── index.html              # Page d'accueil
├── competences.html        # Page compétences
├── projets.html           # Projets SAE
├── perso.html             # Projets personnels
├── entreprise.html        # Projets entreprise
├── annexes.html           # Annexes et justificatifs
├── merci.html             # Page de confirmation
│
├── css/
│   └── style.css          # Styles globaux
│
├── js/
│   ├── main.js            # Scripts principaux
│   └── spa.js             # Navigation SPA
│
├── assets/
│   ├── docs/              # Documents et justificatifs
│   │   ├── SAE 1.01/      # Documents SAE par semestre
│   │   ├── SAE 1.02/
│   │   ├── ...
│   │   └── entreprise/    # Documents projets entreprise
│   ├── images/            # Images du portfolio
│   └── icons/             # Icônes et favicon
│
├── .github/
│   └── copilot-instructions.md
├── .nojekyll              # Pour GitHub Pages
├── README.md              
├── SECURITE.md            # Documentation sécurité
└── nginx.conf             # Configuration nginx
```

## 🛠️ Technologies utilisées

- **HTML5** - Structure sémantique
- **CSS3** - Styles et animations
  - Variables CSS
  - Flexbox
  - Grid Layout
  - Media Queries
  - Animations avancées
- **JavaScript Vanilla** - Interactivité
  - ES6+
  - DOM Manipulation
  - Intersection Observer API
  - SPA Navigation
- **Font Awesome** - Icônes
- **GitHub Pages** - Hébergement

## 📦 Installation et développement local

1. Clonez le dépôt :
```bash
git clone https://github.com/Mariusklz/Portfolio.git
cd Portfolio
```

2. Aucune installation de dépendances requise (projet vanilla)

3. Ouvrez `index.html` dans votre navigateur ou utilisez un serveur local :

```bash
# Avec Python 3
python -m http.server 8000

# Avec Node.js (si vous avez installé http-server)
npx http-server
```

4. Accédez à `http://localhost:8000`

## 🎨 Personnalisation

### Couleurs

Modifiez les variables CSS dans `css/style.css` :

```css
:root {
    --primary-color: #3498db;      /* Couleur principale */
    --secondary-color: #2c3e50;    /* Couleur secondaire */
    --accent-color: #e74c3c;       /* Couleur d'accent */
    /* ... */
}
```

### Contenu

1. **Informations personnelles** : Modifiez le contenu dans `index.html`
2. **Images** : Ajoutez vos images dans `assets/images/`
3. **Liens sociaux** : Mettez à jour les URLs dans la section hero et footer
4. **Projets** : Remplacez les projets exemple par vos propres réalisations

### Formulaire de contact

Pour rendre le formulaire de contact fonctionnel, décommentez et adaptez le code fetch dans `js/main.js` (ligne ~165) pour envoyer les données à votre backend.

## 📱 Responsive Design

Le portfolio est entièrement responsive et optimisé pour tous les appareils :
- 📱 Mobile (320px et plus)
- 📱 Tablette (768px et plus)
- 💻 Desktop (968px et plus)
- 🖥️ Large screens (1200px et plus)

## 🌐 Déploiement sur GitHub Pages

Le site est automatiquement déployé via GitHub Pages :

**URL du portfolio :** [https://mariusklz.github.io/Portfolio/](https://mariusklz.github.io/Portfolio/)

### Configuration GitHub Pages

1. Le fichier `.nojekyll` est présent pour éviter le traitement Jekyll
2. Le fichier `index.html` est à la racine du projet
3. Tous les chemins sont relatifs et fonctionnent correctement
4. Configuration : Settings > Pages > Deploy from branch `main` / root

### Pour mettre à jour le site

```bash
git add .
git commit -m "Mise à jour du portfolio"
git push origin main
```

Le site sera automatiquement mis à jour sur GitHub Pages.

## 📧 Contact

- **LinkedIn:** [Marius KELTZ](https://www.linkedin.com/in/marius-keltz-18a084257/)
- **GitHub:** [@Mariusklz](https://github.com/Mariusklz)
- **Email:** Via le formulaire de contact du portfolio

## 📄 Licence

© 2026 KELTZ Marius. Tous droits réservés.

---

⭐ Si ce projet vous plaît, n'hésitez pas à mettre une étoile sur GitHub !

- [ ] Mode sombre / clair
- [ ] Internationalisation (i18n)
- [ ] Galerie de photos avec lightbox
- [ ] Blog intégré
- [ ] Animations plus avancées (GSAP, etc.)
- [ ] Backend pour le formulaire de contact
- [ ] Google Analytics
- [ ] PWA (Progressive Web App)

## 📝 Licence

Ce projet est libre d'utilisation pour votre portfolio personnel.

## 🤝 Contribution

Les suggestions et améliorations sont les bienvenues! N'hésitez pas à ouvrir une issue ou une pull request.

## 📧 Contact

Pour toute question, contactez-moi via le formulaire de contact du portfolio.

---

Fait avec ❤️ et ☕
