// easy.js

document.addEventListener("DOMContentLoaded", () => {
    const ongletsContainer = document.getElementById("onglets");
    const accueilCasesContainer = document.getElementById("accueil-cases");
    const docCasesContainer = document.getElementById("doc-cases");
    const liensCasesContainer = document.getElementById("liens-cases");
    const carteTel = document.getElementById("carteSVG_Tel"); // carteTelTel
    const cartePrint = document.getElementById("carteSVG_Print"); // carteTelTel
    const tableauContainer = document.getElementById("tableau-container"); // Tableau
    const BarreRecherche = document.getElementById("recherche-input"); // Bouton Recherche
    const buttonRecherche = document.getElementById("button-recherche"); // Bouton Recherche
    const BarreRecherche2 = document.getElementById("recherche-input2"); // Bouton Recherche
    const buttonRecherche2 = document.getElementById("button-recherche2"); // Bouton Recherche
    const PostalAdresse = document.getElementById("Postal&Adresse");
    const Titre = document.getElementById("Titre");
    const texteInfo = document.getElementById("TexteInfo");
    const LogoAffichage = document.getElementById("")
    

    const images = [
        { src: 'img/home.png', text: 'Accueil' },
        { src: 'img/phone.png', text: 'Listes téléphoniques' },
        { src: 'img/printer.png', text: 'Imprimante et scanners' },
        { src: 'img/ie.png', text: 'Liens rapides' },
        { src: 'img/file.png', text: 'Documentation' },
        { src: 'img/info.png', text: 'Info' }
    ];

    function afficherSection(section) {
        accueilCasesContainer.style.display = "none";
        docCasesContainer.style.display = "none";
        liensCasesContainer.style.display="none";

        recherche-container
        carteTel.style.display ="none";
        cartePrint.style.display ="none";
        tableauContainer.style.display = "none";
        BarreRecherche.style.display = "none";
        buttonRecherche.style.display = "none";
        BarreRecherche2.style.display = "none";
        buttonRecherche2.style.display = "none";
        PostalAdresse.style.display = "none";
        Titre.style.display = "none";
        texteInfo.style.display = "none";

        if (section === "Accueil") {
            accueilCasesContainer.style.display = "flex";
        } else if (section === "Listes téléphoniques") {
            carteTel.style.display = "flex";
            tableauContainer.style.display = "none";
            BarreRecherche.style.display = "flex";
            buttonRecherche.style.display = "flex";
            PostalAdresse.style.display = "none";
            Titre.style.display = "none";
            // Cibler la première occurrence de l'élément avec la classe 'content-Recherche'
            const contentRecherche = document.querySelector("body");
            if (contentRecherche) {
                // Ajouter cette ligne pour mettre 'content-Recherche' en rouge si l'élément existe
                contentRecherche.style.backgroundColor = "#E5E5E5";
            } else {
                console.error("Erreur : L'élément avec la classe 'content-Recherche' n'a pas été trouvé !");
            }
        } else if (section === "Imprimante et scanners") {
            cartePrint.style.display = "flex";
            tableauContainer.style.display = "none";
            BarreRecherche.style.display = "flex";
            buttonRecherche.style.display = "flex";

            // Cibler la première occurrence de l'élément avec la classe 'content-Recherche'
            const contentRecherche = document.querySelector("body");
            if (contentRecherche) {
                // Ajouter cette ligne pour mettre 'content-Recherche' en rouge si l'élément existe
                contentRecherche.style.backgroundColor = "#E5E5E5";
            } else {
                console.error("Erreur : L'élément avec la classe 'content-Recherche' n'a pas été trouvé !");
            }
        } else if (section === "Documentation") {
            loadExcelDataForDocumentation(); // Charger les données de documentation
            docCasesContainer.style.display = "flex";
        } else if (section === "Liens rapides") {
            loadExcelDataForLiens(); // Charger les données des Liens
            liensCasesContainer.style.display = "flex";
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
    }
)},
)
