#!/bin/bash
# Script de génération des ressources du projet Zombu
# Auteur: Clark Winkelmann
# Date: mars 2014
#
# Ce script utilise InkScape depuis la ligne de commande

# Emplacement des sources InkScape
srcdir='../res/sources/'
# Emplacement de sortie
outputdir='../res/images/'

# Création du répertoire de sortie s'il nexiste pas
mkdir -p $outputdir

# Hero
inkscape -f "$srcdir"hero.svg -w 77 -h 120 -i layer1 -j -C -e "$outputdir"hero1.png
inkscape -f "$srcdir"hero.svg -w 77 -h 120 -i layer2 -j -C -e "$outputdir"hero2.png
inkscape -f "$srcdir"hero.svg -w 77 -h 120 -i layer3 -j -C -e "$outputdir"hero3.png
inkscape -f "$srcdir"hero.svg -w 77 -h 120 -i layer4 -j -C -e "$outputdir"hero4.png
inkscape -f "$srcdir"hero.svg -w 77 -h 120 -i layer5 -j -C -e "$outputdir"hero5.png

# Zombie
inkscape -f "$srcdir"zombie.svg -w 80 -h 75 -i layer1 -j -C -e "$outputdir"zombie1.png
inkscape -f "$srcdir"zombie.svg -w 80 -h 75 -i layer2 -j -C -e "$outputdir"zombie2.png
inkscape -f "$srcdir"zombie.svg -w 80 -h 75 -i layer3 -j -C -e "$outputdir"zombie3.png
inkscape -f "$srcdir"zombie.svg -w 80 -h 75 -i layer4 -j -C -e "$outputdir"zombie4.png
inkscape -f "$srcdir"zombie.svg -w 80 -h 75 -i layer5 -j -C -e "$outputdir"zombie5.png

# Missile
inkscape -f "$srcdir"missile.svg -w 14 -h 29 -e "$outputdir"missile.png

# Blood
inkscape -f "$srcdir"blood.svg -w 50 -h 50 -e "$outputdir"blood.png

# Box
inkscape -f "$srcdir"box.svg -w 100 -h 100 -e "$outputdir"box.png

# Wall
inkscape -f "$srcdir"wall.svg -w 250 -h 154 -e "$outputdir"wall.png

# Logo
inkscape -f "$srcdir"logo.svg -w 1000 -h 322 -e "$outputdir"logo.png

