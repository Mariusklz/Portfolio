// easy.js

document.addEventListener("DOMContentLoaded", () => {
    const ongletsContainer = document.getElementById("onglets");
    const accueilCasesContainer = document.getElementById("accueil-cases");
    const docCasesContainer = document.getElementById("doc-cases");
    const liensCasesContainer = document.getElementById("liens-cases");
    const infoCasesContainer = document.getElementById("info-cases")

    const images = [
        { src: 'img/home.png', text: 'Accueil' },
        { src: 'img/phone.png', text: 'Listes' },
        { src: 'img/printer.png', text: 'Imprimante et scanners' },
        { src: 'img/ie.png', text: 'Liens rapides' },
        { src: 'img/file.png', text: 'Documentation' },
        { src: 'img/info.png', text: 'Info' }
    ];

    function afficherSection(section) {
        accueilCasesContainer.style.display = "none";
        docCasesContainer.style.display = "none";
        liensCasesContainer.style.display ="none";
        infoCasesContainer.style.display = "none";


        if (section === "Accueil") {
            loadExcelData(); // Charger les données d'accueil
            accueilCasesContainer.style.display = "flex";
        } else if (section === "Documentation") {
            loadExcelDataForDocumentation(); // Charger les données de documentation
            docCasesContainer.style.display = "flex";
        } else if (section === "Liens rapides") {
            loadExcelDataForLiens(); // Charger les données des Liens
            liensCasesContainer.style.display = "flex";
        } else if (section === "Info") {
            infoCasesContainer.style.display = "flex";
        }

    }

    images.forEach((item, i) => {
        const onglet = document.createElement("button");
        onglet.classList.add("onglet");

        const img = document.createElement("img");
        img.src = item.src;
        img.alt = item.text;

        onglet.appendChild(img);
        ongletsContainer.appendChild(onglet);

        onglet.addEventListener("click", () => {
            document.querySelectorAll(".onglet").forEach(el => el.classList.remove("active"));
            onglet.classList.add("active");
            document.getElementById("dynamic-text").textContent = ` - ${item.text}`;

            afficherSection(item.text);
        });

        if (i === 0) { // Active le premier onglet par défaut
            onglet.classList.add("active");
            document.getElementById("dynamic-text").textContent = ` - ${item.text}`;
            afficherSection(item.text);
        }
    });
});
