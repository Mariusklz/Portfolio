// liens.js

document.addEventListener("DOMContentLoaded", function () {
    loadExcelDataForLiens();
});

function loadExcelDataForLiens() {
    const liensCasesContainer = document.getElementById("liens-cases");
    liensCasesContainer.innerHTML = ""; // Vider l'affichage avant d'ajouter du contenu

    fetch('data/liens.xlsx')
        .then(response => response.arrayBuffer())
        .then(data => {
            const workbook = XLSX.read(data, { type: 'array' });

            // Vérifier si l'onglet "Liens rapides" existe
            const sheetName = "Liens rapides";
            if (!workbook.Sheets[sheetName]) {
                console.error(`L'onglet "${sheetName}" est introuvable dans le fichier Excel.`);
                liensCasesContainer.innerHTML = "<p>Onglet 'Liens rapides' non trouvé.</p>";
                return;
            }

            const sheet = workbook.Sheets[sheetName];
            const jsonData = XLSX.utils.sheet_to_json(sheet);

            console.log("Données chargées depuis l'onglet 'Liens rapides' :", jsonData); // Vérification console

            if (jsonData.length === 0) {
                liensCasesContainer.innerHTML = "<p>Aucune donnée trouvée.</p>";
                return;
            }

            displayExcelDataForLiens(jsonData);
        })
        .catch(error => console.error("Erreur lors du chargement du fichier Excel :", error));
}

function displayExcelDataForLiens(data) {
    const liensCasesContainer = document.getElementById("liens-cases");
    const categories = {};

    // Regrouper les données par catégorie
    data.forEach(item => {
        if (!categories[item.Categorie]) {
            categories[item.Categorie] = [];
        }
        categories[item.Categorie].push(item);
    });

    // Générer le HTML pour chaque catégorie et ses liens
    Object.keys(categories).forEach(category => {
        // Créer la catégorie
        const categoryDiv = document.createElement("div");
        categoryDiv.classList.add("liens-category");
        categoryDiv.textContent = category;
        liensCasesContainer.appendChild(categoryDiv);

        // Conteneur des cases pour cette catégorie
        const casesContainer = document.createElement("div");
        casesContainer.classList.add("liens-cases-container");

        categories[category].forEach(item => {
            const caseDiv = document.createElement("div");
            caseDiv.classList.add("liens-case");
            caseDiv.textContent = item.Nom;
            caseDiv.addEventListener("click", () => {
                window.open(item.Lien, "_blank");
            });

            casesContainer.appendChild(caseDiv);
        });

        liensCasesContainer.appendChild(casesContainer);
    });
}
