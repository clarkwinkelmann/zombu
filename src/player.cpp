/**
  \file
  \brief    Déclaration de la classe Player.
  \author   Clark Winkelmann
  \date     mars 2014
*/
#include "player.h"

#include "resources.h"
#include "libzombu.h"

// Remplace "aucune lettre"
const QString Player::NoLetter("?");

// Score minimum pour passer au niveau suivant
const int SCORE_BASE = 2000;
// Score supplémentaire nécessaire par niveau
const int SCORE_PER_LEVEL = 2000;

//!
//! \brief Création de l'objet Player.
//! \param GameDifficulty Difficulté du jeu pour le joueur
//!
//! Créé les listes d'amélioration propres au joueur.
//!
Player::Player(Player::GameDifficulty_e GameDifficulty)
{
    m_GameDifficulty = GameDifficulty;
    m_Score = 0;
    m_Level = 1;
    m_Round = 1;

    switch(m_GameDifficulty){
    case Hardcore:
        m_Ameliorations[Amelioration_Life].setStages(QList<float>() << 200 << 300 << 400  << 500 << 600 << 700 << 800 << 900 << 1000 << 1100);
        m_Ameliorations[Amelioration_Munitions].setStages(QList<float>() << 200 << 250 << 300 << 350 << 400 << 500 << 600 << 700 << 800 << 1000);
        m_Ameliorations[Amelioration_Speed].setStages(QList<float>() << 220 << 240 << 260 << 280 << 300 << 320 << 340 << 360 << 380 << 400);
        m_Ameliorations[Amelioration_ShootInterval].setStages(QList<float>() << 0.1 << 0.09 << 0.08 << 0.07 << 0.06 << 0.05 << 0.04);
        break;
    case Insane:
        m_Ameliorations[Amelioration_Life].setStages(QList<float>() << 100 << 150 << 200  << 250 << 300 << 350 << 400 << 500 << 600 << 700);
        m_Ameliorations[Amelioration_Munitions].setStages(QList<float>() << 100 << 125 << 150 << 175 << 200 << 225 << 250 << 275 << 300 << 350);
        m_Ameliorations[Amelioration_Speed].setStages(QList<float>() << 200 << 220 << 240 << 260 << 280 << 300 << 320 << 340 << 360 << 380);
        m_Ameliorations[Amelioration_ShootInterval].setStages(QList<float>() << 0.1 << 0.09 << 0.08 << 0.07 << 0.06 << 0.05 << 0.04);
        break;
    case Impossible:
        m_Ameliorations[Amelioration_Life].setStages(QList<float>() << 100 << 120 << 140  << 160 << 180 << 200 << 220 << 240 << 260 << 280);
        m_Ameliorations[Amelioration_Munitions].setStages(QList<float>() << 50 << 75 << 100 << 125 << 150 << 175 << 200 << 250 << 300 << 350);
        m_Ameliorations[Amelioration_Speed].setStages(QList<float>() << 160 << 180 << 200 << 220 << 240 << 260 << 280 << 300 << 320 << 340);
        m_Ameliorations[Amelioration_ShootInterval].setStages(QList<float>() << 0.14 << 0.12 << 0.1 << 0.09 << 0.08 << 0.07);
        break;
    }

    m_pWordLibrary = new WordLibrary;

    m_pWordLibrary->loadFromFile(GameFramework::resourcesPath()+"mots_fr.txt");

    for(int i=0;i<Amelioration_LAST_ITEM;i++){
        m_Ameliorations[i].setWordLibrary(m_pWordLibrary);
    }
}

//!
//! \brief Suppression de l'objet Player.
//!
Player::~Player(){
    delete m_pWordLibrary;
}

