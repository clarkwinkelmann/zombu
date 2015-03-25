/**
  \file
  \brief    Déclaration de l'espace de nommage LibZombu.
  \author   Clark Winkelmann
  \date     mars 2014
*/
#ifndef LIBZOMBU_H
#define LIBZOMBU_H

#include <QPointF>
#include <QRect>
#include <QStringList>

//!
//! \brief Fonctions utilitaires du projet Zombu.
//!
//! Fournit les fonctions utilisées globalement dans le projet.
//!
namespace LibZombu{

//! Énuméré des côtés
enum Side_e{
    Left,
    Right,
    Top,
    Bottom
};

const QStringList ALPHABET = QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z";

Side_e getCloserSideOfRect(QPointF Position, QRectF Rectangle);
QPointF getPositionOutsideRect(QPointF Position, QRectF Rectangle);

}

#endif // LIBZOMBU_H
