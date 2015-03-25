/**
  \file
  \brief    Déclaration de la classe Zombie.
  \author   Clark Winkelmann
  \date     février 2014
*/
#include "zombie.h"

#include <QtMath>
#include <QBrush>

#include "sprite.h"
#include "hero.h"
#include "missile.h"
#include "resources.h"
#include "libzombu.h"

// Vitesse
const float ZOMBIE_SPEED_RECOVERY_PER_TICK = 0.01; // Proportion de la vitesse à garder lors d'un coup

// Barre de vie
const float ZOMBIE_LIFE_METER_WIDTH = 30; // En px
const float ZOMBIE_LIFE_METER_HEIGHT = 5; // En px

// Distance minimum entre la cible et le zombie
const float ZOMBIE_CLOSER_DISTANCE_TO_TARGET = 50;

// Cibles intermédiaires
const float ZOMBIE_MAX_TEMPTARGET_INTERVAL = 1; // En secondes
const float ZOMBIE_TEMPTARGET_INTERVAL_POSSIBILITIES = 10; // Possibilités différentes d'interval

// Précision entre la cible réelle et intermédiaire
const float ZOMBIE_TEMPTARGET_PRECISION = 0.0025; // plus grand = moins précis

// Correction pour se rapprocher de la prochaine cible
const float ZOMBIE_TARGET_CORRECTION_FACTOR = 20; //Plus grand = moins rapide


//!
//! \brief Création de l'objet Zombie.
//! \param Target Cible que le zombie va chercher à atteindre
//! \param pParent Objet parent
//!
Zombie::Zombie(QPointF Target, QGraphicsItem *pParent) : Sprite(pParent)
{
    m_Life = 0;
    m_TotalLife = 0;
    m_AttackDamage = 0;
    m_Speed = 0;
    m_MaxSpeed = 0;
    m_AttackInterval = 0;
    m_TempTargetInterval = ZOMBIE_MAX_TEMPTARGET_INTERVAL;

    // Cibles
    m_Target = Target;
    m_TempTarget = Target;
    m_CurrentTarget = Target;

    // Chronomètres
    m_LastAttackTime.start();
    m_LastTempTargetTime.start();

    // Barre de vie
    m_pLifeMeter      = new QGraphicsRectItem(0,0,ZOMBIE_LIFE_METER_WIDTH,ZOMBIE_LIFE_METER_HEIGHT);
    m_pLifeMeterTotal = new QGraphicsRectItem(0,0,ZOMBIE_LIFE_METER_WIDTH,ZOMBIE_LIFE_METER_HEIGHT);
    m_pLifeMeter      ->setZValue(200);
    m_pLifeMeterTotal ->setZValue(180);
    m_pLifeMeter      ->setBrush(QBrush(Qt::green));
    m_pLifeMeterTotal ->setBrush(QBrush(Qt::red));
    m_pLifeMeter      ->setOpacity(0.5);
    m_pLifeMeterTotal ->setOpacity(0.5);
    m_pLifeMeter      ->hide();
    m_pLifeMeterTotal ->hide();

    // Images du sprite
    int AnimationFrames = 5;
    for(int i=1;i<=AnimationFrames*2-2;i++){
        addAnimationFrame(QString(GameFramework::imagesPath()+"zombie%1.png").arg(i > AnimationFrames ? AnimationFrames-(i % AnimationFrames) : i));
    }
    setAnimationSpeed(130);
    startAnimation();

    // Z index
    setZValue(100);

    // Origine au milieu
    setTransformOriginPoint(width()/2, height()/2);
}

//!
//! \brief Détruit l'objet Zombie.
//!
Zombie::~Zombie(){
    delete m_pLifeMeter;
    delete m_pLifeMeterTotal;
}

//!
//! \brief Retire des points de vie au zombie.
//! \param DamagePoints Nombre de points de vie à retirer
//!
//! Retire des points de vie au zombie. Si la vie devient nulle, le signal beenDead est émis et l'objet est supprimé.
//!
void Zombie::hurt(float DamagePoints){
    m_Life -= DamagePoints;
    if(m_Life < 0){
        m_Life = 0;
    }

    emit beenHurt(m_Life / m_TotalLife * 100);
    emit beenShot(pos());

    if(m_Life == 0){
        emit beenDead(pos());
        delete this;
        return;
    }else{
        m_Speed /= 2;
    }
}

