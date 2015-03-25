/**
  \file
  \brief    Déclaration de la classe MainFrm.
  \author   Clark Winkelmann
  \date     février 2014
  \version  1.0
*/
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "gamescene.h"
#include "gamestatus.h"
#include "player.h"

namespace Ui {
class MainFrm;
}

class MainFrm : public QWidget
{
    Q_OBJECT

public:
    explicit MainFrm(QWidget *parent = 0);
    ~MainFrm();

    //! \brief Structure à passer lors du clic sur un bouton de lettre.
    struct LetterTry_s{
        GameStatus* pGameStatus;
        Player* pPlayer;
        Player::Amelioration_e Amelioration;
        QString Letter;
    };

private:
    Ui::MainFrm *ui;
    GameScene* m_pGameScene;

    void updateShop(GameStatus* pGameStatus, Player* pPlayer);
    void updateShopAmelioration(GameStatus* pGameStatus, Player* pPlayer,Player::Amelioration_e Amelioration, QString AmeliorationSuffix);
    void displayMainMenu();

private slots:
    void on_btnDifficultyHardcore_clicked();
    void on_btnDifficultyInsane_clicked();
    void on_btnDifficultyImpossible_clicked();
    void on_btnPause_clicked();
    void on_btnNextRound_clicked();
    void on_btnNextLevel_clicked();
    void on_btnRestart_clicked();
    void on_btnGameMenuToMainMenu_clicked();
    void on_updateStatus(GameStatus* pGameStatus);
    void on_roundEnded(GameStatus* pGameStatus, Player* pPlayer);
    void on_gameOver();
    void on_roundStart();
    void on_tryLetter();
};

// Enregistrer le type pour pouvoir le passer en variant
Q_DECLARE_METATYPE(MainFrm::LetterTry_s)

#endif // WIDGET_H
