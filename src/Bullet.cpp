#include "Bullet.h"
#include "Constants.h"
#include "PixelArt.h"
#include "Asteroid.h"
#include "Mine.h"
#include "Player.h"
#include "Explosion.h"
#include "Game.h"

#include <QGraphicsScene>

Bullet::Bullet(qreal vx, qreal vy, int ownerIndex)
    : m_vx(vx), m_vy(vy), m_ownerIndex(ownerIndex)
{
    setZValue(4);
}

QRectF Bullet::boundingRect() const {
    return QRectF(- (Const::BULLET_W_CELLS*Const::RASTER)/2.0,
                  - (Const::BULLET_H_CELLS*Const::RASTER)/2.0,
                  Const::BULLET_W_CELLS*Const::RASTER,
                  Const::BULLET_H_CELLS*Const::RASTER);
}

void Bullet::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    static const QVector<QString> sprite = {
        "R",
        "R",
        "R"
    };

    // Color P1 lasers Cyan, P2 lasers Red/Gold
    QColor bulletColor = (m_ownerIndex == 1) ? QColor(0, 230, 255) : QColor(255, 90, 50);
    static const QHash<QChar,QColor> palP1 = { {'R', QColor(0, 230, 255)} };
    static const QHash<QChar,QColor> palP2 = { {'R', QColor(255, 90, 50)} };

    drawPixelArt(p, sprite, (m_ownerIndex == 1) ? palP1 : palP2, true);
}

void Bullet::advance(int phase) {
    if (phase == 0) return;

    setPos(x() + m_vx, y() + m_vy);

    // Collisions
    for (auto* it : collidingItems()) {
        // PvP Combat: Bullet hits opposing player ship!
        if (auto* targetPlayer = dynamic_cast<Player*>(it)) {
            if (targetPlayer->playerIndex() != m_ownerIndex) {
                auto* exp = new Explosion();
                exp->setPos(pos());
                scene()->addItem(exp);

                if (auto* g = Game::instance()) {
                    g->addScore(50, m_ownerIndex);           // Award 50 PvP points to shooter
                    g->playerHit(20, targetPlayer);          // Deal 20 damage to target player
                }
                deleteLater();
                return;
            }
        }

        if (auto* a = dynamic_cast<Asteroid*>(it)) {
            a->hit();
            if (auto* g = Game::instance()) {
                g->addScore(a->points(), m_ownerIndex);
            }
            deleteLater();
            return;
        }

        if (auto* m = dynamic_cast<Mine*>(it)) {
            auto* exp = new Explosion();
            exp->setPos(m->pos());
            scene()->addItem(exp);

            if (auto* g = Game::instance()) {
                g->addScore(100, m_ownerIndex);
            }

            m->deleteLater();
            deleteLater();
            return;
        }
    }

    // Out of scene bounds
    if (y() + boundingRect().top() <= -10 || y() >= Const::SCENE_H + 20 || x() < -20 || x() > Const::SCENE_W + 20) {
        deleteLater();
    }
}
