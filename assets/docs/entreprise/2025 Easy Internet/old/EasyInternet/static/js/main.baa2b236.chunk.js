(window.webpackJsonp = window.webpackJsonp || []).push([
    [0], {
        100: function(e, t) {},
        101: function(e, t) {},
        199: function(e, t, a) {
            "use strict";
            a.r(t);
            var n = a(0),
                s = a.n(n),
                r = a(89),
                o = a.n(r),
                i = a(13),
                l = a(14),
                c = a(16),
                u = a(15),
                h = a(10),
                p = a(17),
                d = function(e) {
                    var t = e.selected ? " selected" : "";
                    return s.a.createElement("button", {
                        title: e.title,
                        className: "NavButton btn btn-light container".concat(t, " "),
                        type: "button",
                        onClick: function() {
                            if (e.id === 7) {
                                window.location.href = "carte.html";
                            } 
                            else {
                                return e.btnAction(e.id);
                            }
                        }
                    }, s.a.createElement("img", {
                        alt: "",
                        src: e.img
                    }))
                },
                m = [
                    [0, "Accueil", "home.png"],
                    [1, "Liste t\xe9l\xe9phonique", "phone.png"],
                    [2, "Imprimantes & Scanners", "printer.png"],
                    [3, "Liens rapides", "ie.png"],
                    [4, "Documentation", "file.png"],
                    [5, "Identité", "info.png"]
                    
                ],
                f = function(e) {
                    function t() {
                        return Object(i.a)(this, t), Object(c.a)(this, Object(u.a)(t).apply(this, arguments))
                    }
                    return Object(p.a)(t, e), Object(l.a)(t, [{
                        key: "generateButtons",
                        value: function() {
                            var e = this,
                                t = [];
                            return m.forEach(function(a) {
                                t.push(s.a.createElement("li", null, s.a.createElement(d, {
                                    id: a[0],
                                    title: a[1],
                                    selected: e.props.menu === a[0],
                                    btnAction: e.props.btnAction,
                                    img: "./icons/".concat(a[2])
                                })))
                            }), t
                        }
                    }, {
                        key: "render",
                        value: function() {
                            return s.a.createElement("nav", {
                                id: "NavBar"
                            }, s.a.createElement("ul", {
                                className: "list-unstyled components no-print"
                            }, this.generateButtons()))
                        }
                    }]), t

                    
                }(s.a.Component),
                v = a(22),
                b = a.n(v),
                S = a(58),
                g = a(90),
                E = a.n(g),
                y = {
                    name: "sites",
                    areas: []
                },
                I = {
                    name: "sites",
                    areas: []
                },
                N = function(e) {
                    function t(e) {
                        var a;
                        return Object(i.a)(this, t), (a = Object(c.a)(this, Object(u.a)(t).call(this, e))).state = {
                            hoveredArea: null
                        }, a.globalSearch = a.globalSearch.bind(Object(h.a)(a)), a
                    }
                    return Object(p.a)(t, e), Object(l.a)(t, [{
                        key: "loadImpMap",
                        value: function() {
                            y = {
                                name: "sites",
                                areas: []
                            }, this.props.sitesInfos.forEach(function(e) {
                                if (e.Coords && "Comm" !== e.ID) {
                                    var t = parseInt(e.Coords.split(",")[0]),
                                        a = parseInt(e.Coords.split(",")[1]),
                                        n = parseInt(e.Coords.split(",")[2]),
                                        s = parseInt(e.Coords.split(",")[3]);
                                    y.areas.push({
                                        id: e.ID,
                                        shape: "rect",
                                        coords: [t, a, n, s]
                                    })
                                }
                            })
                        }
                    }, {
                        key: "loadTelMap",
                        value: function() {
                            I = {
                                name: "sites",
                                areas: []
                            }, this.props.sitesInfos.forEach(function(e) {
                                if (e.Coords) {
                                    var t = parseInt(e.Coords.split(",")[0]),
                                        a = parseInt(e.Coords.split(",")[1]),
                                        n = parseInt(e.Coords.split(",")[2]),
                                        s = parseInt(e.Coords.split(",")[3]);
                                    I.areas.push({
                                        id: e.ID,
                                        shape: "rect",
                                        coords: [t, a, n, s]
                                    })
                                }
                            })
                        }
                    }, {
                        key: "getSiteInfos",
                        value: function(e) {
                            var t = void 0;
                            return this.props.sitesInfos.forEach(function(a) {
                                a.ID === e && (t = a)
                            }), t
                        }
                    }, {
                        key: "getTipPosition",
                        value: function(e) {
                            var t = (e.coords[0] + e.coords[2]) / 2,
                                a = (e.coords[1] + e.coords[3]) / 2;
                            return {
                                top: "".concat(a + 100, "px"),
                                left: "".concat(t, "px")
                            }
                        }
                    }, {
                        key: "setHoveredArea",
                        value: function(e, t) {
                            var a = null,
                                n = e.target.getBoundingClientRect(),
                                s = e.clientX - n.left,
                                r = e.clientY - n.top;
                            t.areas.forEach(function(e) {
                                s >= e.coords[0] && s <= e.coords[2] && r >= e.coords[1] && r <= e.coords[3] && (a = e)
                            }), this.setState({
                                hoveredArea: a
                            })
                        }
                    }, {
                        key: "globalSearch",
                        value: function(e) {
                            var t = (document.getElementById("searchBar") ? document.getElementById("searchBar").value : "").trim().toLowerCase();
                            (e && "Enter" === e.key || !e) && this.props.globalSearchHandler(t)
                        }
                    }, {
                        key: "getStyle",
                        value: function() {
                            var e = {};
                            return this.state.hoveredArea && (e = {
                                cursor: "pointer"
                            }), e
                        }
                    }, {
                        key: "onClick",
                        value: function() {
                            if (this.state.hoveredArea) {
                                var e = this.getSiteInfos(this.state.hoveredArea.id);
                                this.props.selectionHandler(e)
                            }
                        }
                    }, {
                        key: "render",
                        value: function() {
                            var e, t, a, n = this;
                            return this.state.hoveredArea && (e = this.getSiteInfos(this.state.hoveredArea.id)), 2 === this.props.menu ? (this.loadImpMap(), t = y, a = "./img/carteImp.png") : (this.loadTelMap(), t = I, a = "./img/carteTel.png"), s.a.createElement("div", {
                                className: "Map",
                                onMouseMove: function(e) {
                                    return n.setHoveredArea(e, t)
                                },
                                onClick: function() {
                                    return n.onClick()
                                },
                                style: Object(S.a)({}, this.getStyle())
                            }, s.a.createElement("div", {
                                className: "searchBar input-group mb-3 no-print"
                            }, s.a.createElement("div", {
                                className: "input-group-prepend"
                            }, s.a.createElement("button", {
                                className: "searchButton btn btn-outline-secondary",
                                type: "button",
                                onClick: function() {
                                    return n.globalSearch()
                                }
                            }, s.a.createElement("img", {
                                alt: "Rechercher",
                                src: "./icons/loupe.png"
                            }))), s.a.createElement("input", {
                                id: "searchBar",
                                type: "text",
                                className: "form-control",
                                placeholder: "Recherche globale...",
                                onKeyDown: function(e) {
                                    return n.globalSearch(e)
                                }
                            })), s.a.createElement(E.a, {
                                src: a
                            }), this.state.hoveredArea && s.a.createElement("span", {
                                className: "map_popup",
                                style: Object(S.a)({}, this.getTipPosition(this.state.hoveredArea))
                            }, s.a.createElement("div", {
                                className: "siteName"
                            }, e.Nom), s.a.createElement("div", null, e.Adresse), s.a.createElement("div", null, e.CodePostal), s.a.createElement("div", null, e.Standard)))
                        }
                    }]), t
                }(s.a.Component),
                k = function(e) {
                    function t(e) {
                        var a;
                        return Object(i.a)(this, t), (a = Object(c.a)(this, Object(u.a)(t).call(this, e))).state = {
                            searchResults: []
                        }, a.doSearch = a.doSearch.bind(Object(h.a)(a)), a.handleInput = a.handleInput.bind(Object(h.a)(a)), a.generateBat = a.generateBat.bind(Object(h.a)(a)), a
                    }
                    return Object(p.a)(t, e), Object(l.a)(t, [{
                        key: "componentWillMount",
                        value: function() {
                            this.doSearch()
                        }
                    }, {
                        key: "handleInput",
                        value: function(e) {
                            "Enter" === e.key && this.doSearch()
                        }
                    }, {
                        key: "banalizeString",
                        value: function(e) {
                            e = e.toLowerCase().trim();
                            for (var t = [
                                    ["\xe0"],
                                    ["\xe7"],
                                    ["\xe9", "\xe8", "\xea", "\xeb"],
                                    ["\xee", "\xef"],
                                    ["\xf1"],
                                    ["\xf4"],
                                    ["\xf9"]
                                ], a = ["a", "c", "e", "i", "n", "o", "u"], n = "", s = 0; s < e.length; s++) {
                                for (var r = e[s], o = 0; o < t.length; o++) t[o].indexOf(r) >= 0 && (r = a[o]);
                                n += r
                            }
                            return n
                        }
                    }, {
                        key: "doSearch",
                        value: function() {
                            var e, t = this,
                                a = this.props.data,
                                n = [];
                            if (this.props.initQuery) e = this.props.initQuery;
                            else {
                                var s = document.getElementById("searchBar") ? document.getElementById("searchBar").value : "";
                                e = this.banalizeString(s)
                            }
                            a.forEach(function(a) {
                                e.split(" ").forEach(function(e) {
                                    1 === t.props.menu && (t.banalizeString(a.Nom).indexOf(e) >= 0 || t.banalizeString(a.T\u00e9l\u00e9phone).indexOf(e) >= 0 || t.banalizeString(a.Mobile).indexOf(e) >= 0 || t.banalizeString(a.Fonction).indexOf(e) >= 0 || t.banalizeString(a.Interne).indexOf(e) >= 0 || t.banalizeString(a.Abr\u00e9g\u00e9).indexOf(e) >= 0 || t.banalizeString(a.Site).indexOf(e) >= 0 || t.banalizeString(a.SST).indexOf(e) >= 0) && n.push(a), 2 === t.props.menu && (a.Type.toLowerCase().indexOf(e) >= 0 || a.Emplacement.toLowerCase().indexOf(e) >= 0 || a.Nom.toLowerCase().indexOf(e) >= 0 || a.Mod\u00e8le.toLowerCase().indexOf(e) >= 0 || a.Site.toLowerCase().indexOf(e) >= 0) && n.push(a)
                                })
                            }), this.setState({
                                searchResults: n
                            })
                        }
                    }, {
                        key: "getTabHeaders",
                        value: function() {
                            var e = [];
                            return 2 === this.props.menu && (e.push(s.a.createElement("th", {
                                className: "imp-Site"
                            }, "Site")), e.push(s.a.createElement("th", {
                                className: "imp-Emplacement"
                            }, "Emplacement")), e.push(s.a.createElement("th", {
                                className: "imp-Type"
                            }, "Type")), e.push(s.a.createElement("th", {
                                className: "imp-Nom"
                            }, "Nom")), e.push(s.a.createElement("th", {
                                className: "imp-Mod\xe8le"
                            }, "Mod\xe8le")), e.push(s.a.createElement("th", {
                                className: "imp-Actions no-print"
                            }, "Actions"))), 1 === this.props.menu && (e.push(s.a.createElement("th", {
                                className: "tel-Site"
                            }, "Site")), e.push(s.a.createElement("th", {
                                className: "tel-Nom"
                            }, "Nom")), e.push(s.a.createElement("th", {
                                className: "tel-Fonction"
                            }, "Fonction")), e.push(s.a.createElement("th", {
                                className: "tel-T\xe9l\xe9phone"
                            }, "T\xe9l\xe9phone")), e.push(s.a.createElement("th", {
                                className: "tel-Interne"
                            }, "Interne")), e.push(s.a.createElement("th", {
                                className: "tel-Mobile"
                            }, "Mobile")), e.push(s.a.createElement("th", {
                                className: "tel-Abr\xe9g\xe9"
                            }, "Abr\xe9g\xe9"))), e
                        }
                    }, {
                        key: "getResultsRows",
                        value: function() {
                            var e = this,
                                t = [],
                                a = 2 === this.props.menu ? "imp-" : "tel-";
                            return this.state.searchResults.forEach(function(n) {
                                var r = [];
                                if (Object.keys(n).forEach(function(e) {
                                        if ("Id" !== e && "MF" !== e && "IP" !== e && "Serveur" !== e && "SST" !== e) {
                                            var t = [];
                                            "Nom" === e && n.SST && t.push(s.a.createElement("img", {
                                                className: "lst_icon",
                                                src: "./icons/sst.png",
                                                alt: ""
                                            })), r.push(s.a.createElement("td", {
                                                className: "".concat(a).concat(e)
                                            }, n[e], t))
                                        }
                                    }), 2 === e.props.menu) {
                                    var o = [],
                                        i = "Fax" === n.MF ? "Fax" : "imprimante";
                                    o.push(s.a.createElement("a", {
                                        className: "premierButton btn btn-light",
                                        role: "button",
                                        href: "installprinter:".concat(n.Serveur, "---").concat(n.Nom)
                                    }, "Installer ", i)), n.MF && "MF" === n.MF && o.push(s.a.createElement("a", {
                                        rel: "noopener noreferrer",
                                        target: "_blank",
                                        className: "secondButton btn btn-light",
                                        role: "button",
                                        href: "http://".concat(n.IP)
                                    }, "Documents scann\xe9s")), "Fax" === n.MF && o.push(s.a.createElement("a", {
                                        rel: "noopener noreferrer",
                                        target: "_blank",
                                        className: "secondButton btn btn-light",
                                        role: "button",
                                        href: "http://".concat(n.IP)
                                    }, "Journal transmissions")), r.push(s.a.createElement("td", {
                                        className: "".concat(a, "Actions no-print")
                                    }, o))
                                }
                                t.push(s.a.createElement("tr", {
                                    className: "result"
                                }, r))
                            }), t
                        }
                    }, {
                        key: "getAdresse",
                        value: function() {
                            var e = this.props.siteInfos.Adresse ? this.props.siteInfos.Adresse : "";
                            return this.props.siteInfos.CodePostal && (e += ", " + this.props.siteInfos.CodePostal), e
                        }
                    }, {
                        key: "getSearchHeader",
                        value: function() {
                            var e = [];
                            return e.push(s.a.createElement("h1", null, this.props.siteInfos.Nom)), e.push(s.a.createElement("h3", null, this.getAdresse())), e
                        }
                    }, {
                        key: "generateBat",
                        value: function(e) {
                            var t = e.PrintServer,
                                a = e.PrinterName,
                                n = "Fichier installation " + a + ".bat",
                                s = new Blob(["rundll32 printui.dll PrintUIEntry /in /n\\\\" + t + "\\" + a], {
                                    type: "text/bat"
                                });
                            if (window.navigator.msSaveOrOpenBlob) window.navigator.msSaveBlob(s, n);
                            else {
                                var r = window.document.createElement("a");
                                r.href = window.URL.createObjectURL(s), r.download = n, document.body.appendChild(r), r.click(), document.body.removeChild(r)
                            }
                        }
                    }, {
                        key: "render",
                        value: function() {
                            var e = this;
                            console.log(this.props.siteInfos);
                            var t, a = this.getTabHeaders(),
                                n = this.getResultsRows(),
                                r = [];
                            return "" !== this.props.siteInfos && (r = this.getSearchHeader(), this.getAdresse()), 1 === this.props.menu && (t = "* = Abr\xe9g\xe9 accessible sur tous les postes t\xe9l\xe9phoniques"), s.a.createElement("div", {
                                id: "ResultsWindow"
                            }, s.a.createElement("div", {
                                className: "results-container"
                            }, s.a.createElement("div", {
                                className: "searchHeader container"
                            }, r), s.a.createElement("div", {
                                className: "searchBar input-group mb-3 no-print"
                            }, s.a.createElement("div", {
                                className: "input-group-prepend"
                            }, s.a.createElement("button", {
                                title: "Lancer la recherche",
                                className: "searchButton btn btn-outline-secondary",
                                type: "button",
                                onClick: function() {
                                    return e.doSearch()
                                }
                            }, s.a.createElement("img", {
                                alt: "Rechercher",
                                src: "./icons/loupe.png"
                            }))), s.a.createElement("input", {
                                id: "searchBar",
                                type: "text",
                                className: "form-control",
                                placeholder: "Recherche...",
                                onKeyDown: this.handleInput
                            }), s.a.createElement("div", {
                                className: "input-group-prepend"
                            }, s.a.createElement("button", {
                                title: "Imprimer la liste actuelle",
                                className: "printButton btn btn-outline-secondary",
                                type: "button",
                                onClick: function() {
                                    return window.print()
                                }
                            }, s.a.createElement("img", {
                                alt: "Imprimer",
                                src: "./icons/printer.png"
                            })))), s.a.createElement("p", {
                                className: "info"
                            }, t), s.a.createElement("div", null, s.a.createElement("table", {
                                className: "resultTab"
                            }, s.a.createElement("thead", null, s.a.createElement("tr", null, a)), s.a.createElement("tbody", null, n)))))
                        }
                    }]), t
                }(s.a.Component),
                O = function(e, t) {
                    for (var a = !1, n = 0; n < e.length; n++) e[n] === t && (a = !0);
                    return a
                },
                C = function(e) {
                    for (var t = "", a = 0; a < e.length; a++) " " !== e[a] && (t += e[a]);
                    return t
                },
                j = function(e) {
                    function t(e) {
                        var a;
                        return Object(i.a)(this, t), (a = Object(c.a)(this, Object(u.a)(t).call(this, e))).state = {
                            selectedSiteID: void 0,
                            selectedSiteInfos: void 0,
                            refWorkbook: void 0,
                            sitesInfos: void 0,
                            globalSearchQuery: void 0,
                            loaded: !1,
                            depots: void 0,
                            nomades: void 0
                        }, a.handleSiteSelection = a.handleSiteSelection.bind(Object(h.a)(a)), a.handleGlobalSearch = a.handleGlobalSearch.bind(Object(h.a)(a)), a
                    }
                    return Object(p.a)(t, e), Object(l.a)(t, [{
                        key: "componentWillMount",
                        value: function() {
                            this.loadRef(), this.props.sst && this.setState({
                                selectedSiteID: "SST"
                            })
                        }
                    }, {
                        key: "componentDidUpdate",
                        value: function() {
                            !this.state.refWorkbook || this.state.nomades || this.state.loaded || this.getComm(), !this.state.refWorkbook || this.state.depots || this.state.loaded || this.getDepots(), this.state.refWorkbook && this.state.sitesInfos && this.state.depots && this.state.nomades && !this.state.loaded && this.setState({
                                loaded: !0
                            })
                        }
                    }, {
                        key: "componentWillReceiveProps",
                        value: function(e) {
                            e.resetMap && !this.props.resetMap && this.setState({
                                selectedSiteID: void 0
                            })
                        }
                    }, {
                        key: "getSiteInfos",
                        value: function(e) {
                            var t = void 0;
                            return this.state.sitesInfos.forEach(function(a) {
                                a.ID === e && (t = a)
                            }), t
                        }
                    }, {
                        key: "loadRef",
                        value: function() {
                            var e = this,
                                t = new XMLHttpRequest;
                            t.open("GET", "./data/referentiel.xlsx", !0), t.responseType = "arraybuffer", t.onload = function() {
                                var a = new Uint8Array(t.response),
                                    n = b.a.read(a, {
                                        type: "array"
                                    });
                                e.setState({
                                    refWorkbook: n
                                });
                                var s = e.getData("Sites");
                                e.setState({
                                    sitesInfos: s
                                })
                            }, t.send(null)
                        }
                    }, {
                        key: "getComm",
                        value: function() {
                            var e = this.state.refWorkbook,
                                t = b.a.utils.sheet_to_json(e.Sheets.Sites),
                                a = [];
                            t.forEach(function(e) {
                                "Comm" !== e.ID || e.Coords || a.push(e.Nom)
                            }), this.setState({
                                nomades: a
                            })
                        }
                    }, {
                        key: "getDepots",
                        value: function() {
                            var e = this.state.refWorkbook,
                                t = b.a.utils.sheet_to_json(e.Sheets.Sites),
                                a = [];
                            t.forEach(function(e) {
                                "Depots" !== e.ID || e.Coords || a.push(e.Nom)
                            }), this.setState({
                                depots: a
                            })
                        }
                    }, {
                        key: "getData",
                        value: function(e) {
                            var t = this,
                                a = this.state.refWorkbook,
                                n = b.a.utils.sheet_to_json(a.Sheets[e]);
                            n.forEach(function(e) {
                                Object.keys(e).forEach(function(t) {
                                    e[t] = e[t].toString().trim()
                                })
                            });
                            var s = [];
                            return n.forEach(function(a, n) {
                                if ("Sites" === e) a.Coords && s.push(a);
                                else if ("Tel" === e) {
                                    if ((a.Site === t.state.selectedSiteID || "Comm" === t.state.selectedSiteID && (O(t.state.nomades, a.Site) || a.Nomade) || "SST" === t.state.selectedSiteID && a.SST || "Depots" === t.state.selectedSiteID && O(t.state.depots, a.Site)) && "#" !== a.Balise || "*" === t.state.selectedSiteID) {
                                        var r = (a.Nom ? a.Nom : "") + " " + (a.Pr\u00e9nom ? a.Pr\u00e9nom : ""),
                                            o = {
                                                Id: n,
                                                Site: a.Site ? a.Site : "",
                                                Nom: r,
                                                Fonction: a.Fonction ? a.Fonction : "",
                                                "T\xe9l\xe9phone": a.T\u00e9l\u00e9phone ? C(a.T\u00e9l\u00e9phone) : "",
                                                Interne: a.Int ? a.Int : "",
                                                Mobile: a.Mobile ? C(a.Mobile) : "",
                                                "Abr\xe9g\xe9": a.Abr\u00e9g\u00e9 ? a.Abr\u00e9g\u00e9 : "",
                                                SST: a.SST ? "SST" : ""
                                            };
                                        s.push(o)
                                    }
                                } else if ("Imp" === e && (a.Site === t.state.selectedSiteID || "*" === t.state.selectedSiteID || "Depots" === t.state.selectedSiteID && O(t.state.depots, a.Site))) {
                                    var i = {
                                        Id: n,
                                        Site: a.Site ? a.Site : "",
                                        Emplacement: a.Place ? a.Place : "",
                                        Type: a.Type ? a.Type : "",
                                        Nom: a.NomTechnique ? a.NomTechnique : "",
                                        "Mod\xe8le": a.Mod\u00e8le ? a.Mod\u00e8le : "",
                                        MF: a.MF ? a.MF : "",
                                        IP: a.AdresseIP ? a.AdresseIP : "",
                                        Serveur: a.ServeurImpression ? a.ServeurImpression : ""
                                    };
                                    s.push(i)
                                }
                            }), s
                        }
                    }, {
                        key: "handleSiteSelection",
                        value: function(e) {
                            this.setState({
                                selectedSiteID: e.ID
                            }), this.setState({
                                selectedSiteInfos: e
                            })
                        }
                    }, {
                        key: "handleGlobalSearch",
                        value: function(e) {
                            this.setState({
                                selectedSiteID: "*"
                            }), this.setState({
                                globalSearchQuery: e
                            }), this.setState({
                                selectedSiteInfos: ""
                            })
                        }
                    }, {
                        key: "getContent",
                        value: function() {
                            var e = [];
                            if (void 0 === this.state.selectedSiteID) e.push(s.a.createElement(N, {
                                selectionHandler: this.handleSiteSelection,
                                globalSearchHandler: this.handleGlobalSearch,
                                sitesInfos: this.state.sitesInfos,
                                menu: this.props.menu
                            }));
                            else {
                                var t = [],
                                    a = this.state.globalSearchQuery ? this.state.globalSearchQuery : "";
                                2 === this.props.menu && (t = this.getData("Imp")), 1 === this.props.menu && (t = this.getData("Tel")), this.props.sst && (this.state.selectedSiteInfos = this.getSiteInfos("SST")), e.push(s.a.createElement(k, {
                                    menu: this.props.menu,
                                    data: t,
                                    siteInfos: this.state.selectedSiteInfos,
                                    globalSearchQuery: a
                                }))
                            }
                            return e
                        }
                    }, {
                        key: "render",
                        value: function() {
                            var e = this.state.loaded ? this.getContent() : [];
                            return s.a.createElement("div", {
                                id: "map-controller"
                            }, e)
                        }
                    }]), t
                }(s.a.Component),
                M = function(e) {
                    function t() {
                        var e, a;
                        Object(i.a)(this, t);
                        for (var n = arguments.length, s = new Array(n), r = 0; r < n; r++) s[r] = arguments[r];
                        return (a = Object(c.a)(this, (e = Object(u.a)(t)).call.apply(e, [this].concat(s)))).state = {
                            accesRapides: void 0,
                            liensRapides: void 0,
                            liensDocumentation: void 0,
                            loaded: !1
                        }, a
                    }
                    return Object(p.a)(t, e), Object(l.a)(t, [{
                        key: "componentWillMount",
                        value: function() {
                            this.getData()
                        }
                    }, {
                        key: "componentDidUpdate",
                        value: function() {
                            this.state.accesRapides && this.state.liensRapides && this.state.liensDocumentation && !this.state.loaded && this.setState({
                                loaded: !0
                            })
                        }
                    }, {
                        key: "getData",
                        value: function() {
                            var e = this,
                                t = new XMLHttpRequest;
                            return t.open("GET", "./data/liens.xlsx", !0), t.responseType = "arraybuffer", t.onload = function() {
                                var a = new Uint8Array(t.response),
                                    n = b.a.read(a, {
                                        type: "array"
                                    });
                                e.setState({
                                    accesRapides: b.a.utils.sheet_to_json(n.Sheets["Acces rapides"])
                                }), e.setState({
                                    liensRapides: b.a.utils.sheet_to_json(n.Sheets["Liens rapides"])
                                }), e.setState({
                                    liensDocumentation: b.a.utils.sheet_to_json(n.Sheets.Documentation)
                                })
                            }, t.send(null), 0
                        }
                    }, {
                        key: "getContent",
                        value: function() {
                            var e = this,
                                t = [];
                            if (0 === this.props.menu) {
                                for (var a = [], n = 0; n < Math.ceil(this.state.accesRapides.length / 3); n++) {
                                    for (var r = [], o = 3 * n; o < 3 * n + 3; o++) {
                                        var i = this.state.accesRapides[o];
                                        void 0 !== i && r.push(s.a.createElement("div", {
                                            className: "col-sm"
                                        }, s.a.createElement("a", {
                                            rel: "noopener noreferrer",
                                            className: "acces btn btn-outline-dark",
                                            role: "button",
                                            href: i.Lien
                                        }, s.a.createElement("img", {
                                            src: "./icons/" + i.Image,
                                            alt: i.Image.substr(0, i.Image.length - 4)
                                        }), " ", s.a.createElement("br", null), s.a.createElement("span", null, i.Nom))))
                                    }
                                    a.push(s.a.createElement("div", {
                                        className: "row"
                                    }, r))
                                }
                                t.push(a)
                            }
                            if (3 === this.props.menu || 4 === this.props.menu) {
                                var l = 3 === this.props.menu ? this.state.liensRapides : this.state.liensDocumentation,
                                    c = [],
                                    u = "";
                                l.forEach(function(t, a) {
                                    t.Categorie !== u && (u = t.Categorie, c.push([u, []]));
                                    var n = 3 === e.props.menu ? t.Lien : ".".concat(t.Lien);
                                    c[c.length - 1][1].push(s.a.createElement("div", {
                                        className: "col-3"
                                    }, s.a.createElement("a", {
                                        rel: "noopener noreferrer",
                                        target: "_blank",
                                        className: "rapide btn btn-light",
                                        role: "button",
                                        href: n
                                    }, s.a.createElement("span", null, t.Nom))))
                                }), c.forEach(function(e) {
                                    t.push(s.a.createElement("div", {
                                        className: "category"
                                    }, s.a.createElement("div", {
                                        className: "category-name"
                                    }, e[0]), s.a.createElement("div", {
                                        className: "container"
                                    }, s.a.createElement("div", {
                                        className: "row justify-content-center"
                                    }, e[1]))))
                                })
                            }
                            return 1 !== this.props.menu && 2 !== this.props.menu || t.push(s.a.createElement(j, {
                                menu: this.props.menu,
                                resetMap: this.props.resetMap,
                                sst: this.props.sst
                            })), t
                        }
                    }, {
                        key: "render",
                        value: function() {
                            var e = this.state.loaded ? this.getContent() : [];
                            return s.a.createElement("div", {
                                id: "ContentWindow",
                                className: "container"
                            }, e)
                        }
                    }]), t
                }(s.a.Component),
                D = ["Accueil", "Listes t\xe9l\xe9phoniques", "Imprimantes et scanners", "Liens rapides", "Documentation", "Identité"],
                w = function(e) {
                    function t(e) {
                        var a;
                        Object(i.a)(this, t), (a = Object(c.a)(this, Object(u.a)(t).call(this, e))).state = {
                            menu: 0,
                            resetMap: !1,
                            sst: !1
                        }, a.handleMenuChange = a.handleMenuChange.bind(Object(h.a)(a));
                        var n = a.findGetParameter("menu"),
                            s = a.findGetParameter("sst");
                        return n && (a.state.menu = parseInt(n)), "true" === s && (a.state.sst = !0), a
                    }
                    return Object(p.a)(t, e), Object(l.a)(t, [{
                        key: "findGetParameter",
                        value: function(e) {
                            var t = null,
                                a = [];
                            return window.location.search.substr(1).split("&").forEach(function(n) {
                                (a = n.split("="))[0] === e && (t = decodeURIComponent(a[1]))
                            }), t
                        }
                    }, {
                        key: "handleMenuChange",
                        value: function(e) {
                            this.setState({
                                menu: e
                            }), 1 !== e && 2 !== e || (this.setState({
                                resetMap: !0
                            }), this.setState({
                                sst: !1
                            }))
                        }
                    }, {
                        key: "render",
                        value: function() {
                            var e = !1,
                                t = "";
                            return 2 === this.state.menu && (t = " imp"), this.state.resetMap && (e = !0, this.setState({
                                resetMap: !1
                            })), s.a.createElement("div", {
                                id: "App"
                            }, s.a.createElement("div", {
                                className: "header no-print"
                            }, s.a.createElement("span", {
                                id: "Title"
                            }, s.a.createElement("img", {
                                alt: "easyinternet",
                                src: "./icons/easy_internet.png"
                            }), " - ", D[this.state.menu])), s.a.createElement("div", {
                                id: "divider"
                            }), s.a.createElement("div", {
                                className: "wrapper" + t
                            }, s.a.createElement(f, {
                                menu: this.state.menu,
                                btnAction: this.handleMenuChange
                            }), s.a.createElement(M, {
                                menu: this.state.menu,
                                resetMap: e,
                                sst: this.state.sst
                            })))
                        }
                    }]), t
                }(s.a.Component);
            a(106);
            o.a.render(s.a.createElement(w, null), document.getElementById("root"))
        },
        61: function(e, t) {},
        91: function(e, t, a) {
            e.exports = a(199)
        }
    },
    [
        [91, 1, 2]
    ]
]);

//# sourceMappingURL=main.baa2b236.chunk.js.map


