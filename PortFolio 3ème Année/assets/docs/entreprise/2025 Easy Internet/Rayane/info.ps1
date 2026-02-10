##info.ps1

# Définir le chemin du fichier de sortie
$outputFile = "$PSScriptRoot\js\information.js"

# Vérifier s'il existe déjà et supprimer le fichier s'il existe
if (Test-Path $outputFile) {
    Remove-Item $outputFile -Force
}

# Récupérer les informations système
$lastBoot = (Get-WmiObject Win32_OperatingSystem).LastBootUpTime
$DernierBoot = [Management.ManagementDateTimeConverter]::ToDateTime($lastBoot)

$model = (Get-WmiObject Win32_ComputerSystem).Model
$bios = Get-WmiObject Win32_BIOS
$serialNumber = $bios.SerialNumber
$windows = Get-WmiObject Win32_OperatingSystem
$windowsVersion = (Get-ItemProperty "HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion").ReleaseId + "H"

$ip = (Get-NetIPAddress -AddressFamily IPv4 | Where-Object { $_.PrefixOrigin -eq "Dhcp" -or $_.PrefixOrigin -eq "Manual" }).IPAddress | Select-Object -First 1

# Obtenir l'imprimante par défaut
$defaultPrinter = (Get-WmiObject -Class Win32_Printer | Where-Object { $_.Default -eq $true }).Name

# Créer un tableau avec les informations système
$infos = @{
    "Utilisateur" = $env:USERNAME
    "Machine" = $env:COMPUTERNAME
    "Domaine" = (Get-WmiObject Win32_ComputerSystem).Domain
    "Modele" = "$model <br>($serialNumber)"  
    "Windows" = "$($windows.Caption) (Build $($windows.BuildNumber), $windowsVersion)"
    "RAM" = "{0:N2} GB" -f ((Get-WmiObject Win32_ComputerSystem).TotalPhysicalMemory / 1GB)
    "Stockage" = "{1:N2} GB / {0:N2} GB" -f ((Get-PSDrive C).Used / 1GB), ((Get-PSDrive C).Free / 1GB)
    "IP" = $ip
    "DernierBoot" = $DernierBoot.ToString("dd/MM/yyyy HH:mm:ss")
    "ImprimanteDefaut" = $defaultPrinter
}

# Convertir en JSON
$jsonData = $infos | ConvertTo-Json -Depth 2 -Compress
$jsContent = "const systemInfo = $jsonData;"

# Afficher les données dans la console PowerShell pour débogage
Write-Host "Données système générées : $jsContent"

# Écrire les données dans le fichier
Set-Content -Path $outputFile -Value $jsContent -Encoding UTF8
Write-Host "Fichier $outputFile mis à jour avec succès"