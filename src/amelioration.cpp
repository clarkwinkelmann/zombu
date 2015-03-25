/**
  \file
  \brief    Déclaration de la classe Amelioration.
  \author   Clark Winkelmann
  \date     mars 2014
*/
#include "amelioration.h"

//!
//! \brief Création de l'objet Amelioration.
//!
Amelioration::Amelioration(){
    m_Stage = 0;
    m_pMysteryWord = 0;
    m_pWordLibrary = 0;

    newWord();
}

Amelioration::~Amelioration(){
    delete m_pMysteryWord;
}

//!
//! \brief Exécute les actions relatives à un nouveau niveau.
//!
void Amelioration::levelUp(){
    newWord();
}

//!
//! \brief Joue une lettre.
//! \param Letter
//! \return
//!
MysteryWord::TryStatus_e Amelioration::tryLetter(QString Letter){
    MysteryWord::TryStatus_e Status = MysteryWord::NoTryLeft;
    if(canUpdate()){
        Status = m_pMysteryWord->tryLetter(Letter);

        if(Status == MysteryWord::Found && foundWord()){
            m_Stage += 1;
        }
    }

    return Status;
}

//!
//! \brief Définit la liste des améliorations.
//! \param StagesList
//!
void Amelioration::setStages(QList<float> StagesList){
    m_StagesList = StagesList;
}

//!
//! \brief Définit un pointeur vers la WordLibrary à utiliser.
//! \param pWordLibrary Pointeur vers WordLibrary
//!
void Amelioration::setWordLibrary(WordLibrary *pWordLibrary){
    m_pWordLibrary = pWordLibrary;
    // On prend un nouveau mot dans la librairie
    newWord();
}

//!
//! \brief Récupère la valeur de l'amélioration actuelle.
//! \return Valeur de l'amélioration
//!
//! Si le niveau d'amélioration est plus grand que le nombre d'améliorations, c'est la dernière valeur qui est retournée.
//!
float Amelioration::getCurrentStageValue(){
    return getStageValue(m_Stage);
}

//!
//! \brief Récupère la valeur de la prochaine amélioration.
//! \return Valeur de l'amélioration
//!
float Amelioration::getNextStageValue(){
    return getStageValue(m_Stage+1);
}

//!
//! \brief Détermine si l'amélioration peut être améliorée
//! \return
//!
bool Amelioration::canUpdate(){
    return m_Stage < m_StagesList.count()-1;
}

//!
//! \brief Récupère le nombre d'essais restants.
//! \return Nombre d'essais
//!
int Amelioration::getRemainingTries(){
    return m_pMysteryWord->getRemainingTries();
}

//!
//! \brief Récupère la liste des lettres essayées.
//! \return Liste de caractères
//!
QStringList Amelioration::getTestedLetters(){
    return m_pMysteryWord->getTestedLetters();
}

//!
//! \brief Récupère le mot mystérieux tel qu'il est découvert.
//! \return Tableau de caractères
//!
QStringList Amelioration::getMysteryWord(){
    return m_pMysteryWord->getMysteryWord();
}

//!
//! \brief Récupère si la mot a été découvert.
//! \return Vrai si découvert
//!
bool Amelioration::foundWord(){
    return m_pMysteryWord->getRemainingLetters() == 0;
}

//!
//! \brief Définit un nouveau mot à deviner.
//!
void Amelioration::newWord(){
    QString Word = "";
    // Si la librairie n'a pas été définie on laisse un mot vide
    if(m_pWordLibrary != 0){
        Word = m_pWordLibrary->getRandomWord();
    }
    if(m_pMysteryWord == 0){
        delete m_pMysteryWord;
    }
    m_pMysteryWord = new MysteryWord(Word);
    m_pMysteryWord->setRemaingTries(10);
}

//!
//! \brief Récupère la valeur d'un niveau d'amélioration.
//! \return Valeur de l'amélioration
//!
float Amelioration::getStageValue(int Stage){
    if(m_StagesList.size() > Stage){
        // On retourne la valeur actuelle
        return m_StagesList[Stage];
    }else if(!m_StagesList.isEmpty()){
        // On retourne la plus grande valeur
        return m_StagesList.last();
    }
    // Cas peu probable, liste vide
    return 0;
}
