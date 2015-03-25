/*! \mainpage Zombu
 *
 * \author Clark Winkelmann
 * \section intro_sec Introduction
 *
 * Zombu est un jeu de type zombies en 2D réalisé dans le cadre de l'atelier "Programmer un jeu avec QT".
 *
 * Votre objectif est de venir à bout des vagues de zombies qui vous envahissent de toute part afin de récupérer des lettres qui vous permettront d'améliorer votre personnage.
 *
 * Il est basé sur le GameFramework de Jérôme Conus.
 *
 * \section howto_sec Comment jouer
 *
 * * Les touches fléchées GAUCHE/DROITE font tourner le personnage sur lui-même.
 * * Les touches fléchées HAUT/BAS font avancer ou reculer le personnage.
 * * La touche ESPACE tire un coup de feu.
 * * En jeu, la touche ESC permet d'activer ou désactiver la pause.
 * * Vous pouvez recommencer le round à tout moment avec la touche R.
 * * À la fin du round, vous pouvez passer au round suivant avec le bouton "Vague suivante" ou la touche V.
 *
 *
 */

#include "mainfrm.h"

#include <QApplication>
#include <QTime>
#include <QFile>
#include <QMessageBox>

#include "resources.h"

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Vérifie que les images requises sont présentes
    QString ImagePath = GameFramework::imagesPath();
    if(
        !QFile::exists(ImagePath+"hero1.png")   ||
        !QFile::exists(ImagePath+"hero2.png")   ||
        !QFile::exists(ImagePath+"hero3.png")   ||
        !QFile::exists(ImagePath+"hero4.png")   ||
        !QFile::exists(ImagePath+"hero5.png")   ||
        !QFile::exists(ImagePath+"zombie1.png") ||
        !QFile::exists(ImagePath+"zombie2.png") ||
        !QFile::exists(ImagePath+"zombie3.png") ||
        !QFile::exists(ImagePath+"zombie4.png") ||
        !QFile::exists(ImagePath+"zombie5.png") ||
        !QFile::exists(ImagePath+"missile.png") ||
        !QFile::exists(ImagePath+"box.png")     ||
        !QFile::exists(ImagePath+"wall.png")
    ){
        QMessageBox msgBox;
        msgBox.setText("Des ressources indispensables n'ont pas pu être chargées. Le jeu va se fermer.");
        msgBox.exec();
        return 0;
    }

    qRegisterMetaType<MainFrm::LetterTry_s>("MainFrm::LetterTry_s");

    // Initialisation du générateur pseudo-aléatoire
    qsrand(QTime::currentTime().msec());

    MainFrm w;
    w.showMaximized();

    return a.exec();
}
