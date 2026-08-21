#include "Mine.h"
#include "Constants.h"
#include "PixelArt.h"
#include "Game.h"

#include <QGraphicsScene>
#include <QtMath>

Mine::Mine() { setZValue(3); }

QRectF Mine::boundingRect() const {
    // 9x9 cell mine
    return QRectF(- (5*Const::RASTER)/2.0, - (5*Const::RASTER)/2.0,
                   5*Const::RASTER,         5*Const::RASTER);
}

void Mine::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    static const QVector<QString> sprite = {
                                            ".kKk.",
                                            "kGGGk",
                                            "KGsGK",
                                            "kGGGk",
                                            ".kKk."
    };
    static const QHash<QChar,QColor> pal = {
        { 'k', QColor(60,60,60) },
        { 'g', QColor(120,120,120) },
        { 'K', QColor(200,40,40) },
        { 's', QColor(255,200,50) }
    };
    drawPixelArt(p, sprite, pal, true);
}

void Mine::advance(int phase) {
    if (phase == 0) return;

    // fall in raster steps
    m_accum += Const::MINE_SPEED;
    while (m_accum >= Const::RASTER) {
        setY(y() + Const::RASTER);
        m_accum -= Const::RASTER;
    }

    // sway (snap per cell)
    m_t += Const::MINE_SINE_FREQ;
    const qreal sx = std::round((qSin(m_t) * Const::MINE_SINE_AMPL) / Const::RASTER) * Const::RASTER;
    setX(std::clamp((qreal)(x() + sx) , (qreal)Const::RASTER/2.0, (qreal)Const::SCENE_W - Const::RASTER/2.0));

    if (y() - boundingRect().top() > Const::SCENE_H + 40) {
        deleteLater();
        return;
    }

    // mine kills player on contact is already handled in your Player/Game collision logic
}
