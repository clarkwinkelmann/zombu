/**
  \file
  \brief    Déclaration de la classe GameScene.
  \author   JCO
  \date     janvier 2014
*/
#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTime>

class GameCore;
class Sprite;
class QGraphicsSimpleTextItem;
class GameStatus;

//! \brief Représente l'espace 2D du jeu.
//!
//! GameScene représente la scène utilisée par l'interface graphique pour afficher l'espace 2D du jeu dans un objet QGraphicsView.
//!
//! La taille de l'espace de jeu (appelé une *scene*) peut être spécifié avec les méthodes setSceneWidth() et setSceneHeight().
//! Cette taille peut être ensuite obtenue avec les méthodes sceneWidth() et sceneHeight().
//!
//! Cette classe met à disposition différentes méthodes pour simplifier le travail de développement d'un jeu :
//! - Création de sprites (Sprite) avec la méthode createSprite()
//! - Détection de collisions avec la méthode collidingSprites()
//! - Affichage de textes avec la méthode createText()
//! - Génération d'un tick (un clock) régulier donnant la cadence du jeu avec startTick() et stopTick().
//!
//! Cette classe délègue toute la partie logique du jeu à la classe GameCore. Elle lui transmet d'ailleurs tous les
//! événements clavier qu'elle reçoit.
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum { KEEP_PREVIOUS_TICK_INTERVAL = -1  };

    explicit GameScene(QObject* pParent = 0);
    ~GameScene();

    virtual void keyPressEvent(QKeyEvent* pKeyEvent);
    virtual void keyReleaseEvent(QKeyEvent* pKeyEvent);

    Sprite* createSprite();
    Sprite* createSprite(QPointF InitialPosition, const QPixmap& rImage);
    QList<Sprite*> collidingSprites(const Sprite* pSprite) const;
    QList<Sprite*> collidingSprites(const QRectF& rRect) const;
    QList<Sprite*> sprites() const;

    QGraphicsSimpleTextItem* createText(QPointF InitialPosition, const QString& rText, int Size = 10, QColor Color=Qt::white);

    void setWidth(int SceneWidth);
    void setHeight(int SceneHeight);

    void startTick(int TickInterval = KEEP_PREVIOUS_TICK_INTERVAL);
    void stopTick();

    GameCore* getCore();

private:
    GameCore* m_pGameCore;
    QTime m_LastUpdateTime;
    QGraphicsSimpleTextItem* m_pDetailedInfosItem;

    int m_LastTickInterval;
    bool m_KeepTicking;

private slots:
    void onTick();
};

#endif // GAMESCENE_H
