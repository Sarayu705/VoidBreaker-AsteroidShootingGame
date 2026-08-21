#pragma once
#include <QGraphicsObject>

class Asteroid : public QGraphicsObject {
    Q_OBJECT
public:
    enum Size { Large=2, Medium=1, Small=0 };

    explicit Asteroid(Size s = Large);

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void advance(int phase) override;

    void hit();                   // destroyed by bullet
    void destroy(bool forceExp);  // external destroy (keeps split behavior)
    int  points() const;

private:
    Size  m_size = Large;
    qreal m_speed = 1.5;
    qreal m_accum = 0;    // for raster stepping
    bool  m_dead  = false;
};
