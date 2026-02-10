#!/bin/bash

# ===================================
# SCRIPT DE TEST DE SÉCURITÉ
# Portfolio KELTZ Marius
# web.primegaming.freedns.org
# ===================================

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

SITE_URL="https://web.primegaming.freedns.org"

echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}   TEST DE SÉCURITÉ - PORTFOLIO KELTZ     ${NC}"
echo -e "${BLUE}============================================${NC}"
echo ""

# ===================================
# Test 1 : Disponibilité du site
# ===================================
echo -e "${YELLOW}[1/8] Test de disponibilité...${NC}"
HTTP_CODE=$(curl -o /dev/null -s -w "%{http_code}" "$SITE_URL")
if [ "$HTTP_CODE" == "200" ]; then
    echo -e "${GREEN}✓ Site accessible (HTTP $HTTP_CODE)${NC}"
else
    echo -e "${RED}✗ Site non accessible (HTTP $HTTP_CODE)${NC}"
fi
echo ""

# ===================================
# Test 2 : HTTPS et redirection
# ===================================
echo -e "${YELLOW}[2/8] Test HTTPS et redirection...${NC}"
HTTP_REDIRECT=$(curl -I -s -o /dev/null -w "%{redirect_url}" "http://web.primegaming.freedns.org" | grep -i "https")
if [ -n "$HTTP_REDIRECT" ]; then
    echo -e "${GREEN}✓ Redirection HTTP → HTTPS active${NC}"
else
    echo -e "${YELLOW}⚠ Pas de redirection HTTP → HTTPS détectée${NC}"
fi
echo ""

# ===================================
# Test 3 : Headers de sécurité
# ===================================
echo -e "${YELLOW}[3/8] Test des headers de sécurité...${NC}"

HEADERS=$(curl -I -s "$SITE_URL")

# X-Frame-Options
if echo "$HEADERS" | grep -qi "X-Frame-Options"; then
    echo -e "${GREEN}✓ X-Frame-Options présent${NC}"
else
    echo -e "${RED}✗ X-Frame-Options absent${NC}"
fi

# X-Content-Type-Options
if echo "$HEADERS" | grep -qi "X-Content-Type-Options"; then
    echo -e "${GREEN}✓ X-Content-Type-Options présent${NC}"
else
    echo -e "${RED}✗ X-Content-Type-Options absent${NC}"
fi

# Strict-Transport-Security
if echo "$HEADERS" | grep -qi "Strict-Transport-Security"; then
    echo -e "${GREEN}✓ HSTS (Strict-Transport-Security) présent${NC}"
else
    echo -e "${YELLOW}⚠ HSTS absent (à activer après tests HTTPS)${NC}"
fi

# Content-Security-Policy
if echo "$HEADERS" | grep -qi "Content-Security-Policy"; then
    echo -e "${GREEN}✓ Content-Security-Policy présent${NC}"
else
    echo -e "${RED}✗ Content-Security-Policy absent${NC}"
fi

# Referrer-Policy
if echo "$HEADERS" | grep -qi "Referrer-Policy"; then
    echo -e "${GREEN}✓ Referrer-Policy présent${NC}"
else
    echo -e "${RED}✗ Referrer-Policy absent${NC}"
fi

# X-XSS-Protection
if echo "$HEADERS" | grep -qi "X-XSS-Protection"; then
    echo -e "${GREEN}✓ X-XSS-Protection présent${NC}"
else
    echo -e "${RED}✗ X-XSS-Protection absent${NC}"
fi
echo ""

# ===================================
# Test 4 : Certificat SSL
# ===================================
echo -e "${YELLOW}[4/8] Test du certificat SSL...${NC}"
SSL_EXPIRY=$(echo | openssl s_client -servername web.primegaming.freedns.org -connect web.primegaming.freedns.org:443 2>/dev/null | openssl x509 -noout -dates 2>/dev/null | grep notAfter | cut -d= -f2)
if [ -n "$SSL_EXPIRY" ]; then
    echo -e "${GREEN}✓ Certificat SSL valide${NC}"
    echo -e "  Expire le : $SSL_EXPIRY"
