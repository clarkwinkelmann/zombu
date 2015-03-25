/**
  \file
  \brief    Déclaration de la classe WordLibrary.
  \author   Clark Winkelmann
  \date     mars 2014
*/
#include "wordlibrary.h"

#include <QFile>
#include <QTextStream>

//!
//! \brief Création de l'objet WordLibrary.
//!
WordLibrary::WordLibrary(){}

//!
//! \brief Charge un fichier de mots dans la bibliothèque.
//! \param FileName Chemin vers le fichier
//!
void WordLibrary::loadFromFile(QString FileName){
    if(!QFile::exists(FileName)){
        return;
    }

    QFile File(FileName);
    if (!File.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    m_WordList.clear();

    QTextStream TextStream(&File);
    while (true)
    {
        QString Line = TextStream.readLine();
        if (Line.isNull()){
            break;
        }else{
            m_WordList.append(QString(Line).toUpper());
        }
    }
}

//!
//! \brief Retourne un mot aléatoire tiré dans la liste.
//! \return Mot
//!
//! Si la liste ne contient aucun mot, une chaîne vide est retournée.
//!
QString WordLibrary::getRandomWord(){
    if(m_WordList.isEmpty()){
        return "";
    }
    int RandomIndex = qrand() % m_WordList.count();

    return m_WordList[RandomIndex];
}
