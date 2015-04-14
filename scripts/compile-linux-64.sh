#!/bin/bash
# Script de compilation et empaquetage pour linux (64bit)
# Auteur: Clark Winkelmann
# Date: avril 2015

# Éléments répétés

resdir='../res'
docdir='../doc'
profile='../src/Zombu.pro'
exefile='Zombu'
compiledir='linux-compile-64'
deploydir='linux-deploy-64'

# Étapes préliminaires

echo 'Génération des ressources...'
bash generate-res.sh

echo 'Génération des locales...'
bash locales-convert.sh

# Suppression et recréation des dossiers

echo 'Nettoyage des répertoires...'
rm -rf $compiledir
rm -rf $deploydir

mkdir $compiledir
mkdir $deploydir

# Compilation

cd $compiledir

echo 'Exécution de QMake...'
qmake ../"$profile" -spec linux-g++-64 "CONFIG+=release" "DEFINES+=DEPLOY"

echo 'Excution de Make...'
make

cd ..

# Copie des fichiers à déployer

echo 'Copie des fichiers à déployer...'

mkdir "$deploydir"/res/
mkdir "$deploydir"/res/images/
mkdir "$deploydir"/res/locales/
mkdir "$deploydir"/res/words/

cp "$compiledir"/Zombu "$deploydir"/
cp "$docdir"/README_Linux_64.md "$deploydir"/README.md
cp "$resdir"/images/*.png "$deploydir"/res/images/
cp "$resdir"/locales/*.qm "$deploydir"/res/locales/
cp "$resdir"/words/*.txt "$deploydir"/res/words/

# Création de l'archive

echo 'Création de l''archive...'
cd $deploydir
tar czvf ../Zombu_Linux_64bit.tar.gz *
cd ..

# Nettoyage

echo 'Nettoyage...'
rm -rf $compiledir
rm -rf $deploydir

# Fin de l'exécution du script

echo 'Terminé.'

