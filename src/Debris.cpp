#include "Debris.h"
#include <QPainter>
#include <QRandomGenerator>

Debris::Debris() {
    setZValue(7);

    auto* rng = QRandomGenerator::global();
    const double r1 = rng->generateDouble();  // [0,1)
    const double r2 = rng->generateDouble();  // [0,1)

    m_vx = -3.0 + r1 * 6.0;                   // [-3, 3]
    m_vy = -5.0 + r2 * 4.0;                   // [-5, -1]
}

QRectF Debris::boundingRect() const { return QRectF(-2, -2, 4, 4); }

void Debris::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    p->setRenderHint(QPainter::Antialiasing, false);
    p->setPen(Qt::NoPen);
    const int alpha = 80 + (m_life * 6);
    p->setBrush(QColor(255, 230, 120, alpha));
    p->drawRect(-2, -2, 4, 4);
}

void Debris::advance(int phase) {
    if (phase == 0) return;
    setX(x() + m_vx);
    setY(y() + m_vy);
    m_vy += 0.25;
    if (--m_life <= 0) { deleteLater(); return; }
    update();
}

