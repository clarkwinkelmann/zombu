@echo off
::
:: Script de compilation du projet Zombu
:: Auteur: Clark Winkelmann
:: Date: mars 2014
::

if "%1"=="/?" goto help

set Verbose=0
set Debug=0

if "%1"=="/V" set Verbose=1
if "%2"=="/V" set Verbose=1
if "%3"=="/V" set Verbose=1

if "%1"=="/D" set Debug=1
if "%2"=="/D" set Debug=1
if "%3"=="/D" set Debug=1

if "%Verbose%"=="1" echo Demarrage du script de compilation... && echo.

:: Emplacement de qmake
set Qmake=C:\Qt\Static\5.2.0\bin\qmake.exe
:: Répertoire de make
set MakeBin=C:\Qt\Qt5.2.0\Tools\mingw48_32\bin\
:: Exécutable de make
set Make=%MakeBin%mingw32-make.exe
:: Fichier du projet
set Project=..\src\Zombu.pro
:: Nom de l'exécutable que créé ce projet
set Executable=Zombu.exe
:: Répertoire de compilation
set CompilDir=tmpdir
:: Répertoire de sortie
set DeployDir=deploy
:: Exécutable Inno Setup
set InnoSetup="C:\Program Files (x86)\Inno Setup 5\iscc.exe"
:: Fichier de configuration Inno Setup
set InnoConfig=setup.iss

:: Vérification des emplacements
if not exist %QMake% echo [Erreur] Executable QMake inaccessible ! && goto error
if not exist %Make% echo [Erreur] Executable Make inaccessible ! && goto error
if not exist %InnoSetup% echo [Erreur] Executable Inno Setup inaccessible ! && goto error

:: Ajoute temporairement le dossier de make au path
set PATH=%PATH%;%MakeBin%

if "%Verbose%"=="1" echo Suppression des anciens dossiers...
rmdir %CompilDir% /S /Q 2> nul
rmdir %DeployDir% /S /Q 2> nul

if "%Verbose%"=="1" echo Creation des dossiers...
mkdir %CompilDir%
mkdir %DeployDir%

if "%Verbose%"=="1" echo. && echo Compilation du projet...

cd %CompilDir%

:: Exécution de qmake
%Qmake% ..\%Project% -r -spec win32-g++ "CONFIG+=release" "DEFINES += DEPLOY"

:: Exécution de make + Nettoyage
if "%Debug%"=="1" goto makedebug
	%Make% > nul 2> nul
	%Make% clean > nul 2> nul
goto endmake
:makedebug
	%Make%
	%Make% clean
:endmake

if "%Verbose%"=="1" echo Fin de la compilation.

:: Sort du répertoire de compilation
cd ..

if "%Verbose%"=="1" echo. && echo Copie des fichiers vers le repertoire de sortie...
copy %CompilDir%\release\%Executable% %DeployDir%\ > nul

if "%Verbose%"=="1" echo Copie des ressources...
mkdir %DeployDir%\res\images\
mkdir %DeployDir%\res\locales\
mkdir %DeployDir%\res\words\
copy ..\res\images\*.png %DeployDir%\res\images\
copy ..\res\locales\*.qm %DeployDir%\res\locales\
copy ..\res\words\*.txt %DeployDir%\res\words\

if "%Verbose%"=="1" echo Fin de la copie.

if "%Verbose%"=="1" echo. && echo Creation de l'installeur...
%InnoSetup% /Q %InnoConfig%

if "%Verbose%"=="1" echo L'installeur a ete cree.

:: Si l'utilisateur a choisi de conserver les fichiers on saute cette étape
if "%1"=="/K" goto finished
if "%2"=="/K" goto finished
if "%3"=="/K" goto finished

if "%Verbose%"=="1" echo. && echo Nettoyage du repertoire...

:: Supprime les dossiers
rmdir %CompilDir% /S /Q
rmdir %DeployDir% /S /Q

:finished

if "%Verbose%"=="0" goto end

echo.
echo Fin de l'execution du script.
echo Vous pouvez a present deployer votre application avec l'installeur.
echo.
pause

goto end

:error

echo.
echo Une erreur est survenue lors de l'execution du script. Abandon.
echo.
pause

goto end

:help
echo.
echo # [Aide]
echo # Compilateur du projet Zombu
echo # Auteur: Clark Winkelmann
echo #
echo # COMPILE [/?] [/K] [/V] [/D]
echo #
echo #   /?  Affiche l'aide
echo #   /K  Conserve les dossiers de compilation et de sortie
echo #   /V  Affiche des informations sur l'execution du script
echo #   /D  Active le mode deboguage

:end

:: Libère la mémoire
set Verbose=
set Debug=
set Qmake=
set MakeBin=
set Make=
set Project=
set Executable=
set CompilDir=
set DeployDir=
set InnoSetup=
set InnoConfig=