//!
//! \brief Créé un objet Hero selon le niveau et la difficulté du joueur.
//! \return Pointeur vers l'objet Hero créé
//!
Hero* Player::createHero(){
    Hero* pHero = new Hero;

    pHero->setLife(m_Ameliorations[Amelioration_Life].getCurrentStageValue());
    pHero->setMunitions(m_Ameliorations[Amelioration_Munitions].getCurrentStageValue());
    pHero->setSpeed(m_Ameliorations[Amelioration_Speed].getCurrentStageValue());
    pHero->setShootInterval(m_Ameliorations[Amelioration_ShootInterval].getCurrentStageValue());

    float LifeRecoveryPerSecond = 10;
    float LifeRecoveryDelay = 6;
    float MunitionsRecoveryPerSecond = 8;
    float MunitionsRecoveryDelay = 0.3;

    switch(m_GameDifficulty){
    case Hardcore:
        break;
    case Insane:
        LifeRecoveryPerSecond = 5;
        LifeRecoveryDelay = 10;
        MunitionsRecoveryPerSecond = 5;
        MunitionsRecoveryDelay = 0.5;
        break;
    case Impossible:
        LifeRecoveryPerSecond = 3;
        LifeRecoveryDelay = 20;
        MunitionsRecoveryPerSecond = 3;
        MunitionsRecoveryDelay = 1;
        break;
    }

    pHero->setLifeRecoveryPerSecond(LifeRecoveryPerSecond);
    pHero->setLifeRecoveryDelay(LifeRecoveryDelay);
    pHero->setMunitionsRecoveryPerSecond(MunitionsRecoveryPerSecond);
    pHero->setMunitionsRecoveryDelay(MunitionsRecoveryDelay);

    return pHero;
}

//!
//! \brief Créé un objet Zombie selon le niveau et la difficulté du joueur.
//! \return Pointeur vers l'objet Zombie créé
//!
Zombie* Player::createZombie(QPointF Position){
    Zombie* pZombie = new Zombie(Position);
    pZombie->setLife(500);
    pZombie->setSpeed(50);
    pZombie->setAttackDamages(10);
    pZombie->setAttackInterval(0.5);
    return pZombie;
}

//!
//! \brief Définit le score du niveau.
//! \param Score Score en points
//!
void Player::setScore(int Score){
    m_Score = Score;
}

//!
//! \brief Force un niveau+round pour le jeu.
//! \param Level Niveau à forcer
//! \param Round Round à forcer
//! \see nextLevel(), nextRound()
//!
void Player::setLevel(int Level, int Round){
    m_Level = Level;
    m_Round = Round;
}

//!
//! \brief Passe au niveau suivant.
//!
void Player::nextLevel(){
    if(canLevelUp()){
        setLevel(++m_Level,1);
        setScore(0);
        for(int i=0;i<Amelioration_LAST_ITEM;i++){
            m_Ameliorations[i].levelUp();
        }
    }
}

//!
//! \brief Passe au round suivant.
//! \see setLevel()
//!
void Player::nextRound(){
    setLevel(m_Level,++m_Round);
}

//!
//! \brief Récupère le score.
//! \return Score en points
//! \see getScore()
//!
int Player::getScore(){
    return m_Score;
}

//!
//! \brief Récupère le niveau.
//! \return Numéro du niveau en cours
//! \see setLevel()
//!
int Player::getLevel(){
    return m_Level;
}

//!
//! \brief Récupère le round.
//! \return Numéro du round en cours
//! \see setLevel()
//!
int Player::getRound(){
    return m_Round;
}

//!
//! \brief Calcule le score nécessaire pour terminer un niveau.
//! \param Level Niveau pour lequel calculer
//! \return Score en points
//! \see canLevelUp()
//!
int Player::getRequiredScore(int Level){
    return SCORE_BASE+SCORE_PER_LEVEL*Level;
}

//!
//! \brief Retourne les dimensions de scène à utiliser en fonction de la difficulté.
//! \return Dimensions de la scène
//!
QSize Player::getSceneSize(){
    QSize Size(1500,700);
    switch(m_GameDifficulty){
    case Hardcore:
        break;
    case Insane:
        Size.setWidth(1300);
        break;
    case Impossible:
        Size.setWidth(1000);
        break;
    }
    return Size;
}

//!
//! \brief Retourne l'interval entre chaque spawn de zombie en fonction de la difficulté.
//! \return Interval en secondes
//!
float Player::getZombieSpawnInterval(){
    float Interval = 2;
    switch(m_GameDifficulty){
    case Hardcore:
        break;
    case Insane:
        Interval = 1;
        break;
    case Impossible:
        Interval = 0.8;
        break;
    }
    return Interval;
}

