/**
  \file
  \brief    Déclaration de la classe GameStatus.
  \author   Clark Winkelmann
  \date     février 2014
*/
#include "gamestatus.h"

//!
//! \brief Création de l'objet GameStatus.
//!
GameStatus::GameStatus()
{
    m_Level = 1;
    m_Life = 0;
    m_Munitions = 0;
    m_Round = 1;
    m_RoundProgress = 0;
    m_Score = 0;

    resetCurrentRoundStats();
}

//!
//! \brief Récupère le niveau.
//! \return Numéro du niveau en cours
//!
int GameStatus::getLevel(){
    return m_Level;
}

//!
//! \brief Récupère la vie.
//! \return Vie en %
//!
float GameStatus::getLife(){
    return m_Life;
}

//!
//! \brief Récupère les munitions.
//! \return Stock de munitions en %
//!
float GameStatus::getMunitions(){
    return m_Munitions;
}

//!
//! \brief Récupère le score.
//! \return Score Score en points
//!
int GameStatus::getScore(){
    return m_Score;
}

//!
//! \brief Récupère le round.
//! \return Numéro du round en cours
//!
int GameStatus::getRound(){
    return m_Round;
}

//!
//! \brief Récupère la progression du round.
//! \return Progression du round en cours en %
//!
float GameStatus::getRoundProgress(){
    return m_RoundProgress;
}

//!
//! \brief Récupère les stats de dégâts.
//! \return Nombre de dégâts
//!
int GameStatus::getRoundDamages(){
    return m_RoundDamages;
}

//!
//! \brief Récupère les stats de tir.
//! \return Nombre de tirs
//!
int GameStatus::getRoundShoots(){
    return m_RoundShoots;
}

//!
//! \brief Récupère les stats de coups donnés.
//! \return Nombre de coups
//!
int GameStatus::getRoundHits(){
    return m_RoundHits;
}

//!
//! \brief Récupère les stats de zombies tués.
//! \return Nombre de zombies
//!
int GameStatus::getRoundKills(){
    return m_RoundKills;
}

//!
//! \brief Récupère la liste des lettres trouvées
//! \return Liste de lettres
//!
QStringList GameStatus::getFoundLetters(){
    return m_FoundLetters;
}

//!
//! \brief Définit le niveau et round.
//! \param Level Niveau à définir
//! \param Round Round à définir
//!
void GameStatus::setLevel(int Level, int Round){
    m_Level = Level;
    m_Round = Round;
}

//!
//! \brief Définit le niveau de vie.
//! \param Life Nouvelle vie en %
//!
void GameStatus::setLife(float Life){
    if(Life >= 0){
        m_Life = Life;
    }
}

//!
//! \brief Définit le stock de munitions.
//! \param Munitions Nouveau stock en %
//!
void GameStatus::setMunitions(float Munitions){
    if(Munitions >= 0){
        m_Munitions = Munitions;
    }
}

//!
//! \brief Définit la progression du round.
//! \param Progress Nouvelle progression en %
//!
void GameStatus::setRoundProgress(float Progress){
    if(Progress >= 0){
        m_RoundProgress = Progress;
    }
}

//!
//! \brief Définit le score du round en cours.
//! \param Score Nouveau score
//!
void GameStatus::setScore(int Score){
   m_Score = Score;
}

//!
//! \brief Modifie le score de la partie en cours.
//! \param Score Différence de score
//!
void GameStatus::modScore(int Score){
    m_Score += Score;
}

//!
//! \brief Modifie les stats de dégâts.
//! \param Damages Différence de dégâts
//!
void GameStatus::modRoundDamages(int Damages){
    m_RoundDamages += Damages;
}

//!
//! \brief Modifie les stats de tir.
//! \param Shoots Différence de tir
//!
void GameStatus::modRoundShoots(int Shoots){
    m_RoundShoots += Shoots;
}

//!
//! \brief Modifie les stats de coups donnés.
//! \param Hits Différence de coups
//!
void GameStatus::modRoundHits(int Hits){
    m_RoundHits += Hits;
}

//!
//! \brief Modifie les stats de zombies tués.
//! \param Kills Différence de morts
//!
void GameStatus::modRoundKills(int Kills){
    m_RoundKills += Kills;
}

//!
//! \brief Ajoute une lettre à la liste des lettres trouvées.
//! \param Letter Lettre trouvée
//!
void GameStatus::addFoundLetter(QString Letter){
    m_FoundLetters.append(Letter);
}

//!
//! \brief Retire une lettre des lettres possédées.
//! \param Letter Lettre à utiliser
//!
void GameStatus::popLetter(QString Letter){
    m_FoundLetters.removeOne(Letter);
}

//!
//! \brief Remet les statistiques de la partie à zéro.
//!
void GameStatus::resetCurrentRoundStats(){
    m_RoundDamages = 0;
    m_RoundShoots = 0;
    m_RoundHits = 0;
    m_RoundKills = 0;
}
