/**
  \file
  \brief    Définition de la classe GameScene.
  \author   JCO
  \date     janvier 2014
*/
#include "gamescene.h"

#include <cstdlib>
#include <QBrush>
#include <QKeyEvent>
#include <QPen>

#include "gamecore.h"
#include "sprite.h"

const int SCENE_WIDTH = 868;
const int SCENE_HEIGHT = 992;

//! Construit la scène de jeu avec une taille par défaut et un fond noir.
GameScene::GameScene(QObject* pParent) : QGraphicsScene(pParent) {

    m_pGameCore = 0;
    m_LastTickInterval = 20;
    m_KeepTicking = false;

    // Dimensionnement de la scène
    this->setSceneRect(0,0, SCENE_WIDTH, SCENE_HEIGHT);
    this->setBackgroundBrush(QBrush(QColor(Qt::black)));

    // Création de l'objet GameCore
    m_pGameCore = new GameCore(this);
}

//! Destruction de la scène.
GameScene::~GameScene()  {
    delete m_pGameCore;
    m_pGameCore = 0;
}

//! Gère l'appui sur une touche du clavier.
void GameScene::keyPressEvent(QKeyEvent* pKeyEvent) {
    m_pGameCore->keyPressed(pKeyEvent->key());
}

//! Gère le relâchement d'une touche du clavier.
void GameScene::keyReleaseEvent(QKeyEvent* pKeyEvent) {
    m_pGameCore->keyReleased(pKeyEvent->key());
}

//! Créé un nouveau sprite et l'ajoute à la scène.
//! Le sprite, bien que visible, n'affiche rien car aucune image
//! ne lui a encore été attribuée.
//! Si ce sprite n'est pas détruit manuellement, il le sera automatiquement
//! lorsque la scène sera détruite.
//! \see Sprite::addAnimationFrame()
//! \return un pointeur sur le sprite créé.
Sprite* GameScene::createSprite() {
    Sprite* pSprite = new Sprite;
    this->addItem(pSprite);
    pSprite->setVisible(true);
    return pSprite;
}

//! Crée un nouveau sprite, l'ajoute à la scène, lui donne l'apparence de l'image
//! fournie et le place à la position donnée.
//! Si ce sprite n'est pas détruit manuellement, il le sera automatiquement
//! lorsque la scène sera détruite.
//! \param InitialPosition  Position de départ du sprite.
//! \param rImage           Apparence du sprite.
//! \return un pointeur sur le sprite créé.
Sprite* GameScene::createSprite(QPointF InitialPosition, const QPixmap& rImage) {
    Sprite* pSprite = new Sprite(rImage);
    this->addItem(pSprite);
    pSprite->setPos(InitialPosition);
    pSprite->setVisible(true);
    return pSprite;
}

//! Construit la liste de tous les sprites en collision avec le sprite donné en
//! paramètre.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \param pSprite Sprite pour lequel les collisions doivent être vérifiées.
//! \return une liste de sprites en collision. Si aucun autre sprite ne collisionne
//! le sprite donné, la liste retournée est vide.
QList<Sprite*> GameScene::collidingSprites(const Sprite* pSprite) const {
    QList<Sprite*> SpriteList;
    QList<QGraphicsItem*> CollidingItems = pSprite->collidingItems();
    foreach(QGraphicsItem* pItem, CollidingItems) {
        if (pItem->type() == Sprite::SpriteItemType)
            SpriteList << static_cast<Sprite*>(pItem);
    }
    return SpriteList;
}

//! Construit la liste de tous les sprites en collision avec le rectangle donné
//! en paramètre.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \param rRect Rectangle avec lequel il faut tester les collisions.
//! \return une liste de sprites en collision.
QList<Sprite*> GameScene::collidingSprites(const QRectF &rRect) const  {
    QList<Sprite*> CollidingSpriteList;
    QList<Sprite*> SpriteList = sprites();
    foreach(Sprite* pSprite, SpriteList)  {
        QRectF GlobalBBox = pSprite->globalBoundingBox();
        if (GlobalBBox.intersects(rRect)) {
            CollidingSpriteList << pSprite;
        }
    }
    return CollidingSpriteList;
}

//!
//! \return la liste des sprites de cette scène (y compris ceux qui ne sont pas visibles).
//!
QList<Sprite*> GameScene::sprites() const  {
    QList<Sprite*> SpriteList;
    QList<QGraphicsItem*> AllItems = this->items();
    foreach(QGraphicsItem* pItem, AllItems) {
        if (pItem->type() == Sprite::SpriteItemType)
            SpriteList << static_cast<Sprite*>(pItem);
    }
    return SpriteList;
}

//!
//! Affiche un texte sur la scène.
//! Si ce texte n'est pas détruit manuellement, il le sera automatiquement
//! lorsque la scène sera détruite.
//! \param InitialPosition  Position initiale du texte.
//! \param rText            Texte à afficher.
//! \param Size             Taille (en pixels) du texte.
//! \param Color            Couleur du texte.
//! \return un pointeur sur l'élément graphique textuel.
//!
QGraphicsSimpleTextItem* GameScene::createText(QPointF InitialPosition, const QString& rText, int Size, QColor Color) {
    QGraphicsSimpleTextItem* pText = this->addSimpleText(rText);
    pText->setPos(InitialPosition);
    QFont TextFont = pText->font();
    TextFont.setPixelSize(Size);
    pText->setFont(TextFont);
    pText->setBrush(QBrush(Color));
    return pText;
}

//! Change la largeur de la scène.
//! \param SceneWidth   Largeur de la scène en pixels.
void GameScene::setWidth(int SceneWidth)  {
    setSceneRect(0,0, SceneWidth, height());
}

//! Change la hauteur de la scène.
//! \param SceneHeight   Hauteur de la scène, en pixels.
void GameScene::setHeight(int SceneHeight)  {
    setSceneRect(0,0, width(), SceneHeight);
}

//!
//! Démarre la génération d'un tick sur une base de temps régulière,
//! donnée en paramètre.
//! \param TickInterval  Intervalle de temps (en millisecondes) entre chaque tick. Si cette valeur est
//! inférieure à zéro, l'intervalle de temps précédent est utilisé.
//!
void GameScene::startTick(int TickInterval)  {
    if (TickInterval < 0)
        TickInterval = m_LastTickInterval;

    m_LastTickInterval = TickInterval;
    m_LastUpdateTime.start();
    m_KeepTicking = true;
    QTimer::singleShot(TickInterval, this, SLOT(onTick()));
}

//!
//! Arrête la génération du tick.
//!
void GameScene::stopTick()  {
    m_KeepTicking = false;
}

//! Traite le tick : le temps exact écoulé entre ce tick et le tick précédent
//! est mesuré et l'objet GameCore est lui-même informé du tick.
//! Poursuit la génération du tick si nécessaire.
void GameScene::onTick() {

    int ElapsedTime = m_LastUpdateTime.elapsed();

    // On évite une division par zéro (peu probable, mais on sait jamais)
    if (ElapsedTime < 1)
        ElapsedTime = 1;

    m_LastUpdateTime.start();

    m_pGameCore->tick(ElapsedTime);

    if (m_KeepTicking)
        QTimer::singleShot(m_LastTickInterval, this, SLOT(onTick()));
}

//!
//! \brief Retourne un pointeur vers le GameCore.
//! \return Pointeur vers l'objet GameCore
//! \author Clark Winkelmann
//!
GameCore* GameScene::getCore(){
    return m_pGameCore;
}
