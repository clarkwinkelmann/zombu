/**
  \file
  \brief    Déclaration de la classe MysteryWord.
  \author   Clark Winkelmann
  \date     mars 2014
*/
#include "mysteryword.h"

// Remplace une lettre non devinée
const QString MysteryWord::LetterNotFound("?");

const int DEFAULT_TRIES = 10;

//!
//! \brief Création de l'objet MysteryWord.
//! \param Word Mot à deviner
//!
MysteryWord::MysteryWord(QString Word)
{
    setWord(Word);

    setRemaingTries(DEFAULT_TRIES);
}

//!
//! \brief Définit le mot mystère à chercher.
//! \param Word Mot à deviner
//!
void MysteryWord::setWord(QString Word){
    // Les lettres seront traitées en majuscule
    Word.toUpper();

    m_Word.clear();

    // Avec SkipEmptyParts on évite les espaces à chaque bout.
    // Ça implique de n'utiliser que des mots sans espaces (Ce qui devrait toujours être le cas).
    m_Word = Word.split("",QString::SkipEmptyParts);
}

//!
//! \brief Définit le nombre d'essais restants.
//! \param Tries Nombre d'essais
//!
void MysteryWord::setRemaingTries(int Tries){
    m_RemainingTries = Tries;
}

//!
//! \brief Récupère le nombre d'essais restants.
//! \return Nombre d'essais
//!
int MysteryWord::getRemainingTries(){
    return m_RemainingTries;
}

//!
//! \brief Récupère le nombre de lettres qu'il reste à trouver.
//! \return Nombre de lettres
//!
int MysteryWord::getRemainingLetters(){
    int RemainingLetters = 0;
    QStringList TestedLetters;

    foreach(QString Letter,m_Word){
        if(!TestedLetters.contains(Letter)){
            if(!m_TestedLetters.contains(Letter)){
                RemainingLetters++;
            }
            TestedLetters.append(Letter);
        }
    }

    return RemainingLetters;
}

//!
//! \brief Récupère la liste des lettres déjà essayées.
//! \return Liste de lettres
//!
QStringList MysteryWord::getTestedLetters(){
    return m_TestedLetters;
}

//!
//! \brief Rècupère le mot mystère tel qu'il est deviné
//! \return Tableau des lettres du mot mystère. Les lettres non devinées sont remplacées par le caractère LetterNotFound.
//!
QStringList MysteryWord::getMysteryWord(){
    QStringList Word;

    foreach(QString Letter,m_Word){
        if(m_TestedLetters.contains(Letter)){
            Word.append(Letter);
        }else{
            Word.append(LetterNotFound);
        }
    }

    return Word;
}

//!
//! \brief Essaie de deviner une lettre et retourne le statut.
//! \param Letter Lettre à tester
//! \return Statut du test
//!
MysteryWord::TryStatus_e MysteryWord::tryLetter(QString Letter){
    // On travaille avec des lettres majuscules
    Letter.toUpper();

    if(getRemainingTries() > 0){
        // Si la lettre n'a pas encore été testée
        if(!m_TestedLetters.contains(Letter)){
            m_TestedLetters.append(Letter);
            // Si la lettre est dans le mot
            if(m_Word.contains(Letter)){
                return Found;
            }else{
                m_RemainingTries--;
                return NotFound;
            }
        }
        return AlreadyTried;
    }
    return NoTryLeft;
}
