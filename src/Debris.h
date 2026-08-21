#pragma once
#include <QGraphicsObject>

class Debris : public QGraphicsObject {
    Q_OBJECT
public:
    Debris();
    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void advance(int phase) override;

private:
    qreal m_vx {0.0}, m_vy {0.0};
    int   m_life {28};
};
