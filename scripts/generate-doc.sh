#!/bin/bash
# Script de génération des documents du projet Zombu
# Auteur: Clark Winkelmann
# Date: mars 2014
#
# Ce script requiert LibreOffice et Doxygen
# Aucune instance de LibreOffice ne doit être ouverte pendant l'exécution du script !

# Documentation ODT vers PDF
lowriter --headless --convert-to pdf ../doc/*.odt --outdir ../doc/

# Documentation Doxygen
doxygen ../src/Doxyfile
