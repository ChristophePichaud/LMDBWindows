#FROM microsoft/windowsservercore
#FROM microsoft/iis
#FROM microsoft/iis:nanoserver-sac2016
FROM microsoft/iis:windowsservercore-ltsc2016
#RUN netsh advfirewall set allprofiles state off
COPY *.* c:/
#RUN net stop w3svc
#RUN net start LMDBService
RUN sc sdset SCMANAGER D:(A;;CCLCRPRC;;;AU)(A;;CCLCRPWPRC;;;SY)(A;;KA;;;BA)S:(AU;FA;KA;;;WD)(AU;OIIOFA;GA;;;WD)
RUN sc create LMDBService start=auto binpath="C:\LMDBService.exe"
#EXPOSE 80	
EXPOSE 7001	
RUN md c:\temp
