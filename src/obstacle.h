/**
  \file
  \brief    Déclaration de la classe Obstacle.
  \author   Clark Winkelmann
  \date     février 2014
*/
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "sprite.h"

//!
//! \brief Classe qui définit un obstacle.
//!
//! Cette classe est une extension de la classe Sprite qui ajoute les propriétés, fonctions et signaux propres à l'Obstacle.
//! Les obstacles bloquent ou gènent les déplacements des joueurs/ennemis et peuvent avoir des fonctions spéciales.
//!
class Obstacle : public Sprite
{
    Q_OBJECT
public:
    //! \brief Type de l'obstacle
    enum ObstacleType_e{
        Box,
        Wall
    };

    Obstacle(ObstacleType_e Type, QGraphicsItem *pParent=0);
    ObstacleType_e getObstacleType();

    enum { ObstacleItemType = UserType + 50 };
    virtual int type() const { return ObstacleItemType; }

private:
    ObstacleType_e m_ObstacleType;
};

#endif // OBSTACLE_H
