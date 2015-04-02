/**
  \file
  \brief    Fichier qui contient la logique du jeu.
  \author   Clark Winkelmann
  \date     février 2014
*/
#include "gamecore.h"

#include <QDebug>
#include <QGraphicsScale>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QSettings>

#include "resources.h"
#include "missile.h"
#include "zombie.h"
#include "obstacle.h"

const int SCENE_MAX_WALLS = 6;
const int SCENE_MAX_BOXES = 10;

/* =========================
           PUBLIC
 ========================= */

//!
//! \brief Initialise le contrôleur de jeu.
//! \param pScene   Pointeur sur la scène représentant la surface de jeu.
//! \param pParent  Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
//!
GameCore::GameCore(GameScene* pScene, QObject* pParent) : QObject(pParent) {
    m_pScene = pScene;

    // Démarrage du timer
    m_pScene->startTick(20);

    m_pSuperHero = 0;
    m_pGameStatus = 0;
    m_pPauseText = 0;

    m_GamePaused    = true;
    m_RoundFinished = false;

    m_pPlayer = 0;

    m_pScene->setWidth(1000);
    m_pScene->setHeight(322);

    Sprite* pLogo = new Sprite(QString(GameFramework::imagesPath()+"logo.png"));

    m_pScene->addItem(pLogo);
}

//!
//! \brief Traite la pression d'une touche.
//! \param Key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int Key) {
    if(!inGame()){
        return;
    }
    if(!m_GamePaused && !m_RoundFinished){
        switch (Key){
        case Qt::Key_Left:  case Qt::Key_A: m_pSuperHero->turnLeft(true);  break;
        case Qt::Key_Right: case Qt::Key_D: m_pSuperHero->turnRight(true); break;
        case Qt::Key_Up:    case Qt::Key_W: m_pSuperHero->goAhead(true);   break;
        case Qt::Key_Down:  case Qt::Key_S: m_pSuperHero->goBack(true);    break;
        case Qt::Key_Space:
            Missile* pMissile = m_pSuperHero->shoot();
            // Si un missile a bien été créé
            if(pMissile != 0){
                m_pScene->addItem(pMissile);
                connect(this,SIGNAL(gameTick(int)),pMissile,SLOT(update(int)));
            }
            break;
        }
    }
    if(Key == Qt::Key_R || (Key == Qt::Key_V && m_RoundFinished)){
        beginRound();
    }
    if(Key == Qt::Key_L && m_RoundFinished){
        beginNextLevel();
    }
    if(Key == Qt::Key_Escape){
        togglePause();
    }
}

//!
//! \brief Traite le relâchement d'une touche.
//! \param Key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyReleased(int Key) {
    if(!inGame()){
        return;
    }
    switch (Key){
    case Qt::Key_Left:  case Qt::Key_A: m_pSuperHero->turnLeft(false);  break;
    case Qt::Key_Right: case Qt::Key_D: m_pSuperHero->turnRight(false); break;
    case Qt::Key_Up:    case Qt::Key_W: m_pSuperHero->goAhead(false);   break;
    case Qt::Key_Down:  case Qt::Key_S: m_pSuperHero->goBack(false);    break;
    }
}

//!
//! \brief Permet l'animation des éléments du jeu.
//! \param ElapsedTimeInMilliSeconds Temps depuis le dernier tick
//!
//! Met à jour l'état du jeu et émet le signal gameTick qui est utilisé par les sous-classes.
//!
//! \see gameTick()
//!
void GameCore::tick(int ElapsedTimeInMilliSeconds) {
    // Si l'interval entre les spawn de zombies est écoulé
    if(!m_GamePaused && !m_RoundFinished && inGame()){
        if(m_LastZombieSpawnTime.elapsed() > m_ZombieSpawnInterval*1000 && m_RoundZombiesLeft > 0){
            m_LastZombieSpawnTime.start();
            m_RoundZombiesLeft--;
            m_RoundZombiesAlive++;

            // Mettre à jour l'avancement du round
            onStatusRoundChange();

            // Création d'un zombie et connexion des signaux
            Zombie* pZombie = m_pPlayer->createZombie(m_pSuperHero->pos());
            m_pScene->addItem(pZombie);
            m_pScene->addItem(pZombie->getLifeMeter());
            m_pScene->addItem(pZombie->getLifeMeterTotal());
            pZombie->setObstacleZone(m_ObstacleZone);
            connect(this,SIGNAL(gameTick(int)),pZombie,SLOT(update(int)));
            connect(m_pSuperHero,SIGNAL(hasMoved(QPointF)),pZombie,SLOT(setTarget(QPointF)));
            connect(pZombie,SIGNAL(beenShot(QPointF)),this,SLOT(spillBlood(QPointF)));
            connect(pZombie,SIGNAL(beenShot(QPointF)),this,SLOT(onZombieHurt()));
            connect(pZombie,SIGNAL(beenDead(QPointF)),this,SLOT(onZombieDeath(QPointF)));

            // On le place quelque part aléatoirement
            // TODO: Empêcher le spawn au milieu de la scène
            pZombie->setPos(qrand() % m_pPlayer->getSceneSize().width(),qrand() % m_pPlayer->getSceneSize().height());

            // On diminue l'interval :Rire machiavélique:
            m_ZombieSpawnInterval *= 0.99;
        }
        emit gameTick(ElapsedTimeInMilliSeconds);

        // Exécuter après gameTick pour être sûr que tous les objets ont finit de se refraîchir
        // Réagir sur un signal se produirait pendant le refraîchissement
        if(m_RoundZombiesLeft <= 0 && m_RoundZombiesAlive <= 0){
            m_RoundFinished = true;
            endRound();
        }
    }
}

