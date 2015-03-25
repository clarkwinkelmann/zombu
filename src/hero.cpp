/**
  \file
  \brief    Déclaration de la classe Hero.
  \author   Clark Winkelmann
  \date     février 2014
*/
#include "hero.h"

#include <QtMath>

#include "missile.h"
#include "obstacle.h"
#include "resources.h"
#include "libzombu.h"

// Missiles
const float HERO_MISSILE_DAMAGE = 130; // En points de vie
const float HERO_MISSILE_SPEED = 500;  // En px par seconde

// Vitesse
const float HERO_TURN_SPEED = 300;                // En degrés par seconde
const float HERO_SPEED_RECOVERY_PER_TICK = 0.005; // En proportion de la vitesse totale
const float HERO_SPEED_HURT_FACTOR = 0.6;         // Proportion de la vitesse à garder lors d'un coup

// Distance devant le héros où apparaissent les missiles
const float HERO_MISSILE_SPAWN_DISTANCE = 50; // En px

//!
//! \brief Création de l'objet Hero.
//! \param pParent Objet parent
//!
//! L'objet Hero représente un héros qui est manipulé par le joueur et peut effectuer différentes actions avec le jeu.
//!
Hero::Hero(QGraphicsItem *pParent) : Sprite(pParent)
{
    m_MaxLife = 0;
    m_Life = 0;
    m_LifeRecoveryPerSecond = 0;
    m_LifeRecoveryDelay = 0;

    m_Protection = 0;

    m_MaxMunitions = 0;
    m_Munitions = 0;
    m_MunitionRecoveryPerSecond = 0;
    m_MunitionRecoveryDelay = 0;

    m_ShootInterval = 0;
    m_MissileSpeed = HERO_MISSILE_SPEED;
    m_MissileDamage = HERO_MISSILE_DAMAGE;

    m_MaxSpeed = 0;
    m_Speed = 0;
    m_TurnSpeed = HERO_TURN_SPEED;

    m_LastHurtTime.start();
    m_LastShootTime.start();

    m_TurnLeft  = false;
    m_TurnRight = false;
    m_GoAhead   = false;
    m_GoBack    = false;

    m_GameZone = QRectF(); // Définir défaut

    // Images du sprite
    int AnimationFrames = 5;
    for(int i=1;i<=AnimationFrames*2-2;i++){
        addAnimationFrame(QString(GameFramework::imagesPath()+"hero%1.png").arg(i > AnimationFrames ? AnimationFrames-(i % AnimationFrames) : i));
    }
    setAnimationSpeed(130);
    startAnimation();

    // Z index
    setZValue(200);

    // Origine au milieu
    setTransformOriginPoint(width()/2, height()/2);
}

// Gestion des déplacements
//! \brief Active ou désactive la rotation à gauche.
//! \param Start Si oui activer, si faux désactiver
void Hero::turnLeft (bool Start){ m_TurnLeft  = Start; }

//! \brief Active ou désactive la rotation à droite.
//! \param Start Si oui activer, si faux désactiver
void Hero::turnRight(bool Start){ m_TurnRight = Start; }

//! \brief Active ou désactive le déplacement avant.
//! \param Start Si oui activer, si faux désactiver
void Hero::goAhead  (bool Start){ m_GoAhead   = Start; }

//! \brief Active ou désactive le déplacement arrière.
//! \param Start Si oui activer, si faux désactiver
void Hero::goBack   (bool Start){ m_GoBack    = Start; }

//!
//! \brief Tire un coup de feu.
//! \return Un pointeur vers le missile créé
//!
//! Créé un missile et définit sa direction, vitesse et dégâts selon l'état actuel du héros.
//!
Missile* Hero::shoot(){
    if(m_LastShootTime.elapsed() > m_ShootInterval*1000 && m_Munitions >= 1 && m_Life > 0){
        float AngleInRadians = qDegreesToRadians(rotation());
        // Le missile est positionné devant le joueur pour éviter qu'il neHERO_MISSILE_SPAWN_DISTANCE le joueur est collé
        Missile* pMissile = new Missile(pos() + QPointF(width()/2,height()/2) + QPointF(qSin(AngleInRadians),qCos(AngleInRadians)*-1) * HERO_MISSILE_SPAWN_DISTANCE,
                                        rotation(),m_MissileSpeed,m_MissileDamage);
        pMissile->setGameZone(m_GameZone);
        pMissile->setHero(this);
        m_Munitions -= 1;
        m_LastShootTime.start();
        emit hasShooted(getMunitions());
        return pMissile;
    }
    return 0;
}

