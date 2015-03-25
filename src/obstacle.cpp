/**
  \file
  \brief    Déclaration de la classe Obstacle.
  \author   Clark Winkelmann
  \date     février 2014
*/
#include "obstacle.h"
#include "resources.h"

//!
//! \brief Création de l'objet Obstacle.
//! \param Type Type de l'obstacle
//! \param pParent Parent de l'élément
//!
//! Créé un obstacle d'un certain type.
//!
Obstacle::Obstacle(ObstacleType_e Type, QGraphicsItem *pParent) : Sprite(pParent)
{
    m_ObstacleType = Type;

    QString Image = "";
    switch(Type){
    case Box:
        Image = "box.png";
        break;
    case Wall:
        Image = "wall.png";
        break;
    }
    addAnimationFrame(QString(GameFramework::imagesPath()+Image));

    setZValue(50);
}

//!
//! \brief Retourne le type d'obstacle.
//! \return Type d'obstacle
//!
Obstacle::ObstacleType_e Obstacle::getObstacleType(){
    return m_ObstacleType;
}
