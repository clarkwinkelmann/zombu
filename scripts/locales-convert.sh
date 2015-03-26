#!/bin/bash
# Script de compilation des fichiers de traduction
# Auteur: Clark Winkelmann
# Date: mars 2015

# Emplacement des sources de traduction
srcdir='../src/locales/'
# Emplacement de sortie des traductions
outdir='../res/locales/'

lconvert -i "$srcdir"en.ts -o "$outdir"en.qm

