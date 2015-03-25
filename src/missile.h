/**
  \file
  \brief    Déclaration de la classe Missile.
  \author   Clark Winkelmann
  \date     février 2014
*/
#ifndef MISSILE_H
#define MISSILE_H

#include "sprite.h"

class Hero;

//!
//! \brief Classe qui définit un missile.
//!
//! Cette classe est une extension de la classe Sprite qui ajoute les propriétés, fonctions et signaux propres au Missile.
//!
class Missile : public Sprite
{
    Q_OBJECT
public:
    Missile(QPointF Position, float Angle, float Speed, float Damage, QGraphicsItem* pParent = 0);

    void setGameZone(QRectF GameZone);
    void setHero(Hero* pHero);

    Hero* getHero();
    float getDamage();

    enum { MissileItemType = UserType + 20 };
    virtual int type() const { return MissileItemType; }

public slots:
    void update(int ElapsedTimeInMilliSeconds);

signals:
    //! \brief Annonce que le missile s'est déplacé
    //! \param NewPosition Nouvelle position du missile
    void hasMoved(QPointF NewPosition);
    //! \brief Annonce que le missile a quitté la zone de jeu
    //! \param LastPosition Dernière position avant de sortir
    void hasExited(QPointF LastPosition);

private:
    float m_Speed;
    float m_Damage;
    int   m_HitsLeft;

    Hero* m_pHero;

    QRectF m_GameZone;
};

#endif // MISSILE_H
