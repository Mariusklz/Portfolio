@echo off

net use M: \\WBGWFS11\FR_METIERS$ /persistent:yes
net use P: \\WBGWFS11\FR_PUBLIC$ /persistent:yes
net use Q: \\WBGWFS11\FR_TEMPO /persistent:yes
net use S: \\WBGWFS11\FR_SITES$ /persistent:yes

xcopy /y \\ACHSV023\FR_Ressources$\EasyInternet.lnk %userprofile%\Desktop\