//!
//! \brief Retourne le nombre de zombies pour le round en fonction du niveau et de la difficulté.
//! \return Nombre de zombies
//!
int Player::getRoundTotalZombies(){
    float ZombiesPerLevel = 10;
    float ZombiesPerRound = 5;
    switch(m_GameDifficulty){
    case Hardcore:
        break;
    case Insane:
        ZombiesPerLevel = 10;
        ZombiesPerRound = 5;
        break;
    case Impossible:
        ZombiesPerLevel = 10;
        ZombiesPerRound = 5;
        break;
    }
    return ZombiesPerLevel*getLevel()+ZombiesPerRound*getRound();
}

//!
//! \brief Tire une lettre "aléatoire".
//! \return Lettre Lettre "aléatoire"
//!
//! La difficulté à trouver certaines lettres devra changer en fonction du niveau/difficulté.
//! (Pas encore implémenté).
//!
//! \see NoLetter
//!
QString Player::pickLetter(){
    int Probability = 1;
    switch(m_GameDifficulty){
    case Hardcore:
        break;
    case Insane:
        Probability = 2;
        break;
    case Impossible:
        Probability = 3;
        break;
    }
    if(qrand() % Probability == 0){
        return LibZombu::ALPHABET[qrand() % LibZombu::ALPHABET.size()];
    }
    return NoLetter;
}

//!
//! \brief Modifie le score.
//! \param Points Points à ajouter au score. Mettre un nombre négatif pour retirer des points
//! \see getScore()
//!
void Player::modScore(int Points){
    m_Score += Points;
}

//!
//! \brief Détermine si le joueur est en mesure de passer au niveau supérieur.
//! \return Retourne vrai si possible
//! \see getRequiredScore()
//!
bool Player::canLevelUp(){
    return m_Score >= getRequiredScore(m_Level);
}

//!
//! \brief Joue une lettre pour une cartaine amélioration.
//! \param Amelioration
//! \param Letter
//! \return Résulat du test TryStatus
//!
MysteryWord::TryStatus_e Player::tryLetter(Amelioration_e Amelioration,QString Letter){
    return m_Ameliorations[Amelioration].tryLetter(Letter);
}

//!
//! \brief Récupère la liste des lettres essayées pour une amélioration.
//! \param Amelioration
//! \return
//!
QStringList Player::getTestedLetters(Amelioration_e Amelioration){
    return m_Ameliorations[Amelioration].getTestedLetters();
}

//!
//! \brief Retourne le mot mystère tel qu'il est actuellement découvert pour une amélioration.
//! \param Amelioration
//! \return Liste de lettres
//!
QStringList Player::getMysteryWord(Amelioration_e Amelioration){
    return m_Ameliorations[Amelioration].getMysteryWord();
}

//!
//! \brief Récupère le nombre d'essais restants pour une amélioration.
//! \param Amelioration
//! \return Nombre d'essais
//!
int Player::getRemainingTries(Amelioration_e Amelioration){
    return m_Ameliorations[Amelioration].getRemainingTries();
}

//!
//! \brief Récupère si le mot de l'amélioration a été découvert.
//! \param Amelioration
//! \return Vrai si découvert
//!
bool Player::foundWord(Amelioration_e Amelioration){
    return m_Ameliorations[Amelioration].foundWord();
}

//!
//! \brief Détermine si l'amélioration peu être améliorée.
//! \param Amelioration
//! \return
//!
bool Player::canUpdate(Amelioration_e Amelioration){
    return m_Ameliorations[Amelioration].canUpdate();
}

//!
//! \brief Compare l'amélioration actuelle avec son niveau suivant.
//! \param Amelioration
//! \return Pourcentage par rapport à la prochaine amélioration
//!
float Player::compareWithNextAmelioration(Amelioration_e Amelioration){
    float CurrentValue = m_Ameliorations[Amelioration].getCurrentStageValue();
    float NextValue = m_Ameliorations[Amelioration].getNextStageValue();
    return (NextValue > CurrentValue ? CurrentValue/NextValue : NextValue/CurrentValue) * 100;
}