else
    echo -e "${YELLOW}⚠ Impossible de vérifier le certificat SSL${NC}"
fi
echo ""

# ===================================
# Test 5 : Protection XSS
# ===================================
echo -e "${YELLOW}[5/8] Test protection XSS...${NC}"
XSS_TEST=$(curl -s "$SITE_URL?test=<script>alert('XSS')</script>" | grep -i "<script>")
if [ -z "$XSS_TEST" ]; then
    echo -e "${GREEN}✓ Protection XSS semble active (script non injecté)${NC}"
else
    echo -e "${RED}✗ Vulnérabilité XSS détectée !${NC}"
fi
echo ""

# ===================================
# Test 6 : Fichiers sensibles exposés
# ===================================
echo -e "${YELLOW}[6/8] Test de fichiers sensibles exposés...${NC}"

SENSITIVE_FILES=(
    "/.git/config"
    "/.env"
    "/.htaccess"
    "/config.php"
    "/backup.sql"
    "/.gitignore"
)

EXPOSED=0
for file in "${SENSITIVE_FILES[@]}"; do
    HTTP_CODE=$(curl -o /dev/null -s -w "%{http_code}" "$SITE_URL$file")
    if [ "$HTTP_CODE" == "200" ]; then
        echo -e "${RED}✗ Fichier exposé : $file${NC}"
        EXPOSED=$((EXPOSED + 1))
    fi
done

if [ $EXPOSED -eq 0 ]; then
    echo -e "${GREEN}✓ Aucun fichier sensible exposé${NC}"
fi
echo ""

# ===================================
# Test 7 : Rate Limiting
# ===================================
echo -e "${YELLOW}[7/8] Test du rate limiting (15 requêtes rapides)...${NC}"
RATE_LIMIT_TRIGGERED=0
for i in {1..15}; do
    HTTP_CODE=$(curl -o /dev/null -s -w "%{http_code}" "$SITE_URL")
    if [ "$HTTP_CODE" == "429" ] || [ "$HTTP_CODE" == "503" ]; then
        RATE_LIMIT_TRIGGERED=1
        break
    fi
    sleep 0.1
done

if [ $RATE_LIMIT_TRIGGERED -eq 1 ]; then
    echo -e "${GREEN}✓ Rate limiting actif (HTTP 429/503 détecté)${NC}"
else
    echo -e "${YELLOW}⚠ Rate limiting non détecté (peut nécessiter plus de requêtes)${NC}"
fi
echo ""

# ===================================
# Test 8 : Version serveur exposée
# ===================================
echo -e "${YELLOW}[8/8] Test de divulgation de version serveur...${NC}"
SERVER_HEADER=$(echo "$HEADERS" | grep -i "^Server:")
if echo "$SERVER_HEADER" | grep -qi "nginx\|apache"; then
    VERSION=$(echo "$SERVER_HEADER" | grep -Eo "[0-9]+\.[0-9]+")
    if [ -n "$VERSION" ]; then
        echo -e "${YELLOW}⚠ Version serveur exposée : $SERVER_HEADER${NC}"
    else
        echo -e "${GREEN}✓ Version serveur masquée${NC}"
    fi
else
    echo -e "${GREEN}✓ Header Server masqué ou absent${NC}"
fi
echo ""

# ===================================
# RÉSUMÉ FINAL
# ===================================
echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}   RÉSUMÉ DES TESTS${NC}"
echo -e "${BLUE}============================================${NC}"
echo ""
echo -e "${GREEN}Tests de sécurité terminés !${NC}"
echo ""
echo -e "Pour des tests plus approfondis, utilisez :"
echo -e "  • SSL Labs   : ${BLUE}https://www.ssllabs.com/ssltest/analyze.html?d=web.primegaming.freedns.org${NC}"
echo -e "  • SecHeaders : ${BLUE}https://securityheaders.com/?q=web.primegaming.freedns.org${NC}"
echo -e "  • OWASP ZAP  : ${BLUE}https://www.zaproxy.org/${NC}"
echo ""
echo -e "${YELLOW}Note : Certains tests peuvent nécessiter une configuration NPM/serveur pour être pleinement efficaces.${NC}"
echo ""
