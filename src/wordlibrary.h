/**
  \file
  \brief    Déclaration de la classe WordLibrary.
  \author   Clark Winkelmann
  \date     mars 2014
*/
#ifndef WORDLIBRARY_H
#define WORDLIBRARY_H

#include <QStringList>

//!
//! \brief Classe qui gère une liste de mots dans laquelle on peut piocher.
//!
class WordLibrary
{
public:
    WordLibrary();

    void loadFromFile(QString FileName);
    QString getRandomWord();

private:
    QStringList m_WordList;
};

#endif // WORDLIBRARY_H