//!
//! \brief Donne des dégâts au héros.
//! \param DamagePoints Dégâts en points de vie
//!
//! Donne le nombre de dégâts indiqué au héros. Dans tous les cas les signaux beenHurt et beenShot sont émis.
//! Si le joueur n'a plus de vie, le signal beenDead est émis.
//!
void Hero::hurt(int DamagePoints){
    // Considérons qu'on mort ne peut pas être davantage blessé
    if(m_Life <= 0){
        return;
    }

    m_Life -= (100-m_Protection) / 100 * static_cast<float>(DamagePoints);
    m_LastHurtTime.start();
    if(m_Life < 0){
        m_Life = 0;
    }
    emit beenHurt(getLife());
    emit beenShot(pos());
    if(m_Life == 0){
        emit beenDead(pos());
    }else{
        m_Speed *= HERO_SPEED_HURT_FACTOR;
    }
}

//!
//! \brief Définit la vie du héros.
//! \param Life Points de vie
//!
void Hero::setLife(float Life){
    m_Life = Life;
    m_MaxLife = Life;
}

//!
//! \brief Définit la récupération de vie du héros.
//! \param Life Points de vie par seconde
//!
void Hero::setLifeRecoveryPerSecond(float Life){
    m_LifeRecoveryPerSecond = Life;
}

//!
//! \brief Définit le délai de récupération de vie du héros.
//! \param Delay Délai en secondes
//!
void Hero::setLifeRecoveryDelay(float Delay){
    m_LifeRecoveryDelay = Delay;
}

//!
//! \brief Définit la protection du héros.
//! \param Protection Dégâts à retirer en %
//!
void Hero::setProtection(float Protection){
    m_Protection = Protection;
}

//!
//! \brief Définit le stock de munitions du héros.
//! \param Munitions Nombre de munitions
//!
void Hero::setMunitions(float Munitions){
    m_Munitions = Munitions;
    m_MaxMunitions = Munitions;
}

//!
//! \brief Définit la récupération de munitions du héros.
//! \param Munitions Nombre de munitions par seconde
//!
void Hero::setMunitionsRecoveryPerSecond(float Munitions){
    m_MunitionRecoveryPerSecond = Munitions;
}

//!
//! \brief Définit le délai de récupération de munitions du héros.
//! \param Delay Délai en secondes
//!
void Hero::setMunitionsRecoveryDelay(float Delay){
    m_MunitionRecoveryDelay = Delay;
}

//!
//! \brief Définit l'interval entre 2 tirs du héros.
//! \param Interval Interval en secondes
//!
void Hero::setShootInterval(float Interval){
    m_ShootInterval = Interval;
}

//!
//! \brief Définit la vitesse des missiles tirés par le héros.
//! \param Speed Vitesse en px par seconde
//!
void Hero::setMissileSpeed(float Speed){
    m_MissileSpeed = Speed;
}

//!
//! \brief Définit les dégâts causés par un missile tiré par le héros.
//! \param Damage Dégâts en points de vie
//!
void Hero::setMissileDamage(float Damage){
    m_MissileDamage = Damage;
}

//!
//! \brief Définit la vitesse du héros.
//! \param Speed Vitesse en px par seconde
//!
void Hero::setSpeed(float Speed){
    m_Speed = Speed;
    m_MaxSpeed = Speed;
}

//!
//! \brief Définit la vitesse de rotation du héros.
//! \param Speed Vitesse en degrés par seconde
//!
void Hero::setTurnSpeed(float Speed){
    m_TurnSpeed = Speed;
}

//!
//! \brief Ajoute une zone de jeu au héros.
//! \param GameZone Rectangle de la zone de jeu
//!
//! La zone limitera les déplacements du joueur et sera transmise aux missiles lors de leur lancement.
//!
void Hero::setGameZone(QRectF GameZone){
    m_GameZone = GameZone;
}

//!
//! \brief Définit les zones d'obstacles.
//! \param ObstacleList Zones à bloquer
//!
void Hero::setObstacleZone(QList<QRectF> ObstacleList){
    m_ObstacleZone = ObstacleList;
}

//!
//! \brief Ajoute une zone d'obstacle.
//! \param ObstacleZone Nouvelle zone à bloquer
//!
void Hero::addObstacleZone(QRectF ObstacleZone){
    m_ObstacleZone.append(ObstacleZone);
}

//!
//! \brief Retourne la vie du héros sous forme d'un pourcentage.
//! \return Vie en %
//!
float Hero::getLife(){
    return m_Life / m_MaxLife * 100;
}

//!
//! \brief Retourne le stock de munitions du héros sous forme d'un pourcentage.
//! \return Stock de munitions en %
//!
float Hero::getMunitions(){
    return m_Munitions / m_MaxMunitions * 100;
}

