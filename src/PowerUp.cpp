#include "PowerUp.h"
#include "Constants.h"
#include <QPainter>

PowerUp::PowerUp(Type type)
    : m_type(type)
{
    setZValue(50);
}

QRectF PowerUp::boundingRect() const {
    const qreal sz = 5 * Const::RASTER; // 25px box
    return QRectF(-sz / 2.0, -sz / 2.0, sz, sz);
}

void PowerUp::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing, false);

    const qreal radius = 10.0;

    QColor mainColor;
    QString symbol;

    switch (m_type) {
    case Type::TripleShot:
        mainColor = QColor(255, 215, 0); // Gold
        symbol = "3X";
        break;
    case Type::Shield:
        mainColor = QColor(0, 220, 255); // Neon Cyan
        symbol = "SH";
        break;
    case Type::Heal:
        mainColor = QColor(50, 220, 100); // Emerald Green
        symbol = "HP";
        break;
    }

    // Outer pulsating retro capsule box
    painter->setPen(QPen(Qt::white, 2));
    painter->setBrush(QBrush(mainColor));
    painter->drawRoundedRect(boundingRect().adjusted(2, 2, -2, -2), 4, 4);

    // Inner symbol text
    painter->setPen(Qt::black);
    QFont f("Courier", 9, QFont::Bold);
    f.setStyleStrategy(QFont::NoAntialias);
    painter->setFont(f);
    painter->drawText(boundingRect(), Qt::AlignCenter, symbol);
}

void PowerUp::advance(int phase) {
    if (!phase) return;

    setPos(x(), y() + m_speed);
    m_animFrame++;

    // Despawn if off-screen bottom
    if (y() > Const::SCENE_H + 30) {
        deleteLater();
    }
}
