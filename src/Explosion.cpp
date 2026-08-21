#include "Explosion.h"
#include "Constants.h"
#include "Debris.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QRandomGenerator>

static inline QColor lerp(const QColor& a, const QColor& b, qreal t) {
    return QColor(
        a.red()   + (b.red()   - a.red())   * t,
        a.green() + (b.green() - a.green()) * t,
        a.blue()  + (b.blue()  - a.blue())  * t,
        a.alpha() + (b.alpha() - a.alpha()) * t
        );
}


Explosion::Explosion() {
    setZValue(8);
}

/* QRectF Explosion::boundingRect() const {
    return QRectF(-24, -24, 48, 48);
} */

QRectF Explosion::boundingRect() const {
    const int maxCells = 7;        // maximum radius in "pixels"
    const qreal s = Const::RASTER; // each pixel size
    return QRectF(-maxCells*s, -maxCells*s, (maxCells*2+1)*s, (maxCells*2+1)*s);
}

/* void Explosion::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    p->setRenderHint(QPainter::Antialiasing, false);

    const qreal t = m_frame / qreal(Const::EXP_FRAMES); // 0..1
    const int   r = 6 + int(24 * t);

    const QColor inner(255, 200, 80,  int(255 * (1.0 - t)));
    const QColor outer(255,  80, 40,  int(255 * (1.0 - t)));

    p->setPen(Qt::NoPen);
    p->setBrush(outer);
    p->drawEllipse(-r, -r, 2*r, 2*r);
    p->setBrush(inner);
    p->drawEllipse(-r/2, -r/2, r, r);
} */

/* void Explosion::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    // Disable all smoothing
    p->save();
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setRenderHint(QPainter::SmoothPixmapTransform, false);

    const qreal t = m_frame / qreal(Const::EXP_FRAMES); // 0..1
    const int maxSize = 24;                              // max explosion size
    const int pixelSize = 4;                             // size of each "pixel"

    const int radius = 6 + int(maxSize * t);            // explosion grows over time

    const QColor inner(255, 200, 80, int(255 * (1.0 - t)));
    const QColor outer(255, 80, 40, int(255 * (1.0 - t)));

    p->setPen(Qt::NoPen);

    // Draw outer pixels (randomized jagged circle)
    p->setBrush(outer);
    for (int x = -radius; x <= radius; x += pixelSize) {
        for (int y = -radius; y <= radius; y += pixelSize) {
            int dist = x*x + y*y;
            if (dist <= radius*radius && QRandomGenerator::global()->bounded(100) > 20) {
                // 80% chance to draw pixel, makes it jagged
                p->drawRect(x, y, pixelSize, pixelSize);
            }
        }
    }

    // Draw inner pixels (more dense cluster)
    p->setBrush(inner);
    int innerRadius = radius / 2;
    for (int x = -innerRadius; x <= innerRadius; x += pixelSize) {
        for (int y = -innerRadius; y <= innerRadius; y += pixelSize) {
            int dist = x*x + y*y;
            if (dist <= innerRadius*innerRadius && QRandomGenerator::global()->bounded(100) > 10) {
                p->drawRect(x, y, pixelSize, pixelSize);
            }
        }
    }
} */

void Explosion::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    p->save();
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setRenderHint(QPainter::SmoothPixmapTransform, false);

    const int frames = Const::EXP_FRAMES;
    const qreal t = frames > 1 ? qBound(0.0, m_frame / qreal(frames - 1), 1.0) : 1.0;

    const int minR = 2;
    const int maxR = 7;
    const int r = qRound(minR + t * (maxR - minR));
    const qreal s = Const::RASTER;

    const QColor inner = QColor(255,240,120);
    const QColor mid   = QColor(255,120,60);
    const QColor outer = QColor(160,60,40);

    for (int gy = -r; gy <= r; ++gy) {
        for (int gx = -r; gx <= r; ++gx) {
            const int md = qMax(qAbs(gx), qAbs(gy));
            if (md != r && md != r - 1) continue;
            qreal ringT = md == r ? 1.0 : 0.5;
            QColor c = lerp(lerp(inner, mid, ringT), outer, t);
            p->fillRect(QRectF(gx*s, gy*s, s, s), c);
        }
    }

    p->restore();
}


void Explosion::advance(int phase) {
    if (phase == 0) return;

    if (!m_debrisSpawned && scene()) {
        m_debrisSpawned = true;
        for (int i = 0; i < Const::EXP_DEBRIS; ++i) {
            auto* d = new Debris();
            d->setPos(pos());
            scene()->addItem(d);
        }
    }

    ++m_frame;
    if (m_frame >= Const::EXP_FRAMES) { deleteLater(); return; }
    update();
}
