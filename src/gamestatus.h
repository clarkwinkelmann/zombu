/**
  \file
  \brief    Déclaration de la classe GameStatus.
  \author   Clark Winkelmann
  \date     février 2014
*/
#ifndef GAMESTATUS_H
#define GAMESTATUS_H

#include <QList>
#include <QStringList>

//!
//! \brief Classe qui contient l'état actuel de la partie et qui est envoyé à l'interface lors de la mise à jour.
//!
class GameStatus
{
public:
    GameStatus();
    int   getLevel();
    float getLife();
    float getMunitions();
    int   getScore();
    int   getRound();
    float getRoundProgress();

    int   getRoundDamages();
    int   getRoundShoots();
    int   getRoundHits();
    int   getRoundKills();
    QStringList getFoundLetters();

    void  setLevel(int Level, int Round);
    void  setLife(float Life);
    void  setMunitions(float Munitions);
    void  setRoundProgress(float Progress);
    void  setScore(int Score);

    void  modScore(int Score);
    void  modRoundDamages(int Damages);
    void  modRoundShoots(int Shoots);
    void  modRoundHits(int Hits);
    void  modRoundKills(int Kills);

    void  addFoundLetter(QString Letter);
    void  popLetter(QString Letter);

    void  resetCurrentRoundStats();

private:
    int   m_Level;
    float m_Life;
    float m_Munitions;
    int   m_Round;
    float m_RoundProgress;
    int   m_Score;

    int   m_RoundDamages;
    int   m_RoundShoots;
    int   m_RoundHits;
    int   m_RoundKills;

    QStringList m_FoundLetters;
};

#endif // GAMESTATUS_H
