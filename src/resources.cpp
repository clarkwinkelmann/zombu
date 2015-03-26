/**
  \file
  \brief    Méthodes utilitaires permettant d'accéder aux ressources.
  \author   JCO
  \date     février 2014
  \version  1.0
*/
#include "resources.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

namespace GameFramework {
/**
Cette fonction retourne le chemin absolu du répertoire res.
Si la pseudo-constante DEPLOY est définie, elle se base sur la structure
de répertoires suivante :
\verbatim
+--ce_projet/
   |
   +--res/
   +ce_projet.exe
\endverbatim

Si la pseudo-constante DEPLOY n'est pas définie, elle se base sur la structure
de répertoires suivante :
\verbatim
+--pendu/
   |
   +--build-ce_projet.../
   |  |
   |  +--debug/
   |  |  |
   |  |  +ce_projet.exe
   |  |
   |  +--release/
   |     |
   |     +ce_projet.exe
   |
   +--doc/
   +--res/
   +--src/
\endverbatim

La pseudo-constante DEPLOY ne devrait être définie que lors de la génération de l'executable
en vue du déploiement.

\return une chaîne de caractères contenant le chemin absolu du répertoire res.
*/
    QString resourcesPath() {
        QDir ResourceDir = QDir(qApp->applicationDirPath());

    #ifndef DEPLOY
        #ifndef Q_OS_LINUX
            ResourceDir.cdUp(); // Quitte 'debug' ou 'release'
        #endif
            ResourceDir.cdUp(); // Quitte 'build...'
    #endif
        ResourceDir.cd("res");
        //qDebug() << ResourceDir.absolutePath();
        return ResourceDir.absolutePath() + QDir::separator();
    }

    /**
     * @return Emplacement des images
     */
    QString imagesPath() {
        return resourcesPath() + QDir::separator() + QString("images") + QDir::separator();
    }

    /**
     * @return Emplacement des locales
     */
    QString localesPath() {
        return resourcesPath() + QDir::separator() + QString("locales") + QDir::separator();
    }
}
