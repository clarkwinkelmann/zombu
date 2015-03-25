/**
  \file
  \brief    Déclaration de la classe MysteryWord.
  \author   Clark Winkelmann
  \date     mars 2014
*/
#ifndef MYSTERYWORD_H
#define MYSTERYWORD_H

#include <QString>
#include <QStringList>

//!
//! \brief Classe qui définit un mot à deviner.
//!
class MysteryWord
{
public:
    enum TryStatus_e{
        NoTryLeft,
        AlreadyTried,
        NotFound,
        Found
    };
    static const QString LetterNotFound;

    MysteryWord(QString Word);

    void setWord(QString Word);
    void setRemaingTries(int Tries);

    int  getRemainingTries();
    int  getRemainingLetters();
    QStringList getTestedLetters();

    QStringList getMysteryWord();
    TryStatus_e tryLetter(QString Letter);

private:
    QStringList m_Word;
    QStringList m_TestedLetters;

    int m_RemainingTries;
};

#endif // MYSTERYWORD_H
