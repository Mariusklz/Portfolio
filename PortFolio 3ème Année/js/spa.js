// ===================================
// Single Page Application (SPA) Navigation System
// Charge dynamiquement les pages via AJAX
// ===================================

// Configuration des pages
const pages = {
    'home': {
        id: 'home-sections',
        file: null, // Page par défaut, déjà dans l'HTML
        title: 'Accueil - KELTZ Marius'
    },
    'competences': {
        id: 'page-content',
        file: 'competences.html',
        title: 'Compétences - KELTZ Marius'
    },
    'projets-sae': {
        id: 'page-content',
        file: 'projets.html',
        title: 'Projets SAE - KELTZ Marius'
    },
    'projets-perso': {
        id: 'page-content',
        file: 'perso.html',
        title: 'Projets Personnels - KELTZ Marius'
    },
    'projets-entreprise': {
        id: 'page-content',
        file: 'entreprise.html',
        title: 'Projets Entreprise - KELTZ Marius'
    },
    'annexes': {
        id: 'page-content',
        file: 'annexes.html',
        title: 'Annexes - KELTZ Marius'
    }
};

// Cache pour éviter de recharger les pages
const pageCache = {};

// État de l'application
let currentPage = 'home';
let isLoading = false;

// Initialisation
document.addEventListener('DOMContentLoaded', () => {
    // Charger la page depuis l'URL au démarrage
    const hash = window.location.hash.slice(1) || 'home';
    navigateToPage(hash);
    
    // Écouter les changements d'URL
    window.addEventListener('hashchange', () => {
        const hash = window.location.hash.slice(1) || 'home';
        navigateToPage(hash);
    });
    
    // Intercepter les clics sur tous les liens internes
    document.addEventListener('click', (e) => {
        const link = e.target.closest('a[href]');
        if (!link) return;
        
        const href = link.getAttribute('href');
        
        // Vérifier si c'est un lien interne vers une page
        if (href.endsWith('.html')) {
            e.preventDefault();
            const pageName = getPageNameFromFile(href);
            if (pageName) {
                window.location.hash = pageName;
            }
        }
        // Vérifier si c'est un lien avec #
        else if (href.startsWith('#') && !href.startsWith('#sae-')) {
            const section = href.slice(1);
            if (section === 'home' || section === 'about' || section === 'contact') {
                e.preventDefault();
                window.location.hash = section;
            }
        }
    });
});

// Obtenir le nom de la page depuis le nom du fichier
function getPageNameFromFile(filename) {
    const basename = filename.split('/').pop();
    
    if (basename === 'competences.html') return 'competences';
    if (basename === 'projets.html') return 'projets-sae';
    if (basename === 'perso.html') return 'projets-perso';
    if (basename === 'entreprise.html') return 'projets-entreprise';
    if (basename === 'annexes.html') return 'annexes';
    if (basename === 'index.html') return 'home';
    
    return null;
}

// Fonction principale de navigation
async function navigateToPage(pageName) {
    // Ne rien faire si on charge déjà
    if (isLoading) return;
    
    // Gérer les sections spéciales de la home
    if (pageName === 'home' || pageName === 'about' || pageName === 'contact') {
        showHomeSection(pageName);
        currentPage = pageName;
        updateActiveNav(pageName);
        return;
    }
    
    // Vérifier si la page existe
    if (!pages[pageName]) {
        window.location.hash = 'home';
        return;
    }
    
    // Ne rien faire si on est déjà sur cette page
    if (currentPage === pageName) return;
    
    // Charger et afficher la page
    await loadAndShowPage(pageName);
}

