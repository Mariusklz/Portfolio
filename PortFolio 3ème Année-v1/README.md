# Portfolio Personnel

Un portfolio moderne et responsive développé avec HTML5, CSS3 et JavaScript vanilla.

## 🚀 Aperçu

Ce portfolio présente vos compétences, projets et informations de contact de manière professionnelle et attrayante.

## ✨ Fonctionnalités

- 🎨 Design moderne et responsive
- 📱 Menu mobile hamburger
- 🎯 Navigation fluide avec smooth scroll
- 💫 Animations au scroll
- 📧 Formulaire de contact fonctionnel
- ⬆️ Bouton "Retour en haut"
- 🎭 Highlight de la section active dans la navigation
- 📊 Sections: Hero, À propos, Compétences, Projets, Contact

## 📁 Structure du projet

```
PortFolio/
│
├── index.html              # Page principale
├── css/
│   └── style.css          # Styles globaux
├── js/
│   └── main.js            # Scripts JavaScript
├── assets/
│   ├── images/            # Images du portfolio
│   │   ├── profile.jpg    # Photo de profil
│   │   ├── project1.jpg   # Image projet 1
│   │   ├── project2.jpg   # Image projet 2
│   │   └── project3.jpg   # Image projet 3
│   └── icons/             # Icônes et favicon
│       └── favicon.ico
├── .gitignore
└── README.md              # Ce fichier
```

## 🛠️ Technologies utilisées

- **HTML5** - Structure sémantique
- **CSS3** - Styles et animations
  - Variables CSS
  - Flexbox
  - Grid Layout
  - Media Queries
- **JavaScript** - Interactivité
  - ES6+
  - DOM Manipulation
  - Intersection Observer API

## 📦 Installation

1. Clonez ou téléchargez ce dépôt
2. Aucune installation de dépendances requise (projet vanilla)
3. Ouvrez `index.html` dans votre navigateur

```bash
# Si vous voulez utiliser un serveur local
# Avec Python 3
python -m http.server 8000

# Avec Node.js (si vous avez installé http-server)
npx http-server
```

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

## 📱 Responsive

Le portfolio est entièrement responsive et optimisé pour :
- 📱 Mobile (320px et plus)
- 📱 Tablette (768px et plus)
- 💻 Desktop (968px et plus)
- 🖥️ Large screens (1200px et plus)

## 🌐 Déploiement

### GitHub Pages

1. Poussez votre code sur GitHub
2. Allez dans Settings > Pages
3. Sélectionnez la branche `main` et le dossier racine
4. Votre site sera accessible à `https://username.github.io/repository-name`

### Netlify

1. Créez un compte sur [Netlify](https://netlify.com)
2. Glissez-déposez le dossier du projet
3. Votre site est déployé instantanément!

### Vercel

```bash
npm i -g vercel
vercel
```

## ✅ Checklist avant mise en ligne

- [ ] Remplacer tous les textes placeholder par votre contenu
- [ ] Ajouter vos vraies images
- [ ] Mettre à jour les liens sociaux
- [ ] Tester sur différents navigateurs
- [ ] Tester sur différents appareils
- [ ] Optimiser les images (compression)
- [ ] Ajouter votre favicon
- [ ] Configurer le formulaire de contact
- [ ] Vérifier l'accessibilité
- [ ] Optimiser le SEO (meta tags, etc.)

## 🎯 Améliorations futures possibles

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
