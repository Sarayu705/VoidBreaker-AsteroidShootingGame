#pragma once
#include <QGraphicsObject>

class Mine : public QGraphicsObject {
    Q_OBJECT
public:
    Mine();

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void advance(int phase) override;

private:
    qreal m_accum = 0;
    qreal m_t = 0; // for sine sway
};
