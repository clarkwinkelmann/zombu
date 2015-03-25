/**
  \file
  \brief    Déclaration de la classe GameCore.
  \author   Clark Winkelmann
  \date     février 2014
*/
#ifndef GAMECORE_H
#define GAMECORE_H

#include <QObject>
#include <QList>
#include <QPointF>
#include <QTime>
#include <QRectF>
#include <QGraphicsTextItem>

#include "gamescene.h"
#include "sprite.h"
#include "hero.h"
#include "gamestatus.h"
#include "player.h"

class QGraphicsTransform;
class Sprite;

//!
//! \brief Classe qui gère toute la logique du jeu.
//!
//! Certains signaux sont relayés plus haut afin de mettre à jour la fenêtre.
//!
class GameCore : public QObject
{
    Q_OBJECT
public:
    explicit GameCore(GameScene* pScene, QObject *parent = 0);

    void keyPressed(int Key);
    void keyReleased(int Key);

    void tick(int ElapsedTimeInMilliSeconds);

    bool canLevelUp();
    int  getRequiredScore();

    void togglePause();
    void beginNextLevel();
    void beginRound();
    void newGame(Player::GameDifficulty_e Difficulty);

signals:
    //! \brief Annonce un nouveau tick.
    //! \param ElapsedTimeInMilliSeconds Temps depuis le dernier tick
    void gameTick(int ElapsedTimeInMilliSeconds);
    //! \brief Annonce un changement de statut de la partie.
    //! \param pGameStatus Pointeur vers le statut de la partie
    void updateStatus(GameStatus* pGameStatus);
    //! \brief Annonce la fin du round.
    //! \param pGameStatus Pointeur vers le statut de la partie
    //! \param pPlayer Pointeur vers l'objet Player du joueur
    void roundEnded(GameStatus* pGameStatus, Player* pPlayer);
    //! \brief Annonce la fin du round par mort.
    void gameOver();
    //! \brief Annonce le début d'un round.
    void roundStarted();

private:
    void createScene(int MaxWall, int MaxBox);
    void endRound();
    void emitUpdateStatus();
    bool inGame();

    void displayMessage(QPointF Position, QString Message);

    GameScene*  m_pScene;
    Player*     m_pPlayer;
    Hero*       m_pSuperHero;
    GameStatus* m_pGameStatus;
    QList<QRectF>      m_ObstacleZone; // Zones dans lequelles les joueurs/zombies ne peuvent pas aller
    QGraphicsTextItem* m_pPauseText;

    QTime m_LastZombieSpawnTime;
    float m_ZombieSpawnInterval; // En secondes
    int   m_RoundTotalZombies;
    int   m_RoundZombiesLeft;
    int   m_RoundZombiesAlive;

    bool  m_GamePaused;
    bool  m_RoundFinished;

private slots:
    void spillBlood(QPointF Position);

    void onStatusLifeChange(float NewLife);
    void onStatusMunitionsChange(float NewMunitions);
    void onStatusRoundChange();

    void onHeroShoot();
    void onHeroHurt();
    void onHeroDeath();
    void onZombieHurt();
    void onZombieDeath(QPointF Position);
};

#endif // GAMECORE_H