//!
//! \brief Fait attaquer le zombie.
//!
//! Si le zombie est en mesure d'attaquer, la fonction retourne vrai et définit que le zombie a attaqué. Sinon elle retourne faux.
//!
bool Zombie::attack(){
    if(m_LastAttackTime.elapsed() > m_AttackInterval*1000){
        m_LastAttackTime.start();
        return true;
    }
    return false;
}

//!
//! \brief Définit la vie du zombie.
//! \param Life Points de vie
//!
void Zombie::setLife(float Life){
    m_Life = Life;
    m_TotalLife = Life;
}

//!
//! \brief Définit la vitesse du zombie.
//! \param Speed Vitesse en px par seconde
//!
void Zombie::setSpeed(float Speed){
    m_Speed = Speed;
    m_MaxSpeed = Speed;
}

//!
//! \brief Définit les dégâts causés par le zombie.
//! \param Damages Points de vie
//!
void Zombie::setAttackDamages(float Damages){
    m_AttackDamage = Damages;
}

//!
//! \brief Définit l'interval entre chaque coup du zombie.
//! \param Interval Interval en secondes
//!
void Zombie::setAttackInterval(float Interval){
    m_AttackInterval = Interval;
}

//!
//! \brief Définit les zones d'obstacles.
//! \param ObstacleList Liste de zones à bloquer
//!
void Zombie::setObstacleZone(QList<QRectF> ObstacleList){
    m_ObstacleZone = ObstacleList;
}

//!
//! \brief Ajoute une zone d'obstacle.
//! \param ObstacleZone Zone à bloquer
//!
void Zombie::addObstacleZone(QRectF ObstacleZone){
    m_ObstacleZone.append(ObstacleZone);
}

//!
//! \brief Retourne un pointeur vers l'objet repprésentant la vie du zombie.
//! \return Pointeur vers l'objet représentant la vie du zombie
//!
QGraphicsRectItem* Zombie::getLifeMeter(){
    return m_pLifeMeter;
}

//!
//! \brief Retourne un pointeur vers l'objet repprésentant la vie totale du zombie.
//! \return Pointeur vers l'objet représentant la vie totale du zombie
//!
QGraphicsRectItem* Zombie::getLifeMeterTotal(){
    return m_pLifeMeterTotal;
}

//!
//! \brief Définit une nouvelle cible pour le zombie.
//! \param NewTarget Position de la cible
//!
//! Relier ce signal au déplacement du héros pour le suivre.
//!
void Zombie::setTarget(QPointF NewTarget){
    m_Target = NewTarget;
}

//!
//! \brief Met à jour les éléments du zombie.
//! \param ElapsedTimeInMilliSeconds Temps depuis le dernier tick
//!
void Zombie::update(int ElapsedTimeInMilliSeconds){
    // Réaccélérer
    m_Speed += m_MaxSpeed*ZOMBIE_SPEED_RECOVERY_PER_TICK;
    if(m_Speed > m_MaxSpeed){
        m_Speed = m_MaxSpeed;
    }

    // Rapprocher la cible actuelle de la cible temporaire
    m_CurrentTarget += (m_TempTarget-m_CurrentTarget)/ZOMBIE_TARGET_CORRECTION_FACTOR;

    float MoveDistance = static_cast<float>(ElapsedTimeInMilliSeconds) * m_Speed / 1000;

    QPointF Direction = m_CurrentTarget-pos();

    float TargetDistance = qSqrt(Direction.x()*Direction.x()+Direction.y()*Direction.y());

    // Se tourner en direction de la cible
    float Angle = qAtan2(Direction.y(),Direction.x()) + M_PI/2;
    setRotation(qRadiansToDegrees(Angle));

    // On ne continue plus à avancer si l'on est trop près du joueur
    // -> éviter de faire des tours sur soi-même
    if(TargetDistance > ZOMBIE_CLOSER_DISTANCE_TO_TARGET){
        move(Angle,MoveDistance);
    }

    // Calculer le prochain déplacement "aléatoire"
    genNextTempTarget(TargetDistance);

    // Tester les collisions
    testCollisions();
    // DOIT ETRE LA DERNIÈRE ACTION A CAUSE DU DELETE
}

