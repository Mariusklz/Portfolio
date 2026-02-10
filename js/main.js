// ===================================
// Navigation Mobile Menu Toggle
// ===================================
const hamburger = document.getElementById('hamburger');
const navMenu = document.getElementById('nav-menu');

if (hamburger && navMenu) {
    hamburger.addEventListener('click', () => {
        navMenu.classList.toggle('active');
        
        // Animation du hamburger
        const bars = hamburger.querySelectorAll('.bar');
        bars[0].style.transform = navMenu.classList.contains('active') 
            ? 'rotate(-45deg) translate(-5px, 6px)' 
            : 'none';
        bars[1].style.opacity = navMenu.classList.contains('active') ? '0' : '1';
        bars[2].style.transform = navMenu.classList.contains('active') 
            ? 'rotate(45deg) translate(-5px, -6px)' 
            : 'none';
    });

    // Fermer le menu lors du clic sur un lien
    const navLinks = document.querySelectorAll('.nav-link');
    navLinks.forEach(link => {
        link.addEventListener('click', () => {
            navMenu.classList.remove('active');
            const bars = hamburger.querySelectorAll('.bar');
            bars[0].style.transform = 'none';
            bars[1].style.opacity = '1';
            bars[2].style.transform = 'none';
        });
    });
}

// ===================================
// Dropdown Menu for Mobile
// ===================================
const dropdowns = document.querySelectorAll('.dropdown');

dropdowns.forEach(dropdown => {
    const dropdownToggle = dropdown.querySelector('.dropdown-toggle');
    
    if (dropdownToggle) {
        dropdownToggle.addEventListener('click', (e) => {
            // Sur mobile, empêcher le lien par défaut et toggle le menu
            if (window.innerWidth <= 768) {
                e.preventDefault();
                dropdown.classList.toggle('active');
                
                // Fermer les autres dropdowns
                dropdowns.forEach(otherDropdown => {
                    if (otherDropdown !== dropdown) {
                        otherDropdown.classList.remove('active');
                    }
                });
            }
        });
    }
});

// ===================================
// Navbar Scroll Effect
// ===================================
const navbar = document.getElementById('navbar');
let lastScroll = 0;

window.addEventListener('scroll', () => {
    const currentScroll = window.pageYOffset;
    
    // Ajouter une ombre à la navbar lors du scroll
    if (currentScroll > 0) {
        navbar.style.boxShadow = '0 4px 6px rgba(0, 0, 0, 0.1)';
    } else {
        navbar.style.boxShadow = '0 2px 4px rgba(0, 0, 0, 0.1)';
    }
    
    lastScroll = currentScroll;
});

// ===================================
// Active Link Highlight
// ===================================
const sections = document.querySelectorAll('section[id]');

function highlightNavLink() {
    const scrollY = window.pageYOffset;
    
    sections.forEach(section => {
        const sectionHeight = section.offsetHeight;
        const sectionTop = section.offsetTop - 100;
        const sectionId = section.getAttribute('id');
        const navLink = document.querySelector(`.nav-link[href="#${sectionId}"]`);
        
        if (navLink) {
            if (scrollY > sectionTop && scrollY <= sectionTop + sectionHeight) {
                navLink.style.color = 'var(--primary-color)';
            } else {
                navLink.style.color = 'var(--text-color)';
            }
        }
    });
}

window.addEventListener('scroll', highlightNavLink);

// ===================================
// Scroll to Top Button
// ===================================
const scrollTopBtn = document.getElementById('scroll-top');

if (scrollTopBtn) {
    window.addEventListener('scroll', () => {
        if (window.pageYOffset > 300) {
            scrollTopBtn.classList.add('visible');
        } else {
            scrollTopBtn.classList.remove('visible');
        }
    });

    scrollTopBtn.addEventListener('click', () => {
        window.scrollTo({
            top: 0,
            behavior: 'smooth'
        });
    });
}

// ===================================
// Contact Form Handling
// ===================================
const contactForm = document.getElementById('contact-form');