//!
//! \brief Retourne si le joueur est en mesure de passer au niveau supérieur.
//! \return Vrai ou faux
//! \see getRequiredScore()
//!
bool GameCore::canLevelUp(){
    if(!inGame()){
        return false;
    }
    return m_pPlayer->canLevelUp();
}

//!
//! \brief Récupère le score requis pour passer au niveau suivant.
//! \return Score en points
//! \see canLevelUp()
//!
int GameCore::getRequiredScore(){
    if(!inGame()){
        return 0;
    }
    return m_pPlayer->getRequiredScore(m_pPlayer->getLevel());
}

//!
//! \brief Interrupteur de la pause.
//!
void GameCore::togglePause(){
    // On ne peut activer/désactiver la pause que pendant la partie
    if(!m_RoundFinished && inGame()){
        m_GamePaused = !m_GamePaused;
        if(m_GamePaused){
            m_pPauseText->show();
        }else{
            m_pPauseText->hide();
            // On redémarre le timer zombie pour qu'ils ne spawn pas dès le clic
            m_LastZombieSpawnTime.start();
        }
    }
}

//!
//! \brief Commence le niveau suivant si les conditions sont remplies.
//! \see beginNextRound()
//!
void GameCore::beginNextLevel(){
    if(m_RoundFinished && inGame() && m_pPlayer->canLevelUp()){
        m_pPlayer->nextLevel();
        beginRound();
    }
}

//!
//! \brief Démarre le round selon le niveau/round actuel.
//!
//! Créé les objets de la scène et lie leurs signaux puis lance le jeu.
//!
//! \see createScene()
//!
void GameCore::beginRound(){
    if(!inGame()){
        return;
    }

    // Nettoyage de la zone de jeu
    m_pScene->clear();

    // Trace un rectangle blanc tout autour des limites de la scène.
    m_pScene->addRect(0,0, m_pScene->width(), m_pScene->height(), QPen(Qt::white));

    // Création du héros et connection des signaux
    m_pSuperHero = m_pPlayer->createHero();
    m_pScene->addItem(m_pSuperHero);
    m_pSuperHero->setPos((m_pScene->width()-m_pSuperHero->width())/2,(m_pScene->height()-m_pSuperHero->height())/2);
    m_pSuperHero->setGameZone(QRectF(0,0,m_pPlayer->getSceneSize().width(),m_pPlayer->getSceneSize().height()));

    connect(this,SIGNAL(gameTick(int)),m_pSuperHero,SLOT(update(int)));

    connect(m_pSuperHero,SIGNAL(hasShooted(float)),this,SLOT(onHeroShoot()));
    connect(m_pSuperHero,SIGNAL(hasShooted(float)),this,SLOT(onStatusMunitionsChange(float)));
    connect(m_pSuperHero,SIGNAL(hasReloaded(float)),this,SLOT(onStatusMunitionsChange(float)));

    connect(m_pSuperHero,SIGNAL(beenHurt(float)),this,SLOT(onStatusLifeChange(float)));
    connect(m_pSuperHero,SIGNAL(hasHealed(float)),this,SLOT(onStatusLifeChange(float)));

    connect(m_pSuperHero,SIGNAL(beenShot(QPointF)),this,SLOT(spillBlood(QPointF)));
    connect(m_pSuperHero,SIGNAL(beenShot(QPointF)),this,SLOT(onHeroHurt()));

    connect(m_pSuperHero,SIGNAL(beenDead(QPointF)),this,SLOT(onHeroDeath()));

    // Création du round
    m_ZombieSpawnInterval = m_pPlayer->getZombieSpawnInterval();
    m_LastZombieSpawnTime.start();
    m_RoundTotalZombies = m_pPlayer->getRoundTotalZombies();
    m_RoundZombiesLeft = m_RoundTotalZombies;
    m_RoundZombiesAlive = 0;

    // Création du GameStatus avec les valeurs de début de jeu
    m_pGameStatus = new GameStatus;
    m_pGameStatus->setScore(m_pPlayer->getScore());
    m_pGameStatus->setLevel(m_pPlayer->getLevel(),m_pPlayer->getRound());
    m_pGameStatus->setLife(m_pSuperHero->getLife());
    m_pGameStatus->setMunitions(m_pSuperHero->getMunitions());

    emitUpdateStatus();

    createScene(SCENE_MAX_WALLS,SCENE_MAX_BOXES);

    m_pPauseText = new QGraphicsTextItem;
    m_pScene->addItem(m_pPauseText);
    m_pPauseText->setHtml(QString("<div style=\"color: red; font-size: %1px; font-weight: bold;\"><center>" + QObject::tr("PAUSE") +  "</center></div>")
                          .arg(m_pScene->height()/2.5));
    m_pPauseText->setTextWidth(m_pScene->width());
    m_pPauseText->setPos(QPointF(0,m_pScene->height()/10));
    m_pPauseText->setOpacity(0.8);
    m_pPauseText->setZValue(400);
    m_pPauseText->hide();

    m_pSuperHero->setObstacleZone(m_ObstacleZone);

    m_GamePaused = false;
    m_RoundFinished = false;

    emit roundStarted();
}

