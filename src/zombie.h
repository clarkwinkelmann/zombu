/**
  \file
  \brief    Déclaration de la classe Zombie.
  \author   Clark Winkelmann
  \date     février 2014
*/
#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <QTime>

#include "sprite.h"

//!
//! \brief Classe qui définit un Zombie.
//!
//! Cette classe est une extension de la classe Sprite qui ajoute les propriétés, fonctions et signaux propres au Zombie.
//!
class Zombie : public Sprite
{
    Q_OBJECT
public:
    Zombie(QPointF Target, QGraphicsItem* pParent = 0);
    ~Zombie();

    void hurt(float DamagePoints);
    bool attack();

    void setLife(float Life);
    void setSpeed(float Speed);
    void setAttackDamages(float Damages);
    void setAttackInterval(float Interval);

    void setObstacleZone(QList<QRectF> ObstacleList);
    void addObstacleZone(QRectF ObstacleZone);

    QGraphicsRectItem* getLifeMeter();
    QGraphicsRectItem* getLifeMeterTotal();

    enum { ZombieItemType = UserType + 30 };
    virtual int type() const { return ZombieItemType; }

public slots:
    void setTarget(QPointF NewTarget);
    void update(int ElapsedTimeInMilliSeconds);

signals:
    //! \brief Annonce que le zombie a été blessé
    //! \param NewLife Nouvelle vie en %
    void beenHurt(float NewLife); // EN %
    //! \brief Annonce que le zombie s'est déplacé
    //! \param NewPosition Nouvelle position du zombie
    void hasMoved(QPointF NewPosition);
    //! \brief Annonce que le zombie a été touché
    //! \param HurtPosition Position du zombie lors du coup
    void beenShot(QPointF HurtPosition);
    //! \brief Annonce que le zombie est mort
    //! \param DeathPosition Position du zombie à la mort
    void beenDead(QPointF DeathPosition);

private:
    void move(float Angle,float Distance);
    void updateLifeMeter(QPointF TopCenterPosition);
    void testCollisions();
    void genNextTempTarget(float Distance);

    float m_Life;           // En points de vie
    float m_TotalLife;      // En points de vie
    float m_AttackDamage;   // En points de vie
    float m_Speed;          // En px par seconde
    float m_MaxSpeed;       // En px par seconde
    float m_AttackInterval; // En secondes
    float m_TempTargetInterval; // En secondes

    // Cibles
    QPointF m_Target;        // Position de la cible réelle
    QPointF m_TempTarget;    // Cible temporaire à proximité de la cible réelle
    QPointF m_CurrentTarget; // Cible actuelle, se rapproche lentement de la cible temporaire

    // Chronomètres
    QTime m_LastAttackTime;
    QTime m_LastTempTargetTime; // Temps depuis le dernier choix de cible du déplacement "aléatoire"

    // Zones dans lequelles le zombie ne peut pas aller
    QList<QRectF> m_ObstacleZone;

    // Barre de vie
    QGraphicsRectItem* m_pLifeMeter;
    QGraphicsRectItem* m_pLifeMeterTotal; // Arrière-plan
};

#endif // ZOMBIE_H
