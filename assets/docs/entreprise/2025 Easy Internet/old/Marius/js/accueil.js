const tableauContainer = document.getElementById("tableau-container");
const accueilCasesContainer = document.getElementById("accueil-cases");

function chargerDonneesEtAfficher() {
    fetch('data/liens.xlsx')
        .then(response => response.arrayBuffer())
        .then(data => {
            const workbook = XLSX.read(data, { type: 'array' });
            const sheet = workbook.Sheets[workbook.SheetNames[0]];
            const jsonData = XLSX.utils.sheet_to_json(sheet);
            afficherBoutons(jsonData);
        })
        .catch(error => console.error("Erreur lors du chargement du fichier Excel:", error));
}

function afficherBoutons(data) {
    accueilCasesContainer.innerHTML = ""; // Nettoyer les anciens boutons

    data.forEach(item => {
        const { Nom, Image, Lien } = item;
        const caseDiv = document.createElement("div");
        caseDiv.classList.add("accueil-case");

        const img = document.createElement("img");
        img.src = `img/${Image}`;
        img.alt = Nom;
        img.classList.add("case-image");

        caseDiv.appendChild(img);
        const name = document.createElement("p");
        name.textContent = Nom;
        caseDiv.appendChild(name);

        caseDiv.addEventListener("click", () => {
            if (Nom === "Liste SST") {
                console.log(`Bouton "${Nom}" cliqué !`);
                afficherTableauSST();
            } else {
                window.location.href = Lien;
            }
        });

        accueilCasesContainer.appendChild(caseDiv);
    });
}

function afficherTableauSST() {
    accueilCasesContainer.style.display = "none";
    fetch("data/referentiel.xlsx")
        .then(response => response.arrayBuffer())
        .then(data => {
            const workbook = XLSX.read(data, { type: "array" });
            const sheetTel = workbook.Sheets["Tel"];

            if (!sheetTel) {
                console.error("Feuille 'Tel' introuvable.");
                tableauContainer.innerHTML = "<p>Erreur : Données non disponibles.</p>";
                return;
            }

            let jsonData = XLSX.utils.sheet_to_json(sheetTel, { defval: "" });

            if (jsonData.length === 0) {
                tableauContainer.innerHTML = "<p>Aucune donnée trouvée.</p>";
                return;
            }

            const colonnesDesirees = ["Site", "Nom", "Prénom", "Fonction", "Téléphone", "Interne", "Mobile", "Abrégé"];
            
            // Filtrer uniquement les SST
            jsonData = jsonData
                .filter(item => item["SST"] === 1) // Garde uniquement les personnes SST
                .map(item => {
                    let filteredItem = {};
                    colonnesDesirees.forEach(colonne => {
                        filteredItem[colonne] = item[colonne] || "";
                    });

                    // Ajout de l'icône SST uniquement pour les SST
                    filteredItem["NomPrénom"] = `${filteredItem["Nom"]} ${filteredItem["Prénom"]} <img src='./img/sst.png' alt='SST' style='width: 20px; height: 20px;' />`;

                    return filteredItem;
                });

            afficherTableau(jsonData);
        })
        .catch(error => console.error("Erreur de chargement du fichier Excel :", error));
}

function afficherTableau(data) {
    if (data.length === 0) {
        tableauContainer.innerHTML = "<p>Aucune donnée SST trouvée.</p>";
        return;
    }

    tableauContainer.innerHTML = `
        <table>
            <tr>
                <th>Site</th>
                <th>Nom & Prénom</th>
                <th>Fonction</th>
                <th>Téléphone</th>
                <th>Interne</th>
                <th>Mobile</th>
                <th>Abrégé</th>
            </tr>
            ${data.map(row => `
                <tr>
                    <td>${row["Site"]}</td>
                    <td>${row["NomPrénom"]}</td>
                    <td>${row["Fonction"]}</td>
                    <td>${row["Téléphone"]}</td>
                    <td>${row["Interne"]}</td>
                    <td>${row["Mobile"]}</td>
                    <td>${row["Abrégé"]}</td>
                </tr>
            `).join('')}
        </table>
    `;

    tableauContainer.style.display = "flex";
}

// Charger et afficher les boutons au démarrage
chargerDonneesEtAfficher();
