#pragma once
#include <QGraphicsObject>
#include "Constants.h"

class Bullet : public QGraphicsObject {
    Q_OBJECT
public:
    explicit Bullet(qreal vx = 0.0, qreal vy = -Const::BULLET_SPEED, int ownerIndex = 1);

    int ownerIndex() const { return m_ownerIndex; }

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void advance(int phase) override;

private:
    qreal m_vx = 0.0;
    qreal m_vy = -10.0;
    int m_ownerIndex = 1;
};
