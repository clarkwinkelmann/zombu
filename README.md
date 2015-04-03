Jeu du Zombu
============

Bienvenue dans le terrible jeu du Zombu !

Pourquoi ce jeu ?
-----------------

Ce projet a été réalisé par moi-même à l'[École des Métier Techniques de Porrentruy](http://cpp.ch) dans le cadre d'un atelier de développement de jeu vidéo de 2e année.
L'objectif était de réaliser un jeu graphique à l'aide de la bibliothèque de développement Qt.

Je mets ce jeu en Open Source sur GitHub pour éviter de l'abandonner sur mon disque dur où il moisirait probablement pendant longtemps sans être utile à personne.

Le document [Rapport de projet](doc/Rapport de projet.odt) contient davantage d'informations sur le projet et son déroulement.

Règles du jeu
-------------

Vous trouverez le [Petit guide du Zombu](doc/Mode demploi.odt) avec le reste de la documentation. Ce document vous présentera les bases du jeu.

Installer le jeu
----------------

Rendez-vous simplement sur l'onglet releases ci-dessus pour télécharger un installeur. Vous pouvez également vous rendre sur le site <http://zombu.rip/> et cliquer sur le bouton Télécharger.

Modifier le jeu
---------------

Allons bon, pourquoi faire des modifications ? Bref, voici quelques informations pour vous aider à toucher au code:

Les scripts et ressources ont été créés dans un environnement de développement Ubuntu avec Wine installé pour les exportations Windows.

### Modifier les images
Si vous souhaitez remplacer les images du jeu de base par des meilleures, il vous suffit de modifier le contenu du dossier res/images en respectant les noms de fichier.

Si vous souhaitez améliorer les images d'origine (res/sources), vous aurez besoin du Logiciel [Inkscape](https://inkscape.org). Vous pouvez ensuite utiliser le script generate-res.sh pour les exporter automatiquement vers les fichiers bitmap (Script pour Linux/OSX).

### Compiler le programme
Si vous souhaitez générer à nouveau l'installeur Windows, vous aurez besoin de:

  * [Inno Setup](http://www.jrsoftware.org/isinfo.php)
  * Une compilation statique de la [bibliothèque Qt](http://qt-project.org)
  * Un environnement Windows / [Wine](https://www.winehq.org)

La compilation peut ensuite être effectuée via le script [Compile.bat](scripts/Compile.bat) préalablement modifié avec les bons chemins d'exécution. Toutes les informations sur la compilation se trouvent dans la [Documentation technique](doc/Documentation technique.odt).

### Générer la documentation
Vous aurez besoin des programmes suivants:

  * [LibreOffice](https://www.libreoffice.org)
  * [Doxygen](http://doxygen.org/)

Le script generate-doc.sh se charge de compiler les fichiers odt vers pdf et de générer la documentation Doxygen.

État du projet
--------------

Le projet n'est plus sous développement actif, mais si vous avez un problème ou repérez un bug je serai heureux de vous aider.

Si vous avez des idées d'améliorations n'hésitez pas à faire des suggestions ou même déposer une Pull Request, j'y jeterai un coup d'oeil !

Vous avez quelque chose à dire ? Rendez-vous simplement sur la page des Issues GitHub.

Licence
-------

Zombu
Copyright (C) 2014 Clark Winkelmann

Ce programme est un logiciel libre ; vous pouvez le redistribuer ou le modifier suivant les termes de la GNU Lesser General Public License telle que publiée par la Free Software Foundation ; soit la version 3 de la licence, soit (à votre gré) toute version ultérieure.

Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même la garantie tacite de QUALITÉ MARCHANDE ou d'ADÉQUATION à UN BUT PARTICULIER. Consultez la GNU Lesser General Public License pour plus de détails.

Vous devez avoir reçu une copie de la GNU Lesser General Public License en même temps que ce programme ; si ce n'est pas le cas, consultez <http://www.gnu.org/licenses>.

