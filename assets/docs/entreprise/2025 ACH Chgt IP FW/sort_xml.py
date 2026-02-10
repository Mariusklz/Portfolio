import xml.etree.ElementTree as ET

def extract_hosts(xml_file):
    """Extrait toutes les balises <H> avec leur contenu."""
    try:
        tree = ET.parse(xml_file)
        root = tree.getroot()
    except (FileNotFoundError, ET.ParseError) as e:
        print(f"Erreur lors de l'ouverture du fichier XML : {e}")
        return []

    hosts = []
    for h in root.findall('H'):
        hip = h.find('HIP')
        if hip is not None and hip.text:
            hosts.append((hip.text, h))  # Stocke l'adresse IP + l'élément XML complet

    return hosts

def sort_hosts_by_ip(hosts):
    """Trie les balises <H> en fonction de la valeur de <HIP>."""
    return sorted(hosts, key=lambda x: tuple(map(int, x[0].split('.'))))

def create_sorted_xml(hosts, output_file):
    """Crée un fichier XML trié en conservant toutes les balises."""
    root = ET.Element('Root')
    for _, h in hosts:  # On récupère l'élément <H> sans toucher à son contenu
        root.append(h)

    tree = ET.ElementTree(root)
    tree.write(output_file, encoding='utf-8', xml_declaration=True)
    print(f"✅ Fichier trié généré : {output_file}")

def main():
    input_file = "alertping.xml"  # Remplace avec ton fichier d'entrée + Mettre tous le fichier dans une balise Root pour éxecuter le programme
    output_file = "sorted_output.xml"

    hosts = extract_hosts(input_file)
    if hosts:
        sorted_hosts = sort_hosts_by_ip(hosts)
        create_sorted_xml(sorted_hosts, output_file)

if __name__ == "__main__":
    main()
