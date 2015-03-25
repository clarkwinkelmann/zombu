/**
  \file
  \brief    Définition de la classe Sprite.
  \author   JCO
  \date     janvier 2014
*/
#include "sprite.h"

#include <QPainter>

const int NO_CURRENT_FRAME = -1;

//! Construit un sprite et l'initialise.
//! Le sprite n'a pas d'apparence particulière et n'affichera rien
//! tant qu'une image ne lui sera pas fournie avec addAnimationFrame().
//! \param pParent  Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
Sprite::Sprite(QGraphicsItem* pParent) : QGraphicsPixmapItem(pParent) {
    init();
}

//! Construit un sprite et l'initialise.
//! Le sprite utilisera l'image fournie pour son apparence.
//! \param rPixmap   Image à utiliser pour l'apparence du sprite.
//! \param pParent   Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
Sprite::Sprite(const QPixmap& rPixmap, QGraphicsItem* pParent) : QGraphicsPixmapItem(pParent) {
    init();
    addAnimationFrame(rPixmap);
}
//! Ajoute une image au cycle d'animation.
//! \param rPixmap  Image à ajouter.
void Sprite::addAnimationFrame(const QPixmap& rPixmap) {
    m_AnimationFrames << rPixmap;
    onNextAnimationFrame();
}

//! Change l'image d'animation présentée par le sprite.
//! L'image doit avoir été au préalable ajoutée aux images d'animation avec addAnimationFrame().
//! \param FrameIndex   Index de l'image à utiliser.
void Sprite::setCurrentAnimationFrame(int FrameIndex) {
    if (m_AnimationFrames.isEmpty())
        return;

    if (FrameIndex < 0 || FrameIndex >= m_AnimationFrames.count())
        FrameIndex = 0;

    m_CurrentAnimationFrame = FrameIndex;
    setPixmap(m_AnimationFrames[FrameIndex]);
}

//! Affiche l'image suivante.
void Sprite::showNextAnimationFrame() {
    onNextAnimationFrame();
}

//! Change la vitesse d'animation.
//! \param FrameDuration   Durée d'une image en millisecondes.
void Sprite::setAnimationSpeed(int FrameDuration) {
    if (FrameDuration <= 0)
        m_AnimationTimer.stop();
    else if (FrameDuration != m_FrameDuration) {
        m_FrameDuration = FrameDuration;
        m_AnimationTimer.setInterval(FrameDuration);
    }
}

//! Arrête l'animation.
void Sprite::stopAnimation() {
    m_AnimationTimer.stop();
}

//! Démarre l'animation.
//! La vitesse d'animation utilisée est celle qui a été
//! spécifiée avec setAnimationSpeed().
void Sprite::startAnimation() {
    m_CurrentAnimationFrame = NO_CURRENT_FRAME;
    onNextAnimationFrame();
    m_AnimationTimer.start();
}

//! Démarre l'animation à la vitesse donnée.
//! Cette vitesse donnée remplace la précédente.
//! \param FrameDuration   Durée d'une image en millisecondes.
void Sprite::startAnimation(int FrameDuration)  {
    setAnimationSpeed(FrameDuration);
    startAnimation();
}

//! Choisi si le signal animationFinished() doit être émis chaque fois que l'animation
//! est terminée.
//! \param Enabled   Indique si le signal doit être émis (true) ou non (false).
void Sprite::setEmitSignalEndOfAnimationEnabled(bool Enabled) {
    m_EmitSignalEOA = Enabled;
}

//! \return un booléen qui indique si le signal animationFinished() est émis
//! chaque fois que l'animation est terminée.
bool Sprite::isEmitSignalEndOfAnimationEnabled() const {
    return m_EmitSignalEOA;
}

//! Mémorise une information de type personnalisée.
//! \param Type  Information de type.
void Sprite::setCustomType(int Type) {
    m_CustomType = Type;
}

//! \return l'information de type personnalisée attribuée à ce sprite.
int Sprite::customType() const {
    return m_CustomType;
}

#ifdef DEBUG_BBOX
//! Dessine le sprite, avec sa boundingbox qui l'entoure.
void Sprite::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget) {
    QGraphicsPixmapItem::paint(pPainter, pOption, pWidget);
    pPainter->setPen(Qt::white);
    pPainter->drawRect(this->boundingRect());
}
#endif

//! Initialise le sprite.
void Sprite::init() {

    m_EmitSignalEOA = false;
    m_FrameDuration = 0;
    m_CurrentAnimationFrame = NO_CURRENT_FRAME;
    m_CustomType = -1;
    connect(&m_AnimationTimer, SIGNAL(timeout()), this, SLOT(onNextAnimationFrame()));
}

//! Affiche l'image suivante de l'animation.
//! Si la dernière image est affichée, l'animation reprend au début et,
//! selon la configuration, le signal animationFinished() est émis.
void Sprite::onNextAnimationFrame() {
    if (m_AnimationFrames.isEmpty()) {
        m_CurrentAnimationFrame = NO_CURRENT_FRAME;
        return;
    }

    int PreviousAnimationFrame = m_CurrentAnimationFrame;
    ++m_CurrentAnimationFrame;
    if (m_CurrentAnimationFrame >= m_AnimationFrames.count()) {
        m_CurrentAnimationFrame = 0;
        if (m_EmitSignalEOA)
            emit animationFinished();
    }
    if (PreviousAnimationFrame != m_CurrentAnimationFrame) {
        setPixmap(m_AnimationFrames[m_CurrentAnimationFrame]);
        update();
    }
}
