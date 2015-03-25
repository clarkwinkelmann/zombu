/**
  \file
  \brief    Déclaration de l'espace de nommage LibZombu.
  \author   Clark Winkelmann
  \date     mars 2014
*/
#include "libzombu.h"

namespace LibZombu{
//!
//! \brief Calcule de quel côté du rectangle la position donnée est le plus proche.
//! \param Position Position à tester
//! \param Rectangle Rectangle définissant les limites
//! \return Côté du rectangle
//!
Side_e getCloserSideOfRect(QPointF Position, QRectF Rectangle){
    // On calcule la position par rapport au rectangle
    QPointF RelPosition = QPointF(Position.x()-Rectangle.x(),Position.y()-Rectangle.y());

    float RelPosX = RelPosition.x();
    float RelPosY = RelPosition.y();
    float RectHalfWidth = Rectangle.width()/2;
    float RectHalfHeight = Rectangle.height()/2;

    // On cherche si on est gauche/droite ou haut/bas
    if((RelPosX > RectHalfWidth ? Rectangle.width()-RelPosX : RelPosX)
            < (RelPosY > RectHalfHeight ? Rectangle.height()-RelPosY : RelPosY)){
        // On cherche si gauche ou droite
        if(RelPosX < RectHalfWidth){
            return Left;
        }else{
            return Right;
        }
    }else{
        // On cherche si haut ou bas
        if(RelPosY < RectHalfHeight){
            return Top;
        }else{
            return Bottom;
        }
    }
}

//!
//! \brief Retourne la position la plus proche en dehors du rectangle donné.
//! \param Position Point se trouvant dans le rectangle
//! \param Rectangle Rectangle définissant les limites
//! \return Nouvelle position
//!
//! Calcule la position en dehors du rectangle en cherchant de quel côté la position est le plus proche.
//! Cette fonction implique que Position se trouve dans Rectangle.
//! Pour des raisons de performances, ce test est à exécuter avant d'appeler cette fonction.
//!
QPointF getPositionOutsideRect(QPointF Position, QRectF Rectangle){
    QPointF NewPosition = Position;
    // On cherche de quel côté on est arrivé
    switch (getCloserSideOfRect(Position,Rectangle)){
    case Left:
        NewPosition.setX(Rectangle.x());
        break;
    case Right:
        NewPosition.setX(Rectangle.right());
        break;
    case Top:
        NewPosition.setY(Rectangle.y());
        break;
    case Bottom:
        NewPosition.setY(Rectangle.bottom());
        break;
    }
    return NewPosition;
}
}
