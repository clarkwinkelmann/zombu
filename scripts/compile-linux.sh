#!/bin/bash
# Script de compilation et empaquetage pour linux
# Auteur: Clark Winkelmann
# Date: avril 2015

# Éléments répétés

resdir='../res'
profile='../src/Zombu.pro'
exefile='Zombu'
compiledir='linux-compile'
deploydir='linux-deploy'

# Suppression et recréation des dossiers

rm -rf $compiledir
rm -rf $deploydir

mkdir $compiledir
mkdir $deploydir

# Compilation

cd $compiledir

qmake ../"$profile" -spec linux-g++ "CONFIG+=release" "DEFINES+=DEPLOY"

make

cd ..

# Copie des fichiers à déployer

mkdir "$deploydir"/res/
mkdir "$deploydir"/res/images/
mkdir "$deploydir"/res/locales/
mkdir "$deploydir"/res/words/

cp "$compiledir"/Zombu "$deploydir"/
cp "$resdir"/images/*.png "$deploydir"/res/images/
cp "$resdir"/locales/*.qm "$deploydir"/res/locales/
cp "$resdir"/words/*.txt "$deploydir"/res/words/

# Création de l'archive

cd $deploydir
tar czvf ../Zombu_Linux.tar.gz *
cd ..

# Nettoyage

rm -rf $compiledir
rm -rf $deploydir