//!
//! \brief Démarrage d'une nouvelle partie.
//!
void GameCore::newGame(Player::GameDifficulty_e Difficulty){
    if(m_pPlayer != 0){
        delete m_pPlayer;
    }
    m_pPlayer = new Player(Difficulty);

    // Spécifie les dimensions de la scène
    m_pScene->setWidth(m_pPlayer->getSceneSize().width());
    m_pScene->setHeight(m_pPlayer->getSceneSize().height());

    beginRound();
}

/* =========================
          PRIVATE
 ========================= */

//!
//! \brief Création de la scène de combat.
//!
//! Les obstacles sont placés aléatoirement sur la scène et ajoutés à la liste des obstacles.
//!
//! \see beginRound()
//!
void GameCore::createScene(int MaxWall, int MaxBox){
    m_ObstacleZone.clear();

    // Wall
    for(int i=0;i<MaxWall;i++){
        bool OK = true;
        Obstacle* pWall = new Obstacle(Obstacle::Wall);
        m_pScene->addItem(pWall);
        pWall->setPos(qrand() % (m_pPlayer->getSceneSize().width()-pWall->width()),qrand() % (m_pPlayer->getSceneSize().height()-pWall->height()));

        foreach(QGraphicsItem* pItem, pWall->collidingItems()){
            // S'il touche un autre obstacle on l'annule
            if(pItem->type() == Obstacle::ObstacleItemType){
                OK = false;
            }
        }

        if(OK){
            m_ObstacleZone.append(QRectF(pWall->posX(),pWall->posY(),pWall->width(),pWall->height()));
        }else{
            delete pWall;
        }
    }

    // Box
    for(int i=0;i<MaxBox;i++){
        bool OK = true;
        Obstacle* pBox = new Obstacle(Obstacle::Box);
        m_pScene->addItem(pBox);
        pBox->setPos(qrand() % (m_pPlayer->getSceneSize().width()-pBox->width()),qrand() % (m_pPlayer->getSceneSize().height()-pBox->height()));

        foreach(QGraphicsItem* pItem, pBox->collidingItems()){
            // S'il touche un autre obstacle on l'annule
            if(pItem->type() == Obstacle::ObstacleItemType){
                OK = false;
            }
        }

        if(!OK){
            delete pBox;
        }
    }
}

//!
//! \brief Gère la fin de la vague.
//! \see beginRound()
//!
void GameCore::endRound(){
    // Afficher les résultats
    m_GamePaused = true;

    m_pPlayer->setScore(m_pGameStatus->getScore());
    m_pPlayer->nextRound();

    emit roundEnded(m_pGameStatus,m_pPlayer);
}

//!
//! \brief Envoie le signal updateStatus.
//!
//! Émet un signal indiquant que l'état du jeu a changé et y joint un pointeur vers GameStatus.
//!
//! \see updateStatus()
//!
void GameCore::emitUpdateStatus(){
    emit updateStatus(m_pGameStatus);
}

//!
//! \brief Détermine si une partie est en cours.
//! \return Vrai si en cours
//!
bool GameCore::inGame(){
    return m_pPlayer != 0;
}

