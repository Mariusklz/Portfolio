document.addEventListener("DOMContentLoaded", () => {
    // Récupération des éléments HTML
    const carteTel = document.getElementById("carteSVG_Tel"); 
    const tableauContainer = document.getElementById("tableau-container"); 
    const BarreRecherche = document.getElementById("recherche-input"); 
    const buttonRecherche = document.getElementById("button-recherche"); 
    const buttonImprimante = document.getElementById("button-imprimante"); 
    const PostalAdresse = document.getElementById("Postal&Adresse");
    const Titre = document.getElementById("Titre");
    const texteInfo = document.getElementById("TexteInfo");
    const buttonSites = document.querySelectorAll(".site_Tel");
    const depotLinks = document.querySelectorAll('[id^="Tel"]');

    let isTableauVisible = false;

    // Boutons pour les différentes catégories
    const buttonCommerce = [
        { button: document.querySelector("a[href='Structure']"), categorie: "Structure" },
        { button: document.querySelector("a[href='Toiture']"), categorie: "Toiture" },
        { button: document.querySelector("a[href='Façade']"), categorie: "Façade" },
        { button: document.querySelector("a[href='Préscription']"), categorie: "Prescription" },
        { button: document.querySelector("a[href='Chantiers']"), categorie: "Chantiers" }
    ];

    // Gestion des clics sur les sites
    buttonSites.forEach(bouton => {
        bouton.addEventListener("click", () => {
            const siteId = bouton.id;
            console.log("Clic sur le site :", siteId);
            
            texteInfo.style.display = "flex";
            carteTel.style.display = "none"; // Cache la carteTel
            chargerEtAfficherTableau("Sites", siteId);
        });
    });

    // Gestion des clics sur les dépôts
    depotLinks.forEach(link => {
        link.addEventListener("click", event => {
            event.preventDefault();
            console.log("Clic sur Dépôt détecté !");
            
            carteTel.style.display = "none";
            BarreRecherche.style.display = "flex";
            texteInfo.style.display = "flex";

            chargerEtAfficherTableau("Dépôts");
            isTableauVisible = true;
        });
    });

    // Gestion du bouton de recherche
    if (buttonRecherche) {
        buttonRecherche.addEventListener("click", event => {
            event.preventDefault();
            console.log("Clic sur bouton recherche détecté !");

            carteTel.style.display = "none";
            PostalAdresse.style.display = "none";
            Titre.style.display = "none";
            texteInfo.style.display = "flex";

            chargerEtAfficherTableau();
        });
    }

    // Gestion du bouton imprimante
    if (buttonImprimante) {
        buttonImprimante.addEventListener("click", event => {
            event.preventDefault();
            console.log("Impression en cours...");
            window.print();
        });
    }

    // Gestion des clics pour les catégories (Commerce)
    buttonCommerce.forEach(({ button, categorie }) => {
        if (button) {
            button.addEventListener("click", event => {
                event.preventDefault();
                console.log(`Clic sur ${categorie} détecté !`);

                carteTel.style.display = "none";
                texteInfo.style.display = "flex";
                PostalAdresse.style.display = "none";
                isTableauVisible = true;

                // Met à jour le titre avec la catégorie
                Titre.textContent = categorie;
                Titre.style.display = "block";

                // Charge les données correspondantes
                chargerEtAfficherTableau("Commerce", null, categorie);
            });
        }
    });

    /**
     * Fonction pour charger et afficher le tableau
     */
    function chargerEtAfficherTableau(type, siteId = null, categorie = null) {
        fetch("data/referentiel.xlsx")
            .then(response => response.arrayBuffer())
            .then(data => {
                let workbook = XLSX.read(data, { type: "array" });
                let sheetSites = workbook.Sheets["Sites"];
                if (sheetSites) {
                    let jsonSites = XLSX.utils.sheet_to_json(sheetSites);
                    let site = jsonSites.find(item => item.ID == siteId);
                    if (site) {
                        Titre.textContent = site.Nom || "Nom non disponible";
                        PostalAdresse.textContent = `${site["Adresse"] || "Adresse non disponible"}, ${site["CodePostal"] || "Code Postal non disponible"}`;
                    } else if (type === "Dépôts") {
                        Titre.textContent = "Dépôts";
                        PostalAdresse.textContent = "";
                    }
                    Titre.style.display = "block";
                    PostalAdresse.style.display = "block";
                }

                let sheetTel = workbook.Sheets["Tel"];
                if (!sheetTel) {
                    console.error("La feuille 'Tel' est introuvable.");
                    tableauContainer.innerHTML = "<p>Erreur : Données non disponibles.</p>";
                    return;
                }

                let jsonData = XLSX.utils.sheet_to_json(sheetTel, { defval: "" });
                const termesExclus = ["Structure", "Toiture", "Façade", "Prescription", "Divers", "Chantiers"];

                if (type === "Sites" && siteId) {
                    jsonData = jsonData.filter(item => item["Site"] && item["Site"].trim() === siteId);
                } else if (type === "Dépôts") {
                    jsonData = jsonData.filter(item => item["Site"] && item["Site"].length > 3 && !termesExclus.some(terme => item["Site"].includes(terme)));
                } else if (type === "Commerce" && categorie) {
                    jsonData = jsonData.filter(item => item["Site"] && item["Site"].includes(categorie));
                }

                // Liste des colonnes désirées + fusion Nom & Prénom
                const colonnesDesirees = ["Site", "Nom", "Prénom", "Fonction", "Téléphone", "Interne", "Mobile", "Abrégé"];
    
                jsonData = jsonData.map(item => {
                    let filteredItem = {};
                    colonnesDesirees.forEach(colonne => {
                        filteredItem[colonne] = item[colonne] || "";
                    });
    
                    // Vérifier la colonne SST et ajouter le logo si nécessaire
                    if (item["SST"] === 1) {
                        filteredItem["NomPrénom"] = `${filteredItem["Nom"]} ${filteredItem["Prénom"]} <img src='./img/sst.png' alt='Logo' style='width: 20px; height: 20px;' />`.trim();
                    } else {
                        filteredItem["NomPrénom"] = `${filteredItem["Nom"]} ${filteredItem["Prénom"]}`.trim();
                    }
    
                    return filteredItem;
                });
    
                afficherTableau(jsonData);
            })
            .catch(error => console.error("Erreur de chargement du fichier Excel :", error));
    }

    // Fonction générique d'affichage du tableau
    const afficherTableau = (data) => {
        console.log("Données affichées :", data);
    
        if (!tableauContainer) {
            console.error("Erreur : élément #tableau-container introuvable !");
            return;
        }
    
        tableauContainer.innerHTML = "";
    
        if (data.length === 0) {
            tableauContainer.innerHTML = "<p>Aucune donnée trouvée.</p>";
            return;
        }
    
        let tableHTML = "<table>";
    
        // Définition des colonnes
        const colonneClasses = {
            "Site": "tel-Site",
            "NomPrénom": "tel-NomPrenom",
            "Fonction": "tel-Fonction",
            "Téléphone": "tel-Téléphone",
            "Interne": "tel-Interne",
            "Mobile": "tel-Mobile",
            "Abrégé": "tel-Abrégé"
        };
    
        // En-têtes du tableau
        tableHTML += "<tr>";
        Object.keys(colonneClasses).forEach(key => {
            let label = key === "NomPrénom" ? "Nom & Prénom" : key;
            tableHTML += `<th class="${colonneClasses[key]}">${label}</th>`;
        });
        tableHTML += "</tr>";
    
        // Lignes du tableau
        data.forEach(row => {
            tableHTML += "<tr>";
            Object.keys(colonneClasses).forEach(key => {
                let value = row[key] || "";
    
                // Pour la colonne 'NomPrénom', on accepte du HTML (comme l'image)
                if (key === "NomPrénom") {
                    tableHTML += `<td class="${colonneClasses[key]}" style="white-space: nowrap;">${value}</td>`;
                } else {
                    tableHTML += `<td class="${colonneClasses[key]}">${value}</td>`;
                }
            });
            tableHTML += "</tr>";
        });
    
        tableHTML += "</table>";
        tableauContainer.innerHTML = tableHTML;
        tableauContainer.style.display = "flex";
        buttonImprimante.style.display = "flex";
    
        console.log("✅ Tableau affiché !");
    };
});   
