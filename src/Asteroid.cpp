#include "Asteroid.h"
#include "Constants.h"
#include "PixelArt.h"
#include "Explosion.h"
#include "Game.h"

#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QTimer>

/*static QVector<QString> spriteFor(Asteroid::Size s) {
    // Simple rounded rocks in 10..14 cells; different sizes
    if (s == Asteroid::Small) {
        return {
            "...",
            ".c.",
            "..."
        };
    } else if (s == Asteroid::Medium) {
        return {
                ".c.",
                "ccc",
                ".c."
        };
    }
    // Large
    return {
            ".ccc.",
            "ccccc",
            "ccccc",
            ".ccc."
    };
} */

static QVector<QString> spriteFor(Asteroid::Size s) {
    if (s == Asteroid::Small) {
        return {
            "..cc..",
            ".cccc.",
            "cccccc",
            ".cccc.",
            "..cc.."
        };
    }
    else if (s == Asteroid::Medium) {
        return {
            "..cccc..",
            ".cccccc.",
            "cccccccc",
            "cccccccc",
            "cccccccc",
            ".cccccc.",
            "..cccc.."
        };
    }

    // Large
    return {
        "...cccccc...",
        "..cccccccc..",
        ".cccccccccc.",
        "cccccccccccc",
        "cccccccccccc",
        "cccccccccccc",
        "cccccccccccc",
        ".cccccccccc.",
        "..cccccccc..",
        "...cccccc..."
    };
}

static QHash<QChar,QColor> rockPal() {
    return {
        { 'c', QColor(200,170,150) }
    };
}

Asteroid::Asteroid(Size s) : m_size(s) {
    setZValue(2);

    auto* rng = QRandomGenerator::global();
    const double r = rng->generateDouble();
    qreal base = Const::AST_MIN_SPEED + r * (Const::AST_MAX_SPEED - Const::AST_MIN_SPEED);
    if (auto* g = Game::instance()) base *= g->difficultySpeedMul();
    m_speed = base;
}

QRectF Asteroid::boundingRect() const {
    const int w = spriteFor(m_size)[0].size();
    const int h = spriteFor(m_size).size();
    return QRectF(- (w*Const::RASTER)/2.0, - (h*Const::RASTER)/2.0,
                   w*Const::RASTER,        h*Const::RASTER);
}

void Asteroid::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    drawPixelArt(p, spriteFor(m_size), rockPal(), true);
}

void Asteroid::advance(int phase) {
    if (phase == 0 || m_dead) return;

    // accumulate float speed and step whole raster cells
    m_accum += m_speed;
    while (m_accum >= Const::RASTER) {
        setY(y() + Const::RASTER);
        m_accum -= Const::RASTER;
    }

    if (y() - boundingRect().top() > Const::SCENE_H + 40) {
        m_dead = true;
        if (auto* sc = scene()) {
            QTimer::singleShot(0, sc, [](){
                if (auto* g = Game::instance()) g->playerHit(g->leakDamage());
            });
        }
        deleteLater();
    }
}

void Asteroid::hit() {
    if (m_dead) return;
    m_dead = true;

    const QPointF posNow = pos();
    const Size sNow = m_size;
    if (auto* g = Game::instance()) {
        g->addScore(points());
        g->spawnPowerUpAt(posNow);
    }

    if (auto* sc = scene()) {
        QTimer::singleShot(0, sc, [sc, posNow, sNow](){
            auto* ex = new Explosion();
            ex->setPos(posNow);
            sc->addItem(ex);

            if (sNow > Small) {
                auto next = static_cast<Size>(static_cast<int>(sNow) - 1);
                auto* a1 = new Asteroid(next);
                auto* a2 = new Asteroid(next);
                a1->setPos(posNow.x() - Const::RASTER, posNow.y());
                a2->setPos(posNow.x() + Const::RASTER, posNow.y());
                sc->addItem(a1);
                sc->addItem(a2);
            }
        });
    }
    deleteLater();
}

void Asteroid::destroy(bool forceExp) {
    if (m_dead) return;
    m_dead = true;

    const QPointF posNow = pos();
    const Size sNow = m_size;
    if (auto* g = Game::instance()) g->addScore(points());

    if (auto* sc = scene()) {
        QTimer::singleShot(0, sc, [sc, posNow, sNow, forceExp](){
            auto* ex = new Explosion();
            ex->setPos(posNow);
            sc->addItem(ex);

            if (!forceExp && sNow > Small) {
                auto next = static_cast<Size>(static_cast<int>(sNow) - 1);
                auto* a1 = new Asteroid(next);
                auto* a2 = new Asteroid(next);
                a1->setPos(posNow.x() - Const::RASTER, posNow.y());
                a2->setPos(posNow.x() + Const::RASTER, posNow.y());
                sc->addItem(a1);
                sc->addItem(a2);
            }
        });
    }
    deleteLater();
}

int Asteroid::points() const {
    switch (m_size) {
        case Small:  return Const::AST_SCORE_SMALL;
        case Medium: return Const::AST_SCORE_MED;
        default:     return Const::AST_SCORE_LARGE;
    }
}
