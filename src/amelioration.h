/**
  \file
  \brief    Déclaration de la classe Amelioration.
  \author   Clark Winkelmann
  \date     mars 2014
*/
#ifndef AMELIORATION_H
#define AMELIORATION_H

#include "mysteryword.h"
#include "wordlibrary.h"

//!
//! \brief Classe qui définit une amélioration pour le joueur.
//!
class Amelioration
{
public:
    Amelioration();
    ~Amelioration();

    void levelUp();
    MysteryWord::TryStatus_e tryLetter(QString Letter);
    void setStages(QList<float> StagesList);
    void setWordLibrary(WordLibrary* pWordLibrary);
    float getCurrentStageValue();
    float getNextStageValue();
    bool canUpdate();

    int getRemainingTries();
    QStringList getTestedLetters();
    QStringList getMysteryWord();
    bool foundWord();

private:
    void newWord();
    float getStageValue(int Stage);

    QList<float> m_StagesList;
    int m_Stage;
    MysteryWord* m_pMysteryWord;
    WordLibrary* m_pWordLibrary;
};

#endif // AMELIORATION_H