// Charger et afficher une page
async function loadAndShowPage(pageName) {
    const page = pages[pageName];
    if (!page || !page.file) return;
    
    isLoading = true;
    
    // Afficher un loader
    showLoader();
    
    try {
        // Récupérer le contenu (depuis le cache ou charger)
        let content;
        if (pageCache[pageName]) {
            content = pageCache[pageName];
        } else {
            content = await fetchPageContent(page.file);
            pageCache[pageName] = content;
        }
        
        // Masquer les sections home
        hideHomeSections();
        
        // Injecter le contenu avec DOMPurify pour sécurité XSS
        const contentContainer = document.getElementById('dynamic-content');
        if (contentContainer) {
            // Sanitize le contenu HTML avant injection
            const cleanContent = typeof DOMPurify !== 'undefined' 
                ? DOMPurify.sanitize(content, { 
                    ALLOWED_TAGS: ['a', 'abbr', 'b', 'blockquote', 'br', 'button', 'cite', 'code', 'dd', 'div', 'dl', 'dt', 'em', 'footer', 'h1', 'h2', 'h3', 'h4', 'h5', 'h6', 'header', 'i', 'img', 'li', 'nav', 'ol', 'p', 'pre', 'section', 'span', 'strong', 'table', 'tbody', 'td', 'th', 'thead', 'tr', 'ul', 'form', 'input', 'textarea', 'label', 'select', 'option'],
                    ALLOWED_ATTR: ['class', 'id', 'href', 'src', 'alt', 'title', 'target', 'rel', 'aria-label', 'type', 'name', 'value', 'placeholder', 'required', 'rows', 'style'],
                    ALLOW_DATA_ATTR: false
                }) 
                : content;
            
            contentContainer.innerHTML = cleanContent;
            contentContainer.classList.remove('hidden-page');
            contentContainer.classList.add('fade-in');
            
            // Retirer l'animation après qu'elle soit terminée
            setTimeout(() => {
                contentContainer.classList.remove('fade-in');
            }, 500);
        }
        
        // Mettre à jour le titre
        document.title = page.title;
        
        // Mettre à jour l'état
        currentPage = pageName;
        
        // Mettre à jour la navigation
        updateActiveNav(pageName);
        
        // Scroll en haut
        window.scrollTo({ top: 0, behavior: 'smooth' });
        
    } catch (error) {
        console.error('Erreur lors du chargement de la page:', error);
        alert('Erreur lors du chargement de la page. Redirection vers l\'accueil...');
        window.location.hash = 'home';
    } finally {
        hideLoader();
        isLoading = false;
    }
}

// Récupérer le contenu d'une page
async function fetchPageContent(filename) {
    const response = await fetch(filename);
    if (!response.ok) {
        throw new Error(`Erreur HTTP: ${response.status}`);
    }
    
    const html = await response.text();
    
    // Extraire seulement le contenu entre les sections (sans nav) mais avec footer
    const parser = new DOMParser();
    const doc = parser.parseFromString(html, 'text/html');
    
    // Récupérer toutes les sections sauf nav
    const sections = doc.querySelectorAll('section:not(.navbar)');
    let content = '';
    sections.forEach(section => {
        content += section.outerHTML;
    });
    
    // Ajouter le footer s'il existe
    const footer = doc.querySelector('footer');
    if (footer) {
        content += footer.outerHTML;
    }
    
    return content;
}

// Afficher/masquer les sections home
function showHomeSection(section) {
    // Masquer le contenu dynamique
    const dynamicContent = document.getElementById('dynamic-content');
    if (dynamicContent) {
        dynamicContent.classList.add('hidden-page');
    }
    
    // Afficher les sections home
    const homeSections = document.getElementById('home-sections');
    if (homeSections) {
        homeSections.classList.remove('hidden-page');
    }
    
    // Mettre à jour le titre
    document.title = 'Portfolio - KELTZ Marius';
    
    // Si c'est about ou contact, scroller vers la section
    if (section === 'about' || section === 'contact') {
        setTimeout(() => {
            const element = document.getElementById(section);
            if (element) {
                element.scrollIntoView({ behavior: 'smooth' });
            }
        }, 100);
    } else {
        window.scrollTo({ top: 0, behavior: 'smooth' });
    }
}

function hideHomeSections() {
    const homeSections = document.getElementById('home-sections');
    if (homeSections) {
        homeSections.classList.add('hidden-page');
    }
}

// Loader
function showLoader() {
    let loader = document.getElementById('spa-loader');
    if (!loader) {
        loader = document.createElement('div');
        loader.id = 'spa-loader';
        loader.className = 'spa-loader';
        loader.innerHTML = '<div class="spinner"></div>';
        document.body.appendChild(loader);
    }
    loader.style.display = 'flex';
}

function hideLoader() {
    const loader = document.getElementById('spa-loader');
    if (loader) {
        loader.style.display = 'none';
    }
}

// Mettre à jour la navigation active
function updateActiveNav(pageName) {
    // Retirer tous les actifs
    document.querySelectorAll('.nav-link').forEach(link => {
        link.classList.remove('active');
    });
    document.querySelectorAll('.dropdown').forEach(dropdown => {
        dropdown.classList.remove('active');
    });
    document.querySelectorAll('.dropdown-menu a').forEach(link => {
        link.classList.remove('active');
    });
    
    // Déterminer quel lien activer
    let linkSelector = '';
    
    if (pageName === 'home' || pageName === 'about' || pageName === 'contact') {
        linkSelector = `a[href="#${pageName}"]`;
    } else {
        // Pour les pages dans le dropdown
        if (pageName.startsWith('projets-')) {
            const dropdown = document.querySelector('.dropdown');
            if (dropdown) {
                dropdown.classList.add('active');
            }
        }
        
        // Trouver le lien correspondant dans le dropdown
        const file = pages[pageName]?.file;
        if (file) {
            linkSelector = `a[href="${file}"]`;
        }
    }
    
    if (linkSelector) {
        const activeLink = document.querySelector(linkSelector);
        if (activeLink) {
            activeLink.classList.add('active');
        }
    }
}

