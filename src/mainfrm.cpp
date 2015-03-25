/**
  \file
  \brief    Définition de la classe MainFrm.
  \author   Clark Winkelmann
  \date     février 2014
  \version  1.0
*/
#include "mainfrm.h"
#include "ui_mainfrm.h"

#include "gamecore.h"

#include <QSignalMapper>
#include <QToolButton>
#include <QAction>

#include "libzombu.h"
#include "resources.h"

MainFrm::MainFrm(QWidget *parent) : QWidget(parent), ui(new Ui::MainFrm) {
    /*// Vérifie que les images requises sont présentes
    if(     !QFile::exists(GameFramework::imagesPath()+"hero1.png") ||
            !QFile::exists(GameFramework::imagesPath()+"hero2.png") ||
            !QFile::exists(GameFramework::imagesPath()+"hero3.png") ||
            !QFile::exists(GameFramework::imagesPath()+"hero4.png") ||
            !QFile::exists(GameFramework::imagesPath()+"hero5.png") ||
            !QFile::exists(GameFramework::imagesPath()+"zombie1.png") ||
            !QFile::exists(GameFramework::imagesPath()+"zombie2.png") ||
            !QFile::exists(GameFramework::imagesPath()+"zombie3.png") ||
            !QFile::exists(GameFramework::imagesPath()+"zombie4.png") ||
            !QFile::exists(GameFramework::imagesPath()+"zombie5.png") ||
            !QFile::exists(GameFramework::imagesPath()+"missile.png") ||
            !QFile::exists(GameFramework::imagesPath()+"box.png") ||
            !QFile::exists(GameFramework::imagesPath()+"wall.png")
            ){
        QMessageBox msgBox;
        msgBox.setText("Des ressources indispensables n'ont pas pu être chargées. Le jeu va se fermer.");
        msgBox.exec();
        exit(0);
        return;
    }*/

    ui->setupUi(this);

    m_pGameScene = new GameScene;

    ui->grvGame->setScene(m_pGameScene);

    // On écoute les changements d'états de la zone de jeu
    connect(m_pGameScene->getCore(),SIGNAL(updateStatus(GameStatus*)),this,SLOT(on_updateStatus(GameStatus*)));
    connect(m_pGameScene->getCore(),SIGNAL(roundEnded(GameStatus*,Player*)),this,SLOT(on_roundEnded(GameStatus*,Player*)));

    connect(m_pGameScene->getCore(),SIGNAL(gameOver()),this,SLOT(on_gameOver()));
    connect(m_pGameScene->getCore(),SIGNAL(roundStarted()),this,SLOT(on_roundStart()));

    displayMainMenu();
}

MainFrm::~MainFrm() {
    delete ui;
}

//!
//! \brief Met à jour les champs relatifs au shop.
//! \param pGameStatus Pointeur vers le GameStatus de la partie
//! \param pPlayer Pointeur vers l'objet Player
//!
void MainFrm::updateShop(GameStatus *pGameStatus, Player *pPlayer){
    QStringList FoundLetters = pGameStatus->getFoundLetters();

    // Liste des lettres trouvées
    QString LettersText = "";
    foreach(QString Letter, FoundLetters){
        if(LettersText != ""){
            LettersText += ", ";
        }
        LettersText += Letter;
    }

    ui->labLetters->setText(LettersText);

    updateShopAmelioration(pGameStatus,pPlayer,Player::Amelioration_Life,"Life");
    updateShopAmelioration(pGameStatus,pPlayer,Player::Amelioration_Munitions,"Munitions");
    updateShopAmelioration(pGameStatus,pPlayer,Player::Amelioration_Speed,"Speed");
    updateShopAmelioration(pGameStatus,pPlayer,Player::Amelioration_ShootInterval,"Shoot");
}