if (contactForm) {
    contactForm.addEventListener('submit', (e) => {
        // Ne pas empêcher la soumission par défaut pour FormSubmit
        // Juste valider les champs avant l'envoi
        
        const formData = new FormData(contactForm);
        const name = formData.get('name');
        const email = formData.get('email');
        const message = formData.get('message');
        
        // Validation simple
        if (!name || !email || !message) {
            e.preventDefault();
            showNotification('Veuillez remplir tous les champs obligatoires.', 'error');
            return;
        }
        
        // Validation email
        if (!isValidEmail(email)) {
            e.preventDefault();
            showNotification('Veuillez entrer une adresse email valide.', 'error');
            return;
        }
        
        // Si tout est OK, laisser le formulaire se soumettre normalement
        // FormSubmit gérera l'envoi
        showNotification('Envoi en cours...', 'info');
    });
}

// ===================================
// Utility Functions
// ===================================

// Validation d'email
function isValidEmail(email) {
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    return emailRegex.test(email);
}

// Afficher une notification
function showNotification(message, type = 'info') {
    // Créer l'élément de notification
    const notification = document.createElement('div');
    notification.className = `notification notification-${type}`;
    notification.textContent = message;
    
    // Styles inline pour la notification
    notification.style.cssText = `
        position: fixed;
        top: 100px;
        right: 20px;
        padding: 1rem 1.5rem;
        border-radius: 5px;
        color: white;
        font-weight: 500;
        z-index: 9999;
        animation: slideIn 0.3s ease;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    `;
    
    // Couleur selon le type
    if (type === 'success') {
        notification.style.backgroundColor = '#27ae60';
    } else if (type === 'error') {
        notification.style.backgroundColor = '#e74c3c';
    } else {
        notification.style.backgroundColor = '#3498db';
    }
    
    document.body.appendChild(notification);
    
    // Retirer la notification après 3 secondes
    setTimeout(() => {
        notification.style.animation = 'slideOut 0.3s ease';
        setTimeout(() => {
            notification.remove();
        }, 300);
    }, 3000);
}

// Ajouter les animations CSS pour les notifications
const style = document.createElement('style');
style.textContent = `
    @keyframes slideIn {
        from {
            transform: translateX(400px);
            opacity: 0;
        }
        to {
            transform: translateX(0);
            opacity: 1;
        }
    }
    
    @keyframes slideOut {
        from {
            transform: translateX(0);
            opacity: 1;
        }
        to {
            transform: translateX(400px);
            opacity: 0;
        }
    }
`;
document.head.appendChild(style);

// ===================================
// Smooth Scrolling (pour les navigateurs plus anciens)
// ===================================
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function(e) {
        const href = this.getAttribute('href');
        
        // Ne pas empêcher le comportement par défaut si c'est juste "#"
        if (href === '#') return;
        
        // Ne traiter que les ancres de la page actuelle (pas les liens inter-pages comme "annexes.html#sae-101")
        const fullHref = this.getAttribute('href');
        if (fullHref.includes('.html#')) {
            // C'est un lien vers une ancre sur une autre page, laisser le navigateur gérer
            return;
        }
        
        e.preventDefault();
        const target = document.querySelector(href);
        
        if (target) {
            const offsetTop = target.offsetTop - 70; // 70px pour la navbar
            
            window.scrollTo({
                top: offsetTop,
                behavior: 'smooth'
            });
        }
    });
});

// ===================================
// Animation on Scroll (Optional)
// ===================================
function animateOnScroll() {
    const elements = document.querySelectorAll('.skill-card, .project-card');
    
    const observer = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.style.opacity = '1';
                entry.target.style.transform = 'translateY(0)';
            }
        });
    }, {
        threshold: 0.1
    });
    
    elements.forEach(element => {
        element.style.opacity = '0';
        element.style.transform = 'translateY(20px)';
        element.style.transition = 'opacity 0.6s ease, transform 0.6s ease';
        observer.observe(element);
    });
}

