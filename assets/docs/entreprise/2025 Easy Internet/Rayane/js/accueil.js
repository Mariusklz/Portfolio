// accueil.js

function loadExcelData() {
    const accueilCasesContainer = document.getElementById("accueil-cases");
    accueilCasesContainer.innerHTML = ""; // Vide l'affichage pour éviter les doublons
    
    fetch('data/liens.xlsx')
        .then(response => response.arrayBuffer())
        .then(data => {
            const workbook = XLSX.read(data, { type: 'array' });
            const sheet = workbook.Sheets[workbook.SheetNames[0]];
            const jsonData = XLSX.utils.sheet_to_json(sheet);
            displayExcelData(jsonData);
        })
        .catch(error => console.error("Erreur lors du chargement du fichier Excel:", error));
}

function displayExcelData(data) {
    const accueilCasesContainer = document.getElementById("accueil-cases");
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
            window.location.href = Lien;
        });

        accueilCasesContainer.appendChild(caseDiv);
    });
}