//!
//! \brief Met à jour les champs du shop relatifs à une amélioration.
//! \param pGameStatus
//! \param pPlayer
//! \param Amelioration
//! \param AmeliorationSuffix Suffixe des noms de champs
//!
//! Aucune vérification n'est faite sur l'existence des champs, car s'ils ne sont pas présents c'est qu'il y a un bug tellement gros que le jeu ne peut plus tourner.
//!
void MainFrm::updateShopAmelioration(GameStatus *pGameStatus, Player *pPlayer, Player::Amelioration_e Amelioration, QString AmeliorationSuffix){

    QHBoxLayout* LettersLayout = findChild<QHBoxLayout*>("layLetters"+AmeliorationSuffix);
    // Supprime tous les enfants du layout qui contiendra les boutons
    while(LettersLayout->count() > 0){
        delete LettersLayout->itemAt(0)->widget();
    }

    if(pPlayer->canUpdate(Amelioration)){
        QStringList FoundLetters = pGameStatus->getFoundLetters();
        QStringList TestedLetters = pPlayer->getTestedLetters(Amelioration);

        bool FoundWord = pPlayer->foundWord(Amelioration);
        int RemainingTries = pPlayer->getRemainingTries(Amelioration);

        // Pour chaque lettre de l'alphabet
        foreach(QString Letter,LibZombu::ALPHABET){
            // Est-ce que la lettre a déjà été jouée
            bool Tried = TestedLetters.contains(Letter);
            // Si la lettre est proposée et que la partie n'est pas finie ou a déjà été jouée
            if((!FoundWord && RemainingTries > 0 && FoundLetters.contains(Letter)) || Tried){
                QToolButton* pButton = new QToolButton();
                LettersLayout->addWidget(pButton);

                // Éléments à envoyer lors du clic sur une lettre
                LetterTry_s LetterTry;
                LetterTry.pGameStatus = pGameStatus;
                LetterTry.pPlayer = pPlayer;
                LetterTry.Amelioration = Amelioration;
                LetterTry.Letter = Letter;

                // Création d'une action pour le bouton
                QAction * pAction = new QAction(Letter,pButton);
                // Désactiver le bouton si déjà essayé
                pAction->setDisabled(Tried);

                // La structure est placée dans un variant afin de l'utiliser comme Donnée de l'action
                QVariant ActionData;
                ActionData.setValue(LetterTry);
                pAction->setData(ActionData);

                // On lie le bouton à l'action
                pButton->setDefaultAction(pAction);

                // On connecte l'action au slot
                connect(pAction,SIGNAL(triggered()),this,SLOT(on_tryLetter()));
            }
        }

        // On affiche le mot tel qu'il est découvert
        QStringList DisplayWord;
        foreach(QString Letter, pPlayer->getMysteryWord(Amelioration)){
            if(Letter == MysteryWord::LetterNotFound){
                DisplayWord.append("_");
            }else{
                DisplayWord.append(Letter);
            }
        }
        findChild<QLabel*>("labWord"+AmeliorationSuffix)->setText(DisplayWord.join(" "));
        findChild<QLabel*>("labErrors"+AmeliorationSuffix)->setText(FoundWord ? "Vous avez trouvé le mot mystère !" :
                            (RemainingTries > 0 ? QString("Encore %1 erreurs possibles").arg(RemainingTries-1) : "Perdu !"));

        findChild<QProgressBar*>("pgbAmelioration"+AmeliorationSuffix)->setValue(pPlayer->compareWithNextAmelioration(Amelioration));
    }else{
        findChild<QLabel*>("labWord"+AmeliorationSuffix)->setText("B R A V O");
        findChild<QLabel*>("labErrors"+AmeliorationSuffix)->setText("Vous avez atteint l'amélioration maximum ");
        findChild<QProgressBar*>("pgbAmelioration"+AmeliorationSuffix)->setValue(100);
    }
}

//!
//! \brief Affiche le menu principal et masque le reste.
//!
void MainFrm::displayMainMenu(){
    ui->widMainMenu->show();
    ui->widStatus->hide();
    ui->widShop->hide();
    ui->widRoundResult->hide();
    ui->widGameMenu->hide();
}

//!
//! \brief Gère le clic sur le bouton nouvelle partie Hardcore.
//!
void MainFrm::on_btnDifficultyHardcore_clicked(){
    m_pGameScene->getCore()->newGame(Player::Hardcore);
}

//!
//! \brief Gère le clic sur le bouton nouvelle partie Insane.
//!
void MainFrm::on_btnDifficultyInsane_clicked(){
    m_pGameScene->getCore()->newGame(Player::Insane);
}