// Initialiser l'animation au chargement de la page
window.addEventListener('load', () => {
    animateOnScroll();
});

// ===================================
// Console Message
// ===================================
console.log('%c👋 Bienvenue sur mon portfolio!', 'color: #3498db; font-size: 20px; font-weight: bold;');
console.log('%cSi vous voyez ce message, c\'est que vous êtes curieux! 🔍', 'color: #2c3e50; font-size: 14px;');

// ===================================
// Validation et Sécurité du Formulaire de Contact
// ===================================
const contactFormSecure = document.getElementById('contact-form');
if (contactFormSecure) {
    // Rate limiting côté client (basique)
    let lastSubmitTime = 0;
    const SUBMIT_COOLDOWN = 5000; // 5 secondes entre chaque soumission

    contactFormSecure.addEventListener('submit', function(e) {
        const currentTime = Date.now();
        const formError = document.getElementById('form-error');
        const submitBtn = document.getElementById('submit-btn');

        // Rate limiting
        if (currentTime - lastSubmitTime < SUBMIT_COOLDOWN) {
            e.preventDefault();
            showError('Veuillez attendre quelques secondes avant de renvoyer le formulaire.');
            return false;
        }

        // Vérifier le honeypot
        const honeypot = this.querySelector('[name="_honey"]').value;
        if (honeypot !== '') {
            e.preventDefault();
            // Ne pas afficher d'erreur pour ne pas alerter les bots
            return false;
        }

        // Récupérer et valider les champs
        const name = document.getElementById('contact-name').value.trim();
        const email = document.getElementById('contact-email').value.trim();
        const subject = document.getElementById('contact-subject').value.trim();
        const message = document.getElementById('contact-message').value.trim();

        // Validation du nom
        if (name.length < 2 || name.length > 100) {
            e.preventDefault();
            showError('Le nom doit contenir entre 2 et 100 caractères.');
            return false;
        }

        // Validation du nom (pas de caractères spéciaux dangereux)
        const namePattern = /^[A-Za-zÀ-ÿ\s\-']+$/;
        if (!namePattern.test(name)) {
            e.preventDefault();
            showError('Le nom ne peut contenir que des lettres, espaces, tirets et apostrophes.');
            return false;
        }

        // Validation de l'email
        const emailPattern = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
        if (!emailPattern.test(email) || email.length > 100) {
            e.preventDefault();
            showError('Veuillez entrer une adresse email valide.');
            return false;
        }

        // Validation du sujet
        if (subject.length > 200) {
            e.preventDefault();
            showError('Le sujet ne peut pas dépasser 200 caractères.');
            return false;
        }

        // Validation du message
        if (message.length < 10) {
            e.preventDefault();
            showError('Le message doit contenir au moins 10 caractères.');
            return false;
        }

        if (message.length > 5000) {
            e.preventDefault();
            showError('Le message ne peut pas dépasser 5000 caractères.');
            return false;
        }

        // Détecter les patterns suspects (injection SQL, XSS basique)
        const suspiciousPatterns = [
            /<script/i,
            /javascript:/i,
            /on\w+\s*=/i,
            /<iframe/i,
            /\bSELECT\b.*\bFROM\b/i,
            /\bUNION\b.*\bSELECT\b/i,
            /\bDROP\b.*\bTABLE\b/i,
            /\bINSERT\b.*\bINTO\b/i,
            /\bUPDATE\b.*\bSET\b/i,
            /\bDELETE\b.*\bFROM\b/i
        ];

        const allContent = name + ' ' + email + ' ' + subject + ' ' + message;
        for (let pattern of suspiciousPatterns) {
            if (pattern.test(allContent)) {
                e.preventDefault();
                showError('Contenu suspect détecté. Veuillez vérifier votre message.');
                return false;
            }
        }

        // Tout est valide, on peut envoyer
        lastSubmitTime = currentTime;
        hideError();
        
        // Désactiver le bouton temporairement
        submitBtn.disabled = true;
        submitBtn.textContent = 'Envoi en cours...';
        
        // Réactiver après 5 secondes (au cas où)
        setTimeout(() => {
            submitBtn.disabled = false;
            submitBtn.textContent = 'Envoyer';
        }, 5000);

        return true;
    });

    // Fonctions d'affichage des erreurs
    function showError(message) {
        const formError = document.getElementById('form-error');
        if (formError) {
            formError.textContent = message;
            formError.style.display = 'block';
            formError.style.padding = '10px';
            formError.style.backgroundColor = '#fee';
            formError.style.border = '1px solid #e74c3c';
            formError.style.borderRadius = '5px';
        }
    }

    function hideError() {
        const formError = document.getElementById('form-error');
        if (formError) {
            formError.style.display = 'none';
        }
    }

    // Nettoyage en temps réel des inputs
    const inputs = contactFormSecure.querySelectorAll('input[type="text"], input[type="email"], textarea');
    inputs.forEach(input => {
        input.addEventListener('input', function() {
            // Supprimer les caractères dangereux en temps réel
            this.value = this.value.replace(/<|>|{|}|\[|\]|`/g, '');
        });
    });
}

// ===================================
// Filtrage des Projets par Niveau
// ===================================
// Note: Le code de filtrage a été déplacé en inline dans projets.html
// pour éviter les conflits de timing avec le chargement du DOM

/*
window.addEventListener('load', function() {
    const filterButtons = document.querySelectorAll('.filter-btn');
    const projectDetails = document.querySelectorAll('.project-detail');

    console.log('Boutons trouvés:', filterButtons.length);
    console.log('Projets trouvés:', projectDetails.length);

    if (filterButtons.length > 0 && projectDetails.length > 0) {
        // État des filtres actifs
        let activeFilters = [];

        filterButtons.forEach(button => {
            button.addEventListener('click', function(e) {
                e.preventDefault();
                const filter = button.getAttribute('data-filter');
                console.log('Clic sur:', filter);
                
                // Toggle l'état actif du bouton
                button.classList.toggle('active');
                
                // Mettre à jour la liste des filtres actifs
                if (button.classList.contains('active')) {
                    if (!activeFilters.includes(filter)) {
                        activeFilters.push(filter);
                    }
                } else {
                    activeFilters = activeFilters.filter(f => f !== filter);
                }
                
                console.log('Filtres actifs:', activeFilters);
                
                // Filtrer les projets
                filterProjects();
            });
        });
        
        function filterProjects() {
            projectDetails.forEach(project => {
                const projectLevel = project.getAttribute('data-level');
                
                // Si aucun filtre actif, afficher tous les projets
                if (activeFilters.length === 0) {
                    project.style.display = 'block';
                    project.style.opacity = '1';
                    project.style.transform = 'translateY(0)';
                } else {
                    // Afficher seulement si le projet correspond à un des filtres actifs
                    if (activeFilters.includes(projectLevel)) {
                        project.style.display = 'block';
                        project.style.opacity = '1';
                        project.style.transform = 'translateY(0)';
                    } else {
                        project.style.display = 'none';
                        project.style.opacity = '0';
                    }
                }
            });
            console.log('Filtrage terminé');
        }
    } else {
        console.error('Boutons ou projets non trouvés!');
    }
});
*/

// ===================================
// Gestion des sections déroulantes (Collapsibles)
// ===================================
// Fonction réutilisable pour initialiser les sections déroulantes
window.initCollapsibles = function() {
    console.log('=== INITIALISATION SECTIONS DEROULANTES ===');
    console.log('Timestamp:', new Date().toLocaleTimeString());
    
    // Chercher d'abord dans le contenu dynamique, puis dans tout le document
    const dynamicContent = document.getElementById('dynamic-content');
    const searchArea = (dynamicContent && !dynamicContent.classList.contains('hidden-page')) 
        ? dynamicContent 
        : document;
    
    console.log('Zone de recherche:', searchArea === document ? 'document entier' : 'contenu dynamique');
    console.log('searchArea est:', searchArea);
    
    const headers = searchArea.querySelectorAll('.collapsible-header');
    console.log('Nombre de headers collapsibles trouvés:', headers.length);
    
    // Lister tous les headers trouvés
    headers.forEach((h, i) => {
        const text = h.textContent.trim().substring(0, 50);
        console.log(`  Header ${i}: "${text}..."`);
    });
    
    if (headers.length === 0) {
        console.log('Aucun header collapsible trouvé');
        return;
    }
    
    headers.forEach(function(header, index) {
        // Éviter d'ajouter plusieurs fois le listener
        const alreadyInitialized = header.getAttribute('data-collapsible-init');
        if (alreadyInitialized === 'true') {
            console.log(`Header ${index} déjà initialisé, passage...`);
            return;
        }
        
        const headerText = header.querySelector('span') ? header.querySelector('span').textContent : header.textContent;
        console.log(`Initialisation header ${index}: "${headerText.trim().substring(0, 40)}"`);
        
        header.style.cursor = 'pointer';
        header.setAttribute('data-collapsible-init', 'true');
        
        console.log(`  → Ajout du listener de clic pour header ${index}`);
        
        header.onclick = function(e) {
            console.log(`\n╔═══════════════════════════════════════════════════════════════`);
            console.log(`║ >>> CLIC DETECTE sur header ${index}`);
            console.log(`╚═══════════════════════════════════════════════════════════════`);
            console.log('Timestamp:', new Date().toLocaleTimeString());
            console.log('Event:', e);
            console.log('This:', this);
            
            e.preventDefault();
            e.stopPropagation();
            
            const content = this.nextElementSibling;
            const icon = this.querySelector('.toggle-icon');
            
            console.log('Recherche du contenu...');
            console.log('  nextElementSibling:', content);
            console.log('  nextElementSibling tagName:', content ? content.tagName : 'NULL');
            console.log('  nextElementSibling className:', content ? content.className : 'NULL');
            
            if (!content) {
                console.error('❌ PAS DE CONTENU TROUVÉ !');
                console.log('Éléments enfants du parent:');
                Array.from(this.parentElement.children).forEach((child, idx) => {
                    console.log(`  ${idx}: ${child.tagName}.${child.className}`);
                });
                return;
            }
            
            if (!content.classList.contains('collapsible-content')) {
                console.warn('⚠️ Le nextElementSibling n\'est PAS un collapsible-content !');
                console.log('Classes:', content.className);
            }
            
            console.log('Icône toggle:', icon ? 'trouvée' : 'non trouvée');
            const isActive = content.classList.contains('active');
            console.log('État actuel:', isActive ? '✅ OUVERT' : '⬜ FERMÉ');
            
            if (isActive) {
                content.classList.remove('active');
                if (icon) icon.classList.remove('rotate');
                console.log('→ 🔽 FERMETURE effectuée');
            } else {
                content.classList.add('active');
                if (icon) icon.classList.add('rotate');
                console.log('→ 🔼 OUVERTURE effectuée');
            }
            
            console.log('Classes finales du contenu:', content.className);
            console.log('Max-height calculé:', window.getComputedStyle(content).maxHeight);
            console.log('Opacity calculée:', window.getComputedStyle(content).opacity);
            console.log('═══════════════════════════════════════════════════════════════\n');
        };
    });
    
    console.log('=== SECTIONS DEROULANTES INITIALISÉES ===\n');
};

// ===================================
// Filtrage des Projets par Niveau (BUT 1/2/3)
// ===================================
window.initProjectFilters = function() {
    console.log('\n========== INITIALISATION FILTRES PROJETS ==========');
    console.log('Timestamp:', new Date().toLocaleTimeString());
    
    // Chercher dans le contenu dynamique d'abord
    const dynamicContent = document.getElementById('dynamic-content');
    const searchArea = (dynamicContent && !dynamicContent.classList.contains('hidden-page')) 
        ? dynamicContent 
        : document;
    
    console.log('Zone de recherche:', searchArea === document ? 'document entier' : 'contenu dynamique');
    
    // Recherche des boutons
    const filterButtons = searchArea.querySelectorAll('.filter-btn');
    console.log('Nombre de boutons de filtre trouvés:', filterButtons.length);
    
    if (filterButtons.length === 0) {
        console.log('⚠️ Aucun bouton de filtre trouvé - Cette page ne contient probablement pas de filtres');
        return;
    }
    
    // Recherche des projets
    const projectDetails = searchArea.querySelectorAll('.project-detail');
    console.log('Nombre de projets trouvés:', projectDetails.length);
    
    if (projectDetails.length === 0) {
        console.warn('⚠️ Aucun projet trouvé - Impossible de filtrer');
        return;
    }
    
    console.log('✓ Boutons et projets trouvés, configuration...');
    
    // État des filtres actifs
    let activeFilters = [];
    
    // Configurer chaque bouton
    filterButtons.forEach(function(button, index) {
        // Éviter la double initialisation
        if (button.getAttribute('data-filter-init') === 'true') {
            console.log('Bouton', index, 'déjà initialisé');
            return;
        }
        
        button.setAttribute('data-filter-init', 'true');
        
        button.addEventListener('click', function(e) {
            console.log('\n╔════════════════════════════════════════╗');
            console.log('║  CLIC sur filtre:', this.getAttribute('data-filter').toUpperCase().padEnd(24), '║');
            console.log('╚════════════════════════════════════════╝');
            
            e.preventDefault();
            e.stopPropagation();
            
            const filter = this.getAttribute('data-filter');
            
            // Toggle le bouton
            this.classList.toggle('active');
            const isActive = this.classList.contains('active');
            
            console.log('Bouton:', isActive ? '✓ ACTIF' : '✗ INACTIF');
            
            // Mise à jour des filtres actifs
            if (isActive) {
                if (!activeFilters.includes(filter)) {
                    activeFilters.push(filter);
                }
            } else {
                activeFilters = activeFilters.filter(f => f !== filter);
            }
            
            console.log('Filtres actifs:', activeFilters.length > 0 ? activeFilters.join(', ') : 'AUCUN');
            
            // Appliquer le filtrage
            let visibleCount = 0;
            let hiddenCount = 0;
            
            projectDetails.forEach(function(project) {
                const level = project.getAttribute('data-level');
                
                if (activeFilters.length === 0) {
                    // Aucun filtre : tout afficher
                    project.style.display = 'block';
                    visibleCount++;
                } else {
                    // Afficher uniquement les projets correspondant aux filtres actifs
                    if (activeFilters.includes(level)) {
                        project.style.display = 'block';
                        visibleCount++;
                    } else {
                        project.style.display = 'none';
                        hiddenCount++;
                    }
                }
            });
            
            console.log('Résultat: ', visibleCount, 'visibles /', hiddenCount, 'cachés');
            console.log('═══════════════════════════════════════════\n');
        });
        
        console.log('  ✓ Bouton', index, 'configuré (' + button.textContent.trim() + ')');
    });
    
    console.log('✓ Filtres initialisés avec succès');
    console.log('========== FIN INITIALISATION FILTRES ==========\n');
};

// Initialiser au chargement de la page
document.addEventListener('DOMContentLoaded', function() {
    console.log('DOMContentLoaded déclenché');
    window.initCollapsibles();
    window.initProjectFilters();
});

// Si le DOM est déjà chargé (script chargé après le HTML), initialiser immédiatement
if (document.readyState === 'loading') {
    console.log('DOM en cours de chargement, attente de DOMContentLoaded');
} else {
    console.log('DOM déjà chargé, initialisation immédiate');
    window.initCollapsibles();
    window.initProjectFilters();
}
