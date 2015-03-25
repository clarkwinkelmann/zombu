/**
  \file
  \brief    Déclaration de la classe Sprite.
  \author   JCO
  \date     janvier 2014
*/
#ifndef SPRITE_H
#define SPRITE_H

//#define DEBUG_BBOX

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>
#include <QTimer>

//! \brief Classe qui représente un élément d'animation graphique 2D.
//!
//! Cette classe met à disposition différentes méthodes permettant de gérer de simplement un sprite animé.
//! Un sprite est un élément graphique qui affiche une image bitmap. Cet élément graphique doit appartenir à une scène.
//! Il possède un certain nombre de propriétés, associées à des méthodes, influençant son apparence et sa position :
//! - `pos` : la position 2D  du sprite
//! - `x` : la position sur l'axe horizontal du sprite
//! - `y` : la position sur l'axe vertical du sprite
//! - `z` : l'ordre z du sprite
//! - `rotation` : l'angle de rotation du sprite
//! - `scale` : un facteur de grossissement/rappetissment du sprite
//! - `opacity` : l'opacité du sprite, valeur décimal comprise en 0 et 1, 0 étant transparent et 1 opaque.
//!
//! Toutes ces propriétés peuvent être animées de façon très simple au moyen d'un objet QPropertyAnimation.
//!
//! D'autres méthodes permettent de connaître la géométrie et le placement du sprite : width(), height(), posX(), posY(), left(), right(), top(), bottom().
//! Il est également possible d'obtenir le rectangle dans lequel le sprite est inscrit avec globalBoundingBox().
//!
//! L'apparence du sprite n'est déterminée que par une seule image. Toutefois, il est possible d'en mémoriser plusieurs, afin de changer facilement d'apparence. Il est également possible de faire changer automatiquement ces images dans le but d'obtenir un sprite animé.
//!
//! La méthode addAnimationFrame() permet d'ajouter une image au sprite. La méthode setCurrentAnimationFrame() permet de spécifier quelle image doit être affichée. La méthode startAnimation() permet de démarrer l'animation des images. La méthode stopAnimation() permet de stopper l'animation des images. La vitesse d'animation peut être réglée avec setAnimationSpeed() ou au moment de démarrer l'animation.
//!
//! Il est également possible de demander au sprite d'émettre un signal chaque fois que l'animation est terminée, avec la méthode setEmitSignalEndOfAnimationEnabled().
//!
//! Il peut être utile, parmi de nombreux sprites, de pouvoir distinguer différentes catégories (ennemi, bombe, joueur, etc.). La méthode setCustomType() permet d'attribuer au sprite un numéro qui représente son type ou sa catégorie. Ce type peut être connu avec la méthode customType().
//!
//! Comme la classe Sprite spécialise la classe QGraphicsPixmapItem, qui elle-même spécialise la class QGraphicsItem, toutes les méthodes de QGraphicsItem sont accessibles au sprite.
//!
class Sprite : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged FINAL)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos FINAL)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged FINAL)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged FINAL)
    Q_PROPERTY(qreal z READ zValue WRITE setZValue NOTIFY zChanged FINAL)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QPointF transformOriginPoint READ transformOriginPoint WRITE setTransformOriginPoint)

public:
    Sprite(QGraphicsItem* pParent = 0);
    Sprite(const QPixmap& rPixmap, QGraphicsItem* pParent = 0);

    void addAnimationFrame(const QPixmap& rPixmap);
    void setCurrentAnimationFrame(int FrameIndex);
    void showNextAnimationFrame();
    void setAnimationSpeed(int FrameDuration);
    void stopAnimation();
    void startAnimation();
    void startAnimation(int FrameDuration);

    void setEmitSignalEndOfAnimationEnabled(bool Enabled);
    bool isEmitSignalEndOfAnimationEnabled() const;

    QRectF globalBoundingBox() const { return mapRectToScene(boundingRect());  }
    int width() const { return boundingRect().width(); }
    int height() const { return boundingRect().height(); }
    int posX() const { return pos().x(); }
    int posY() const { return pos().y(); }
    int left() const { return globalBoundingBox().left(); }
    int top() const { return globalBoundingBox().top(); }
    int right() const { return globalBoundingBox().right(); }
    int bottom() const { return globalBoundingBox().bottom(); }

    void setCustomType(int Type);
    int customType() const;

    enum { SpriteItemType = UserType + 1 };
    virtual int type() const { return SpriteItemType; }

#ifdef DEBUG_BBOX
    virtual void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);
#endif

signals:
    void animationFinished();
    void opacityChanged();
    void visibleChanged();
    void enabledChanged();
    void xChanged();
    void yChanged();
    void zChanged();
    void rotationChanged();
    void scaleChanged();

private:
    void init();

    QTimer m_AnimationTimer;

    bool m_EmitSignalEOA;

    QList<QPixmap> m_AnimationFrames;
    int m_FrameDuration;
    int m_CurrentAnimationFrame;

    int m_CustomType;

private slots:
    void onNextAnimationFrame();

};

#endif // SPRITE_H
