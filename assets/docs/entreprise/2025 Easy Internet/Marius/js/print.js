document.addEventListener("DOMContentLoaded", () => {
    // Récupération des éléments HTML
    const cartePrint = document.getElementById("carteSVG_Print");
    const tableauContainer = document.getElementById("tableau-container");
    const BarreRecherche = document.getElementById("recherche-input");
    const buttonRecherche = document.getElementById("button-recherche");
    const PostalAdresse = document.getElementById("Postal&Adresse");
    const Titre = document.getElementById("Titre");
    const texteInfo = document.getElementById("TexteInfo");
    const buttonSites = document.querySelectorAll(".site_Print");
    const depotLinks = document.querySelectorAll('[id^="Print"]');

    let isTableauVisible = false;
    // Gestion des clics sur les sites
    buttonSites.forEach(bouton => {
        bouton.addEventListener("click", event => {
            event.preventDefault();
            const siteId = bouton.id;
            console.log("Clic sur le site :", siteId);

            
            texteInfo.style.display = "flex";
            cartePrint.style.display = "none";
            chargerEtAfficherTableau("Sites", siteId);
        });
    });

    // Gestion des clics sur les dépôts
    depotLinks.forEach(link => {
        link.addEventListener("click", event => {
            event.preventDefault();
            console.log("Clic sur Dépôt détecté !");

            cartePrint.style.display = "none";
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

            cartePrint.style.display = "none";
            PostalAdresse.style.display = "none";
            Titre.style.display = "none";
            texteInfo.style.display = "flex";

            chargerEtAfficherTableau();
        });
    }

    
    /**
     * Fonction pour charger et afficher le tableau des imprimantes par site
     */
    function chargerEtAfficherTableau(type, siteId = null, categorie = null) {
        fetch("data/referentiel.xlsx")
            .then(response => response.arrayBuffer())
            .then(data => {
                let workbook = XLSX.read(data, { type: "array" });
                console.log("Feuilles disponibles :", workbook.SheetNames); // Vérification des feuilles

                // 🔹 RÉCUPÉRER L'ONGLET "Sites" POUR LE TITRE ET L'ADRESSE
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

                // 🔹 RÉCUPÉRER L'ONGLET "Imp" POUR AFFICHER LE TABLEAU
                let sheetImp = workbook.Sheets["Imp"];
                if (!sheetImp) {
                    console.error("Erreur : La feuille 'Imp' n'existe pas !");
                    return;
                }

                
                

                let jsonData = XLSX.utils.sheet_to_json(sheetImp, { defval: "" });
                const termesExclus = ["Structure", "Toiture", "Façade", "Prescription", "Divers", "Chantiers"]; 
                console.log("Données extraites du fichier Excel :", jsonData); // Vérification des données brutes

                // Liste des colonnes désirées
                const colonnesDesirees = ["Site", "Place", "Type", "NomTechnique", "Modèle", "Actions"];

                // 🔹 FILTRAGE PAR SITE
                if (type === "Sites" && siteId) {
                    jsonData = jsonData.filter(item => item["Site"] && item["Site"].trim() === siteId);
                } else if (type === "Dépôts") {
                    jsonData = jsonData.filter(item => item["Site"] && item["Site"].length > 3 && !termesExclus.some(terme => item["Site"].includes(terme)));
                }

                // Formatage des données avec les colonnes sélectionnées
                jsonData = jsonData.map(item => {
                    let filteredItem = {};
                    colonnesDesirees.forEach(colonne => {
                        filteredItem[colonne] = item[colonne] || "";
                    });
                    return filteredItem;
                });

                afficherTableau(jsonData);
            })
            .catch(error => console.error("Erreur de chargement du fichier Excel :", error));
    }

    /**
     * Fonction générique d'affichage du tableau
     */
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

        // Définition des classes CSS pour les colonnes
        const colonneClasses = {
            "Site": "prt-Site",
            "Place": "prt-Place",
            "Type": "prt-Type",
            "NomTechnique": "prt-NomTechnique",
            "Modèle": "prt-Modèle",
            "Actions": "prt-Actions",
        };

        // En-têtes du tableau
        tableHTML += "<tr>";
        Object.keys(colonneClasses).forEach(key => {
            tableHTML += `<th class="${colonneClasses[key]}">${key}</th>`;
        });
        tableHTML += "</tr>";

        // Lignes du tableau
        data.forEach(row => {
            tableHTML += "<tr>";
            Object.keys(colonneClasses).forEach(key => {
                let value = row[key] || "";
                 // Ajout d'un bouton dans la colonne 'Actions'
                if (key === "Actions") {
                    // Ajouter un bouton "Installer Imprimante" ou un autre type d'action
                    tableHTML += `<td class="${colonneClasses[key]}"><button onclick="handleActionClick('${row['Site']}')">Installer Imprimante</button></td>`;
                } else {
                    tableHTML += `<td class="${colonneClasses[key]}">${value}</td>`;
                }
            });
            tableHTML += "</tr>";
        });

        tableHTML += "</table>";
        tableauContainer.innerHTML = tableHTML;
        tableauContainer.style.display = "block"; // S'assurer que le tableau est visible

        console.log("✅ Tableau affiché !");
    };
});