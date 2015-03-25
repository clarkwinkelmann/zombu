/**
  \file
  \brief    Déclaration de la classe Hero.
  \author   Clark Winkelmann
  \date     février 2014
*/
#ifndef HERO_H
#define HERO_H

#include <QTime>

#include "sprite.h"

class Missile;

//!
//! \brief Classe qui définit un Héros (joueur).
//!
//! Cette classe est une extension de la classe Sprite qui ajoute les propriétés, fonctions et signaux propres au Héros.
//!
class Hero : public Sprite
{
    Q_OBJECT
public:
    Hero(QGraphicsItem* pParent = 0);

    void turnLeft(bool Start);
    void turnRight(bool Start);
    void goAhead(bool Start);
    void goBack(bool Start);

    Missile* shoot();
    void hurt(int DamagePoints);


    void setLife(float Life);
    void setLifeRecoveryPerSecond(float Life);
    void setLifeRecoveryDelay(float Delay);

    void setProtection(float Protection);

    void setMunitions(float Munitions);
    void setMunitionsRecoveryPerSecond(float Munitions);
    void setMunitionsRecoveryDelay(float Delay);

    void setShootInterval(float Interval);
    void setMissileSpeed(float Speed);
    void setMissileDamage(float Damage);

    void setSpeed(float Speed);
    void setTurnSpeed(float Speed);

    void setGameZone(QRectF GameZone);
    void setObstacleZone(QList<QRectF> ObstacleList);


    void addObstacleZone(QRectF ObstacleZone);


    float getLife(); // En %
    float getMunitions(); // En %

    enum { HeroItemType = UserType + 10 };
    virtual int type() const { return HeroItemType; }

public slots:
    void update(int ElapsedTimeInMilliSeconds);

signals:
    //! \brief Annonce que le joueur a récupéré de la vie
    //! \param NewLife Nouvelle vie en %
    void hasHealed(float NewLife);
    //! \brief Annonce que le joueur a perdu de la vie
    //! \param NewLife Nouvelle vie en %
    void beenHurt(float NewLife);
    //! \brief Annonce que le joueur a tiré
    //! \param NewMunitions Nouveau stock de munitions en %
    void hasShooted(float NewMunitions);
    //! \brief Annonce que le joueur a récupéré des munitions
    //! \param NewMunitions Nouveau stock de munitions en %
    void hasReloaded(float NewMunitions);
    //! \brief Annonce que le joueur s'est déplacé
    //! \param NewPosition Nouvelle position du joueur
    void hasMoved(QPointF NewPosition);
    //! \brief Annonce que le joueur a été touché
    //! \param HurtPosition Position du joueur lors du coup
    void beenShot(QPointF HurtPosition);
    //! \brief Annonce que le joueur est mort
    //! \param DeathPosition Position du joueur à la mort
    void beenDead(QPointF DeathPosition);

private:
    void move(float Angle, float Distance);
    void heal(int ElapsedTimeInMilliSeconds);
    void reload(int ElapsedTimeInMilliSeconds);


    float m_Life;                   // En points de vie
    float m_MaxLife;                // En points de vie
    float m_LifeRecoveryPerSecond;  // En points de vie
    float m_LifeRecoveryDelay;      // En secondes

    float m_Protection; // En % de dégâts à retirer

    float m_Munitions;                  // En nombre de tirs
    float m_MaxMunitions;               // En nombre de tirs
    float m_MunitionRecoveryPerSecond;  // En nombre de tirs
    float m_MunitionRecoveryDelay;      // En secondes

    float m_ShootInterval;   // En secondes
    float m_MissileSpeed;    // En px par secondes
    float m_MissileDamage;   // En points de vie

    float m_Speed;      // En px par seconde
    float m_MaxSpeed;   // En px par seconde
    float m_TurnSpeed;  // En degrés par seconde

    // Chronomètres
    QTime m_LastHurtTime;   // Dernier coup reçu
    QTime m_LastShootTime;  // Dernier coup tiré

    // Variables de commandes
    bool  m_TurnLeft;
    bool  m_TurnRight;
    bool  m_GoAhead;
    bool  m_GoBack;

    // Zone dans laquelle se déplace le joueur
    QRectF m_GameZone;

    // Zones dans lequelles le joueur ne peut pas aller
    QList<QRectF> m_ObstacleZone;
};

#endif // HERO_H
