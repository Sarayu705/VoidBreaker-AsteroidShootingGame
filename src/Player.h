#pragma once
#include <QGraphicsObject>
#include <QElapsedTimer>
#include "PowerUp.h"

class Player : public QGraphicsObject {
    Q_OBJECT
public:
    explicit Player(int playerIndex = 1);

    int playerIndex() const { return m_playerIndex; }

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void advance(int phase) override;

    // Direct input handler (bypasses Qt single-item focus limitation)
    void handleInputKey(int key, bool isPress);

    // Power-up & status hooks
    void setInvuln(int ms);
    void applyPowerUp(PowerUp::Type type);
    bool hasShield() const { return m_shieldActive; }
    void popShield();

signals:
    void shootRequested();

protected:
    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;

private:
    int m_playerIndex = 1;

    // Sprite dimensions
    int m_wCells = 5;
    int m_hCells = 5;

    // Input state
    bool m_leftHeld  = false;
    bool m_rightHeld = false;
    bool m_shootHeld = false;

    // Timers & Cooldowns
    QElapsedTimer m_fireTimer;
    bool m_invuln = false;
    qint64 m_invulnEndMs = 0;

    // Power-up states
    int m_tripleShotMs = 0;
    bool m_shieldActive = false;

    // Movement accumulator
    qreal m_moveAcc = 0;

    void doShootIfReady();
    void stepMove();
};
