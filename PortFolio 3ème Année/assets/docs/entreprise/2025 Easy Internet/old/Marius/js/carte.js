document.addEventListener('DOMContentLoaded', function () {
    const carteTel = document.getElementById("carteSVG_tel");
    const cartePrint = document.getElementById("carteSVG_print");
    let selectedRegion = null;
    let selectedSite = null;

    // Gestion des clics sur une région
    document.querySelectorAll('.region').forEach(region => {
        region.addEventListener('click', function (event) {
            event.stopPropagation();

            if (selectedRegion === this) return;

            selectedRegion = this;
            selectedSite = null; // Réinitialiser le site sélectionné

            // Réinitialiser toutes les régions
            document.querySelectorAll('.region').forEach(r => {
                r.style.transform = "scale(1)";
                r.style.zIndex = "0";
                r.style.opacity = "1";
                r.style.pointerEvents = "auto";
                r.classList.remove("disabled");
            });

            // Désactiver les autres régions
            document.querySelectorAll('.region').forEach(r => {
                if (r !== this) {
                    r.style.opacity = "0.1";
                    r.style.pointerEvents = "none";
                    r.classList.add("disabled");
                }
            });

            // Réinitialiser tous les boutons Commerce
            document.querySelectorAll('.Commerce').forEach(r => {
                r.style.zIndex = "0";
                r.style.opacity = "0";
                r.style.pointerEvents = "none";
                r.classList.remove("disabled");
            });

            // Activer UNIQUEMENT les sites appartenant à cette région
            document.querySelectorAll('.site_Tel').forEach(button => {
                if (button.dataset.region === this.id) {
                    button.style.opacity = "0";
                    button.style.pointerEvents = "auto";
                    button.classList.remove("disabled");
                } else {
                    button.style.opacity = "1.1";
                    button.style.pointerEvents = "auto";
                    button.classList.add("disabled");
                }
            });

            // Zoom sur la région sélectionnée
            const rect = this.getBoundingClientRect();
            const centerX = window.innerWidth / 1.9;
            const centerY = window.innerHeight / 1.8;
            const reductionFactor = 0.55;
            const translateX = centerX - (rect.left + rect.width / 2);
            const translateY = centerY - (rect.top + rect.height / 2);

            this.style.transform = `scale(2) translate(${translateX * reductionFactor}px, ${translateY * reductionFactor}px)`;
            this.style.zIndex = "10";
            this.style.opacity = "1";
        });
    });

    // Gestion des clics sur un site
    document.querySelectorAll('.site_Tel').forEach(site => {
        site.addEventListener('click', function (event) {
            event.preventDefault(); // Empêche la redirection
            event.stopPropagation();

            if (selectedSite === this) return;

            selectedSite = this;

            // Désactiver tous les autres sites, sauf ceux de la même région
            document.querySelectorAll('.site_Tel').forEach(button => {
                if (button !== this && button.dataset.region === selectedRegion.id) {
                    button.style.opacity = "0.3"; // Sites non sélectionnés légèrement visibles
                    button.style.pointerEvents = "none"; // Désactiver clics
                    button.classList.add("disabled");
                }
            });

            this.style.opacity = "1"; // Garder le site sélectionné visible
            this.style.pointerEvents = "auto"; // Garder cliquable
        });
    });

    // Réinitialisation en cliquant hors d'une région ou d'un site
    document.addEventListener('click', function () {
        if (selectedRegion !== null || selectedSite !== null) {
            selectedRegion = null;
            selectedSite = null;

            // Réactiver toutes les régions
            document.querySelectorAll('.region').forEach(r => {
                r.style.transform = "scale(1)";
                r.style.zIndex = "0";
                r.style.opacity = "1";
                r.style.pointerEvents = "auto";
                r.classList.remove("disabled");
            });

            // Réactiver tous les sites
            document.querySelectorAll('.site_Tel').forEach(button => {
                button.style.opacity = "1";
                button.style.pointerEvents = "auto";
                button.classList.remove("disabled");
            });
            // Réactiver tous les sites
            document.querySelectorAll('.site_Tel').forEach(button => {
                button.style.opacity = "1";
                button.style.pointerEvents = "auto";
                button.classList.remove("disabled");
            });
        }
        // Réinitialiser tous les boutons Commerce
        setTimeout (() => {
            document.querySelectorAll('.Commerce').forEach(r => {
                r.style.zIndex = "0";
                r.style.opacity = "1";
                r.style.pointerEvents = "auto";
                r.classList.remove("disabled");
            }, "2000000000 second");
        });
    });
});
