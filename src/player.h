/**
  \file
  \brief    Déclaration de la classe Player.
  \author   Clark Winkelmann
  \date     mars 2014
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "hero.h"
#include "zombie.h"
#include "amelioration.h"
#include "wordlibrary.h"

//!
//! \brief Classe qui définit un joueur.
//!
//! Elle permet de conserver l'état d'un joueur avec des améliorations entre les parties, et de créer un héros à partir des informations stockées.
//!
class Player
{
public:
    //! \brief Difficulté du jeu.
    enum GameDifficulty_e{
        Hardcore,
        Insane,
        Impossible
    };

    //! \brief Différentes améliorations possibles.
    enum Amelioration_e{
        Amelioration_Life,
        Amelioration_Munitions,
        Amelioration_Speed,
        Amelioration_ShootInterval,
        Amelioration_LAST_ITEM
    };

    //! \brief Représente "aucune lettre".
    static const QString NoLetter;

    Player(GameDifficulty_e GameDifficulty_e);
    ~Player();
    Hero* createHero();
    Zombie* createZombie(QPointF Position);

    void setScore(int Score);
    void setLevel(int Level, int Round);
    void nextLevel();
    void nextRound();

    int getScore();
    int getLevel();
    int getRound();
    int getRequiredScore(int Level);
    QSize getSceneSize();
    float getZombieSpawnInterval();
    int getRoundTotalZombies();

    QString pickLetter();

    void modScore(int Points);
    bool canLevelUp();

    MysteryWord::TryStatus_e tryLetter(Amelioration_e Amelioration,QString Letter);
    QStringList getTestedLetters(Amelioration_e Amelioration);
    QStringList getMysteryWord(Amelioration_e Amelioration);
    int getRemainingTries(Amelioration_e Amelioration);
    bool foundWord(Amelioration_e Amelioration);
    bool canUpdate(Amelioration_e Amelioration);
    float compareWithNextAmelioration(Amelioration_e Amelioration);

private:
    GameDifficulty_e m_GameDifficulty;
    int m_Score;
    int m_Level;
    int m_Round;

    Amelioration m_Ameliorations[Amelioration_LAST_ITEM];
    WordLibrary* m_pWordLibrary;
};

#endif // PLAYER_H