//!
//! \brief Met à jour le héros.
//! \param ElapsedTimeInMilliSeconds Temps écoulé depuis le dernier tick
//!
//! Met à jour les éléments du héros: direction, position, vie, munitions.
//! Cette fonction doit être connectée à un tick sur un timer externe.
//!
void Hero::update(int ElapsedTimeInMilliSeconds){
    // Un mort ne peut rien faire
    if(m_Life <= 0){
        return;
    }

    // Réaccélérer
    m_Speed += m_MaxSpeed*(m_Life/m_MaxLife/2+0.5)*HERO_SPEED_RECOVERY_PER_TICK;
    if(m_Speed > m_MaxSpeed){
        m_Speed = m_MaxSpeed;
    }

    // Faire tourner le héros sur lui-même
    if(m_TurnLeft || m_TurnRight){
        setRotation(rotation() + (
                        // Calcul des degrés à ajouter / retirer
                        ElapsedTimeInMilliSeconds * HERO_TURN_SPEED / 1000 * (m_TurnLeft ? -1 : 1)
                        ));
    }

    // Faire avancer le héros dans la direction qu'il regarde
    if(m_GoAhead || m_GoBack){
        float MoveDistance = static_cast<float>(ElapsedTimeInMilliSeconds) * m_Speed * (m_GoAhead ? 1 : -1) / 1000;

        move(qDegreesToRadians(rotation()),MoveDistance);
    }

    // Récupérer de la vie si le délai avant récupération est écoulé
    heal(ElapsedTimeInMilliSeconds);

    // Récupérer des munitions si le délai avant récupération est écoulé
    reload(ElapsedTimeInMilliSeconds);
}

//!
//! \brief Déplace le héros dans la direction indiquée.
//! \param Angle Angle en radians
//! \param Distance Distance en px sur laquelle se déplacer
//! Déplace le héros sur la distance et dans la direction indiquée en tenant compte de la zone de jeu et des obstacles.
//!
void Hero::move(float Angle, float Distance){
    float MoveX = qSin(Angle) * Distance;
    float MoveY = qCos(Angle) * Distance * -1;

    QPointF NewPosition = pos()+QPointF(MoveX,MoveY);

    // On teste en prenant le centre du héros
    QPointF NewPositionTest = NewPosition+QPointF(width()/2,height()/2);

    // Limiter le déplacement à la zone de jeu
    // Sans bloquer les déplacements "le long du mur"
    if(NewPositionTest.x() < m_GameZone.x()){ NewPosition.setX(m_GameZone.x()-width()/2); }else
        if(NewPositionTest.x() > m_GameZone.right()){ NewPosition.setX(m_GameZone.right()-width()/2); }
    if(NewPositionTest.y() < m_GameZone.y()){ NewPosition.setY(m_GameZone.y()-height()/2); }else
        if(NewPositionTest.y() > m_GameZone.bottom()){ NewPosition.setY(m_GameZone.bottom()-height()/2); }

    // Empêcher l'accès aux zones d'obstacle
    foreach(QRectF Rectangle,m_ObstacleZone){
        if(Rectangle.contains(NewPositionTest)){
            // On cherche la position du centre du héros en dehors de l'obstacle
            // puis on calcule la position du héros depuis son centre
            NewPosition = LibZombu::getPositionOutsideRect(NewPositionTest,Rectangle) - QPointF(width()/2,height()/2);
        }
    }

    setPos(NewPosition);
    emit hasMoved(NewPosition);
}

//!
//! \brief Redonne de la vie au joueur si le délai après coup est écoulé.
//!
void Hero::heal(int ElapsedTimeInMilliSeconds){
    if(m_LastHurtTime.elapsed() > m_LifeRecoveryDelay*1000 && m_Life < m_MaxLife){
        m_Life += ElapsedTimeInMilliSeconds * m_LifeRecoveryPerSecond / 1000;
        if(m_Life > m_MaxLife){
            m_Life = m_MaxLife;
        }
        emit hasHealed(getLife());
    }
}

//!
//! \brief Redonne des munitions au joueur si le délai après tir est écoulé.
//!
void Hero::reload(int ElapsedTimeInMilliSeconds){
    if(m_LastShootTime.elapsed() > m_MunitionRecoveryDelay*1000 && m_Munitions < m_MaxMunitions){
        m_Munitions += ElapsedTimeInMilliSeconds * m_MunitionRecoveryPerSecond / 1000;
        if(m_Munitions > m_MaxMunitions){
            m_Munitions = m_MaxMunitions;
        }
        emit hasReloaded(getMunitions());
    }
}
