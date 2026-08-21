#pragma once
#include <QGraphicsObject>

class PowerUp : public QGraphicsObject {
    Q_OBJECT
public:
    enum class Type { TripleShot, Shield, Heal };

    explicit PowerUp(Type type);

    Type powerUpType() const { return m_type; }

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void advance(int phase) override;

private:
    Type m_type;
    qreal m_speed = 1.5;
    int m_animFrame = 0;
};
