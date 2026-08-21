#include "Player.h"
#include "Constants.h"
#include "PixelArt.h"
#include "Game.h"
#include "Bullet.h"

#include <QKeyEvent>
#include <QGraphicsScene>
#include <QPainter>

Player::Player(int playerIndex)
    : m_playerIndex(playerIndex)
{
    setFlag(ItemIsFocusable, true);
    setFocus();
    setZValue(5);
    m_fireTimer.start();
}

QRectF Player::boundingRect() const {
    const int w = 5 * Const::RASTER;
    const int h = 9 * Const::RASTER;
    return QRectF(-w/2 - 8, -h/2 - 8, w + 16, h + 16);
}

void Player::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    // Player 1 Sprite (Blue Falcon)
    static const QVector<QString> spriteP1 = {
        "..w..",
        ".www.",
        ".wrw.",
        "wwrww",
        "wwrww",
        ".www.",
        "..f..",
        ".f.f.",
        "f...f"
    };

    static const QHash<QChar,QColor> palP1 = {
        { 'w', QColor(173, 220, 255) },  // Cyan/steel blue body
        { 'r', QColor(0, 180, 255) },   // Blue stripe
        { 'f', QColor(255, 150, 0) }    // Thruster flame
    };

    // Player 2 Sprite (Red Phoenix)
    static const QVector<QString> spriteP2 = {
        "..w..",
        ".www.",
        ".wrw.",
        "wwrww",
        "wwrww",
        ".www.",
        "..f..",
        ".f.f.",
        "f...f"
    };

    static const QHash<QChar,QColor> palP2 = {
        { 'w', QColor(255, 200, 150) },  // Gold/warm body
        { 'r', QColor(255, 50, 50) },    // Crimson stripe
        { 'f', QColor(255, 220, 0) }     // Bright yellow flame
    };

    const auto& currentSprite = (m_playerIndex == 1) ? spriteP1 : spriteP2;
    const auto& currentPal    = (m_playerIndex == 1) ? palP1    : palP2;

    // Render Plasma Shield Bubble if active
    if (m_shieldActive) {
        p->setRenderHint(QPainter::Antialiasing, true);
        p->setPen(QPen(QColor(0, 230, 255, 220), 3));
        p->setBrush(QBrush(QColor(0, 200, 255, 45)));
        p->drawEllipse(boundingRect().adjusted(2, 2, -2, -2));
        p->setRenderHint(QPainter::Antialiasing, false);
    }

    if (m_invuln) {
        // Flash while invulnerable
        if ((m_fireTimer.elapsed() / 120) % 2 == 0) {
            drawPixelArt(p, currentSprite, currentPal, true);
        }
    } else {
        drawPixelArt(p, currentSprite, currentPal, true);
    }
}

void Player::advance(int phase) {
    if (phase == 0) return;

    // Movement & shooting auto-hold
    stepMove();
    if (m_shootHeld) doShootIfReady();

    // Decrement Triple Shot duration
    if (m_tripleShotMs > 0) {
        m_tripleShotMs -= Const::TICK_MS;
        if (m_tripleShotMs < 0) m_tripleShotMs = 0;
    }

    // Cancel invulnerability
    if (m_invuln && m_fireTimer.elapsed() >= m_invulnEndMs) {
        m_invuln = false;
    }

    if (!m_leftHeld && !m_rightHeld) {
        m_moveAcc = 0;
    }

    // Clamp inside scene bounds
    qreal nx = std::clamp(x(), (qreal)Const::RASTER/2.0, (qreal)Const::SCENE_W - Const::RASTER/2.0);
    nx = snapRaster(nx);
    setX(nx);
}

void Player::setInvuln(int ms) {
    m_invuln = true;
    m_invulnEndMs = m_fireTimer.elapsed() + ms;
}

void Player::applyPowerUp(PowerUp::Type type) {
    switch (type) {
    case PowerUp::Type::TripleShot:
        m_tripleShotMs = 10000; // 10 seconds of triple shot
        break;
    case PowerUp::Type::Shield:
        m_shieldActive = true;
        break;
    case PowerUp::Type::Heal:
        Game::instance()->healPlayer(m_playerIndex, 25);
        break;
    }
    update();
}

void Player::popShield() {
    m_shieldActive = false;
    setInvuln(1000);
    update();
}

void Player::handleInputKey(int key, bool isPress) {
    if (m_playerIndex == 1) {
        // Player 1: A / D to move, Space / Z to shoot
        if (key == Qt::Key_A) m_leftHeld = isPress;
        if (key == Qt::Key_D) m_rightHeld = isPress;
        if (key == Qt::Key_Space || key == Qt::Key_Z) {
            m_shootHeld = isPress;
            if (isPress) doShootIfReady();
        }
    } else {
        // Player 2: Left / Right Arrows to move, Enter / M to shoot
        if (key == Qt::Key_Left) m_leftHeld = isPress;
        if (key == Qt::Key_Right) m_rightHeld = isPress;
        if (key == Qt::Key_Return || key == Qt::Key_M) {
            m_shootHeld = isPress;
            if (isPress) doShootIfReady();
        }
    }
}

void Player::keyPressEvent(QKeyEvent* e) {
    if (e->isAutoRepeat()) return;
    handleInputKey(e->key(), true);
}

void Player::keyReleaseEvent(QKeyEvent* e) {
    if (e->isAutoRepeat()) return;
    handleInputKey(e->key(), false);
}

void Player::doShootIfReady() {
    if (m_fireTimer.elapsed() < Const::PLAYER_COOLDOWN_MS) return;
    m_fireTimer.restart();

    if (auto* sc = scene()) {
        const qreal spawnY = y() - (5 * Const::RASTER);

        if (m_tripleShotMs > 0) {
            // Triple Shot: center bullet + 2 angled spread bullets
            auto* bCenter = new Bullet(0.0, -Const::BULLET_SPEED, m_playerIndex);
            bCenter->setPos(x(), spawnY);

            auto* bLeft = new Bullet(-2.5, -Const::BULLET_SPEED * 0.95, m_playerIndex);
            bLeft->setPos(x() - 6, spawnY);

            auto* bRight = new Bullet(2.5, -Const::BULLET_SPEED * 0.95, m_playerIndex);
            bRight->setPos(x() + 6, spawnY);

            sc->addItem(bCenter);
            sc->addItem(bLeft);
            sc->addItem(bRight);
        } else {
            // Standard single bullet
            auto* b = new Bullet(0.0, -Const::BULLET_SPEED, m_playerIndex);
            b->setPos(x(), spawnY);
            sc->addItem(b);
        }
    }
}

void Player::stepMove() {
    const qreal step = Const::RASTER * 1.0;
    if (m_leftHeld == m_rightHeld) return;
    qreal dir = m_leftHeld ? -1.0 : 1.0;
    m_moveAcc += dir * step;

    while (std::abs(m_moveAcc) >= Const::RASTER) {
        const qreal nx = std::clamp(x() + (m_moveAcc > 0 ? Const::RASTER : -Const::RASTER),
                                    (qreal)Const::RASTER/2.0,
                                    (qreal)Const::SCENE_W - Const::RASTER/2.0);
        setX(nx);
        m_moveAcc += (m_moveAcc > 0 ? -Const::RASTER : Const::RASTER);
    }
}
