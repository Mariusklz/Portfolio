// doc.js

document.addEventListener("DOMContentLoaded", function () {
    loadExcelDataForDocumentation();
});

function loadExcelDataForDocumentation() {
    const docCasesContainer = document.getElementById("doc-cases");
    docCasesContainer.innerHTML = ""; // Vider l'affichage avant d'ajouter du contenu

    fetch('data/liens.xlsx')
        .then(response => response.arrayBuffer())
        .then(data => {
            const workbook = XLSX.read(data, { type: 'array' });

            // Vérifier si l'onglet "Documentation" existe
            const sheetName = "Documentation";
            if (!workbook.Sheets[sheetName]) {
                console.error(`L'onglet "${sheetName}" est introuvable dans le fichier Excel.`);
                docCasesContainer.innerHTML = "<p>Onglet 'Documentation' non trouvé.</p>";
                return;
            }

            const sheet = workbook.Sheets[sheetName];
            const jsonData = XLSX.utils.sheet_to_json(sheet);

            console.log("Données chargées depuis l'onglet Documentation :", jsonData); // Vérification console

            if (jsonData.length === 0) {
                docCasesContainer.innerHTML = "<p>Aucune donnée trouvée.</p>";
                return;
            }

            displayExcelDataForDocumentation(jsonData);
        })
        .catch(error => console.error("Erreur lors du chargement du fichier Excel :", error));
}

function displayExcelDataForDocumentation(data) {
    const docCasesContainer = document.getElementById("doc-cases");
    const categories = {};

    // Regrouper les données par catégorie
    data.forEach(item => {
        if (!categories[item.Categorie]) {
            categories[item.Categorie] = [];
        }
        categories[item.Categorie].push(item);
    });

    // Générer le HTML pour chaque catégorie et ses documents
    Object.keys(categories).forEach(category => {
        // Créer la catégorie
        const categoryDiv = document.createElement("div");
        categoryDiv.classList.add("doc-category");
        categoryDiv.textContent = category;
        docCasesContainer.appendChild(categoryDiv);

        // Conteneur des cases pour cette catégorie
        const casesContainer = document.createElement("div");
        casesContainer.classList.add("doc-cases-container");

        categories[category].forEach(item => {
            const caseDiv = document.createElement("div");
            caseDiv.classList.add("doc-case");
            caseDiv.textContent = item.Nom;
            caseDiv.addEventListener("click", () => {
                window.open(item.Lien, "_blank");
            });

            casesContainer.appendChild(caseDiv);
        });

        docCasesContainer.appendChild(casesContainer);
    });
}