//!
//! \brief Déplace le zombie dans la direction indiquée.
//! \param Angle Angle en Radians
//! \param Distance Distance en px sur laquelle se déplacer
//!
//! Déplace le zombie sur la distance et dans la direction indiquée en tenant compte des obstacles.
//!
void Zombie::move(float Angle, float Distance){
    float MoveX = qSin(Angle) * Distance;
    float MoveY = qCos(Angle) * Distance * -1;

    QPointF NewPosition = pos()+QPointF(MoveX,MoveY);

    // On teste en prenant le centre du zombie
    QPointF NewPositionTest = NewPosition+QPointF(width()/2,height()/2);

    // Empêcher l'accès aux zones d'obstacle
    foreach(QRectF Rectangle,m_ObstacleZone){
        if(Rectangle.contains(NewPositionTest)){
            NewPosition = LibZombu::getPositionOutsideRect(NewPositionTest,Rectangle) - QPointF(width()/2,height()/2);
        }
    }

    setPos(NewPosition);

    // Mettre à jour et afficher la barre de vie si vie < 100%
    updateLifeMeter(QPointF(NewPositionTest.x(),NewPosition.y()));

    emit hasMoved(NewPosition);
}

//!
//! \brief Met à jour la valeur et la position de la barre de vie.
//!
void Zombie::updateLifeMeter(QPointF TopCenterPosition){
    if(m_Life < m_TotalLife){
        QPointF LifeMeterPosition = QPointF(TopCenterPosition.x()-ZOMBIE_LIFE_METER_WIDTH/2, TopCenterPosition.y()-ZOMBIE_LIFE_METER_HEIGHT*2);
        m_pLifeMeter      ->setRect(QRectF(LifeMeterPosition,  QSizeF(m_Life / m_TotalLife*ZOMBIE_LIFE_METER_WIDTH, ZOMBIE_LIFE_METER_HEIGHT)));
        m_pLifeMeterTotal ->setRect(QRectF(LifeMeterPosition,  QSizeF(ZOMBIE_LIFE_METER_WIDTH, ZOMBIE_LIFE_METER_HEIGHT)));
        m_pLifeMeter      ->show();
        m_pLifeMeterTotal ->show();
    }
}

//!
//! \brief Teste si le zombie est en collision avec un autre objet.
//!
//! Teste les collisions et effectue les actions correpondantes.
//!
void Zombie::testCollisions(){
    QList<QGraphicsItem*> CollidingItems = collidingItems();
    foreach(QGraphicsItem* pItem, CollidingItems){
        if(pItem->type() == Missile::MissileItemType){
            // Touché par un missile
            // -> Blesser le zombie + supprimer le missile
            Missile* pMissile = static_cast<Missile*>(pItem);
            hurt(pMissile->getDamage());
            delete pMissile;
            // On quitte la fonction après hurt car il aurait pu y avoir un delete
            return;
        }else if(pItem->type() == Hero::HeroItemType){
            // Touche un héros
            // -> Si peut ataquer -> Blesser le héros
            if(attack()){
                Hero* pHero = static_cast<Hero*>(pItem);
                pHero->hurt(m_AttackDamage);
            }
        }
    }
}

//!
//! \brief Calcule la nouvelle cible temporaire si le délai est écoulé.
//!
void Zombie::genNextTempTarget(float Distance){
    if(m_LastTempTargetTime.elapsed() > m_TempTargetInterval*1000){
        // Nouvel interval
        m_TempTargetInterval =
                static_cast<float>(qrand() % static_cast<int>(ZOMBIE_MAX_TEMPTARGET_INTERVAL * ZOMBIE_TEMPTARGET_INTERVAL_POSSIBILITIES)+1)
                / ZOMBIE_TEMPTARGET_INTERVAL_POSSIBILITIES;
        m_LastTempTargetTime.start();

        // Nouvelle position
        // Plus on est loin, moins on est précis
        // Donne une différence positive ou négative
        m_TempTarget = m_Target + ZOMBIE_TEMPTARGET_PRECISION * QPointF(qrand()%static_cast<int>(Distance*2)-Distance,
                                                                        qrand()%static_cast<int>(Distance*2)-Distance);
    }
}