//!
//! \brief Gère le clic sur le bouton nouvelle partie Impossible.
//!
void MainFrm::on_btnDifficultyImpossible_clicked(){
    m_pGameScene->getCore()->newGame(Player::Impossible);
}

//!
//! \brief Gère le clic sur le bouton pause.
//!
void MainFrm::on_btnPause_clicked(){
    m_pGameScene->getCore()->togglePause();
}

//!
//! \brief Gère le clic sur le bouton vague suivante.
//!
void MainFrm::on_btnNextRound_clicked(){
    m_pGameScene->getCore()->beginRound();
}

//!
//! \brief Gère le clic sur le bouton niveau supérieur.
//!
void MainFrm::on_btnNextLevel_clicked(){
    m_pGameScene->getCore()->beginNextLevel();
}

//!
//! \brief Gère le clic sur le bouton recommencer.
//!
void MainFrm::on_btnRestart_clicked(){
    m_pGameScene->getCore()->beginRound();
}

//!
//! \brief Gère le clic sur le bouton Menu principal dans le menu de jeu.
//!
void MainFrm::on_btnGameMenuToMainMenu_clicked(){
    displayMainMenu();
}

//!
//! \brief Met à jour la zone de status du jeu.
//! \param pGameStatus Un object GameStatus représentant l'état de la partie
//!
void MainFrm::on_updateStatus(GameStatus* pGameStatus){
    ui->labScore->setText(QString("Score: %1").arg(pGameStatus->getScore()));
    ui->labLevel->setText(QString("Niveau: %1").arg(pGameStatus->getLevel()));
    ui->pgbLife->setValue(pGameStatus->getLife());
    ui->pgbMunitions->setValue(pGameStatus->getMunitions());
    ui->pgbRound->setFormat(QString("Vague %1").arg(pGameStatus->getRound()));
    ui->pgbRound->setValue(pGameStatus->getRoundProgress());
}

//!
//! \brief Gère la fin du round.
//! \param pGameStatus Pointeur vers le GameStatus de la partie
//! \param pPlayer Pointeur vers l'objet Player
//!
void MainFrm::on_roundEnded(GameStatus *pGameStatus, Player* pPlayer){
    ui->labRoundStatDamages->setText(QString("Coups reçus: %1").arg(pGameStatus->getRoundDamages()));
    ui->labRoundStatShoots->setText(QString("Coups tirés: %1 (%2% ont atteint leur cible)").arg(pGameStatus->getRoundShoots()).
                                    arg(qRound(static_cast<float>(pGameStatus->getRoundHits())/static_cast<float>(pGameStatus->getRoundShoots())*100)));
    ui->labRoundStatKills->setText(QString("Zombies tués: %1").arg(pGameStatus->getRoundKills()));

    bool CanLevelUp = m_pGameScene->getCore()->canLevelUp();
    ui->btnNextLevel->setEnabled(CanLevelUp);
    ui->btnNextLevel->setText(QString("Passer au niveau supérieur")+
                              (CanLevelUp ? QString(" (L)") : QString(" - Score de %1 requis").arg(m_pGameScene->getCore()->getRequiredScore())));

    updateShop(pGameStatus, pPlayer);

    ui->widRoundResult->show();
    ui->widShop->show();
}

//!
//! \brief Gère le gameover.
//!
void MainFrm::on_gameOver(){
    ui->widGameMenu->show();
}

//!
//! \brief Gère le début d'un nouveau round.
//!
void MainFrm::on_roundStart(){
    ui->widMainMenu->hide();
    ui->widStatus->show();
    ui->widRoundResult->hide();
    ui->widShop->hide();
    ui->widGameMenu->hide();
}

//!
//! \brief Essaie une lettre.
//!
//! La fonction récupère les paramètres placés dans l'action qui a déclanché le slot.
//!
void MainFrm::on_tryLetter(){
    // Récupération des données de l'action et conversion de Variant à LetterTry_s
    QAction* action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);
    LetterTry_s LetterTry = action->data().value<LetterTry_s>();

    // Test de la lettre et mise à jour du shop
    LetterTry.pPlayer->tryLetter(LetterTry.Amelioration,LetterTry.Letter);
    LetterTry.pGameStatus->popLetter(LetterTry.Letter);
    updateShop(LetterTry.pGameStatus,LetterTry.pPlayer);
}
