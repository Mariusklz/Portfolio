document.addEventListener("DOMContentLoaded", () => {
    const carteTel = document.getElementById("carteSVG_Tel");
    const tableauContainer = document.getElementById("tableau-container");
    const BarreRecherche = document.getElementById("recherche-input");
    const buttonRecherche = document.getElementById("button-recherche");
    const BarreRecherche2 = document.getElementById("recherche-input2");
    const buttonRecherche2 = document.getElementById("button-recherche2");
    const PostalAdresse = document.getElementById("Postal&Adresse");
    const Titre = document.getElementById("Titre");
    const texteInfo = document.getElementById("TexteInfo");
    const buttonSites = document.querySelectorAll(".site_Tel");
    const depotLinks = document.querySelectorAll('[id^="Tel"]');

    let currentSearchType = "global";
    let currentSiteId = null;

    function toggleVisibility(showSearchForSites) {
        texteInfo.style.display = "flex";
        carteTel.style.display = "none";
        BarreRecherche.style.display = showSearchForSites ? "none" : "flex";
        buttonRecherche.style.display = showSearchForSites ? "none" : "flex";
        BarreRecherche2.style.display = showSearchForSites ? "flex" : "none";
        buttonRecherche2.style.display = showSearchForSites ? "flex" : "none";
    }
    buttonSites.forEach(bouton => {
        bouton.addEventListener("click", (event) => {
            event.preventDefault();
    
            const siteId = bouton.getAttribute("id");
            if (!siteId) {
                console.warn("Un bouton site n'a pas d'ID valide :", bouton);
                return;
            }
            
            console.log("Clic sur le site :", siteId);
            currentSearchType = "site";
            currentSiteId = siteId;
            toggleVisibility(true);
            chargerEtAfficherTableau("Sites", siteId);
        });
    });
    
    
    depotLinks.forEach(link => {
        link.addEventListener("click", event => {
            event.preventDefault();
            const depotId = link.getAttribute("id");
            if (!depotId) {
                console.warn("Un dépôt n'a pas d'ID valide :", link);
                return;
            }
            console.log("📦 Clic sur un dépôt", depotId);
            currentSearchType = "depot";
            currentSiteId = null;
            toggleVisibility(false);
            chargerEtAfficherTableau("Dépôts");
        });
    });
    

    function rechercher(query, siteId) {
        chargerEtAfficherTableau(query, siteId);
    }

    if (buttonRecherche) {
        buttonRecherche.addEventListener("click", () => rechercher(BarreRecherche.value.trim().toLowerCase(), null));
    }
    if (buttonRecherche2) {
        buttonRecherche2.addEventListener("click", () => rechercher(BarreRecherche2.value.trim().toLowerCase(), currentSiteId));
    }

    BarreRecherche.addEventListener("keydown", event => {
        if (event.key === "Enter") rechercher(BarreRecherche.value.trim().toLowerCase(), null);
    });

    BarreRecherche2.addEventListener("keydown", event => {
        if (event.key === "Enter") rechercher(BarreRecherche2.value.trim().toLowerCase(), currentSiteId);
    });

    function chargerEtAfficherTableau(query = "", siteId = null) {
        fetch("data/referentiel.xlsx")
            .then(response => response.arrayBuffer())
            .then(data => {
                let workbook = XLSX.read(data, { type: "array" });
                let sheetTel = workbook.Sheets["Tel"];
                if (!sheetTel) {
                    console.error("❌ Feuille 'Tel' introuvable.");
                    tableauContainer.innerHTML = "<p>Erreur : Données non disponibles.</p>";
                    return;
                }
                let jsonData = XLSX.utils.sheet_to_json(sheetTel, { defval: "" });

                if (siteId) {
                    jsonData = jsonData.filter(item => item["Site"]?.trim() === siteId);
                } else if (currentSearchType === "depot") {
                    jsonData = jsonData.filter(item => item["Site"] && item["Site"].length > 3);
                } else if (query) {
                    jsonData = jsonData.filter(item => Object.values(item).some(val => val.toString().toLowerCase().includes(query)));
                }

                const colonnesDesirees = ["Site", "Nom", "Prénom", "Fonction", "Téléphone", "Interne", "Mobile", "Abrégé"];
                jsonData = jsonData.map(item => {
                    let filteredItem = {};
                    colonnesDesirees.forEach(colonne => {
                        filteredItem[colonne] = item[colonne] || "";
                    });
                    filteredItem["NomPrénom"] = `${filteredItem["Nom"]} ${filteredItem["Prénom"]}`;
                    return filteredItem;
                });

                afficherTableau(jsonData);
            })
            .catch(error => console.error("❌ Erreur de chargement du fichier Excel:", error));
    }

    function afficherTableau(data) {
        if (!tableauContainer) {
            console.error("❌ Élément tableau-container introuvable !");
            return;
        }
        tableauContainer.innerHTML = data.length === 0 ? "<p>Aucune donnée trouvée.</p>" : "";
        if (data.length === 0) return;

        let tableHTML = "<table><tr>";
        const colonnes = { "Site": "tel-Site", "NomPrénom": "tel-NomPrenom", "Fonction": "tel-Fonction", "Téléphone": "tel-Téléphone", "Interne": "tel-Interne", "Mobile": "tel-Mobile", "Abrégé": "tel-Abrégé" };

        Object.keys(colonnes).forEach(key => {
            tableHTML += `<th class="${colonnes[key]}">${key === "NomPrénom" ? "Nom & Prénom" : key}</th>`;
        });
        tableHTML += "</tr>";

        data.forEach(row => {
            tableHTML += "<tr>";
            Object.keys(colonnes).forEach(key => {
                tableHTML += `<td class="${colonnes[key]}">${row[key] || ""}</td>`;
            });
            tableHTML += "</tr>";
        });

        tableHTML += "</table>";
        tableauContainer.innerHTML = tableHTML;
        tableauContainer.style.display = "flex";
        console.log("✅ Tableau mis à jour !");
    }
});