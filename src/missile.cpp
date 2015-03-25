/**
  \file
  \brief    Déclaration de la classe Missile.
  \author   Clark Winkelmann
  \date     février 2014
*/
#include <QtMath>

#include "missile.h"
#include "hero.h"
#include "obstacle.h"
#include "resources.h"
#include "libzombu.h"

//!
//! \brief Création de l'objet missile.
//! \param Position position à laquelle créer le missile
//! \param Angle Angle en degrés (direction)
//! \param Speed Vitesse en px par seconde
//! \param Damage Damages en points de vie
//! \param pParent Objet parent
//!
Missile::Missile(QPointF Position, float Angle, float Speed, float Damage, QGraphicsItem *pParent) : Sprite(pParent)
{
    m_Speed = Speed;
    m_Damage = Damage;
    m_HitsLeft = 5;

    m_pHero = 0;

    m_GameZone = QRectF();

    addAnimationFrame(QString(GameFramework::imagesPath()+"missile.png"));
    setTransformOriginPoint(width()/2, height()/2);

    setPos(Position-QPointF(width()/2,height()/2));
    setRotation(Angle);

    // Z index
    setZValue(150);
}

//!
//! \brief Définit la zone de jeu.
//! \param GameZone Zone de jeu
//!
void Missile::setGameZone(QRectF GameZone){
    m_GameZone = GameZone;
}

//!
//! \brief Définit le héros qui a créé le missile.
//! \param pHero
//!
void Missile::setHero(Hero *pHero){
    m_pHero = pHero;
}

//!
//! \brief Récupère le héros qui a créé le missile.
//! \return Pointeur vers le héros
//!
Hero* Missile::getHero(){
    return m_pHero;
}

//!
//! \brief Décupère la puissance de dégâts du missile.
//! \return Points de dégâts
//!
float Missile::getDamage(){
    return m_Damage;
}

//!
//! \brief Met à jour la position du missile.
//! \param ElapsedTimeInMilliSeconds Temps depuis le dernier tick
//!
//! Met à jour la position du missile. S'il sort de la zone de jeu il est supprimé.
//!
void Missile::update(int ElapsedTimeInMilliSeconds){
    float AngleInRadians = qDegreesToRadians(rotation());
    float MoveDistance = static_cast<float>(ElapsedTimeInMilliSeconds) * m_Speed / 1000;

    float MoveX = qSin(AngleInRadians) * MoveDistance;
    float MoveY = qCos(AngleInRadians) * MoveDistance * -1;

    QPointF NewPosition = pos()+QPointF(MoveX,MoveY);

    if(m_GameZone.contains(NewPosition)){
        setPos(NewPosition);
        emit hasMoved(NewPosition);

        // Tester les collisions
        QList<QGraphicsItem*> CollidingItems = collidingItems();
        foreach(QGraphicsItem* pItem, CollidingItems){
            // Si le missile touche un obstacle
            if(pItem->type() == Obstacle::ObstacleItemType){
                if(m_HitsLeft > 0){
                    Obstacle* pObstacle = static_cast<Obstacle*>(pItem);

                    // En fonction de la direction par laquelle il est arrivé le missile rebondit différemment
                    switch(LibZombu::getCloserSideOfRect(pos(),QRectF(pObstacle->x(),pObstacle->y(),pObstacle->width(),pObstacle->height()))){
                    case LibZombu::Left:
                        setRotation(360-rotation());
                        setX(pObstacle->x()-width());
                        break;
                    case LibZombu::Right:
                        setRotation(360-rotation());
                        setX(pObstacle->right());
                        break;
                    case LibZombu::Top:
                        setRotation(180-rotation());
                        setY(pObstacle->y()-height());
                        break;
                    case LibZombu::Bottom:
                        setRotation(180-rotation());
                        setY(pObstacle->bottom());
                        break;
                    }

                    m_HitsLeft--;
                }else{
                    emit hasExited(NewPosition);
                    delete this;
                    return;
                }
            }
        }
    }else{
        emit hasExited(NewPosition);
        delete this;
        return;
    }
}