//!
//! \brief Affiche un message qui monte lentement puis disparaît à une certaine position.
//! \param Position Position à laquelle afficher le message
//! \param Message Message à afficher
//!
void GameCore::displayMessage(QPointF Position, QString Message){
    QGraphicsTextItem* pText = m_pScene->addText(Message);
    pText->setHtml(QString("<div style=\"color: red; font-size: 20px; font-weight: bold;\">%1</div>")
                          .arg(Message));

    pText->setPos(Position);
    pText->setOpacity(0.8);
    pText->setZValue(2000);

    QSequentialAnimationGroup* pAnimationGroup = new QSequentialAnimationGroup;

    QPropertyAnimation* pAnimationTop = new QPropertyAnimation(pText, "y");
    pAnimationTop->setDuration(500);
    pAnimationTop->setStartValue(Position.y());
    pAnimationTop->setEndValue(Position.y()-30);

    pAnimationGroup->addAnimation(pAnimationTop);

    QPropertyAnimation* pAnimationOpacity = new QPropertyAnimation(pText, "opacity");
    pAnimationOpacity->setDuration(600);
    pAnimationOpacity->setStartValue(0.8);
    pAnimationOpacity->setEndValue(0);

    pAnimationGroup->addAnimation(pAnimationOpacity);

    pAnimationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

/* =========================
       PRIVATE SLOTS
 ========================= */

//!
//! \brief Répend sur sang à la position indiquée.
//! \param Position Position de la scène d'horreur
//!
//! La taille de la tache de sang est aléatoire.
//!
void GameCore::spillBlood(QPointF Position){
    Sprite* pSprite = m_pScene->createSprite();
    pSprite->addAnimationFrame(QString(GameFramework::imagesPath()+"blood.png"));
    pSprite->setScale(static_cast<float>(qrand()%10+1)/10);
    pSprite->setPos(Position+QPointF(pSprite->width()/2,pSprite->height()/2));
}

//!
//! \brief Modifie la valeur de la vie dans GameStatus et émet le signal de rafraîchissement.
//! \param NewLife Nouvelle vie en %
//!
void GameCore::onStatusLifeChange(float NewLife){
    m_pGameStatus->setLife(NewLife);
    emitUpdateStatus();
}

//!
//! \brief Modifie la valeur des munitions dans GameStatus et émet le signal de rafraîchissement.
//! \param NewMunitions Nouveau stock de munitions en %
//!
void GameCore::onStatusMunitionsChange(float NewMunitions){
    m_pGameStatus->setMunitions(NewMunitions);
    emitUpdateStatus();
}

//!
//! \brief Met à jour l'avancement du round dans l'objet GameStatus.
//!
void GameCore::onStatusRoundChange(){
    m_pGameStatus->setRoundProgress(100-static_cast<float>(m_RoundZombiesLeft+m_RoundZombiesAlive)/static_cast<float>(m_RoundTotalZombies)*100);
    emitUpdateStatus();
}

//!
//! \brief Ajoute un tir aux statistiques
//!
void GameCore::onHeroShoot(){
    m_pGameStatus->modRoundShoots(1);
}

//!
//! \brief Met à jour le GameStatus lorsque le Hero est touché.
//!
void GameCore::onHeroHurt(){
    m_pGameStatus->modScore(-50);
    m_pGameStatus->modRoundDamages(1);
    emitUpdateStatus();
}

//!
//! \brief Gère la mort du héros.
//!
//! Met le jeu en pause et affiche Game Over.
//!
void GameCore::onHeroDeath(){
    m_GamePaused = true;
    m_RoundFinished = true;

    QGraphicsTextItem* pText = new QGraphicsTextItem;
    m_pScene->addItem(pText);
    pText->setHtml(QString("<div style=\"color: red; font-size: %1px; font-weight: bold;\"><center><div style=\"font-size: %2px;\">%3</div><div>%4</div></center></div>")
                   .arg(m_pScene->height()/2.5).arg(m_pScene->height()/3.5).arg(QObject::tr("GAME")).arg(QObject::tr("OVER")));
    pText->setTextWidth(m_pScene->width());
    pText->setPos(QPointF(0,m_pScene->height()/20));
    pText->setOpacity(0.8);
    pText->setZValue(500);

    emit gameOver();
}

//!
//! \brief Ajoute un coup qui a atteint la cible.
//!
void GameCore::onZombieHurt(){
    m_pGameStatus->modRoundHits(1);
}

//!
//! \brief Met à jour le GameStatus lorsqu'un Zombie est tué.
//! \param Position Position de la mort
//!
void GameCore::onZombieDeath(QPointF Position){
    QString Letter = m_pPlayer->pickLetter();
    if(Letter != Player::NoLetter){
        displayMessage(Position,QString(QObject::tr("Trouvé %1 !")).arg(Letter));
        m_pGameStatus->addFoundLetter(Letter);
    }

    m_RoundZombiesAlive--;
    m_pGameStatus->modScore(100);
    m_pGameStatus->modRoundKills(1);
    emitUpdateStatus();
    onStatusRoundChange();
}
