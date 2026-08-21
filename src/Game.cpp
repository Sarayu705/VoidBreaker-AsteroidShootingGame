#include "Game.h"
#include "Constants.h"
#include "Player.h"
#include "Asteroid.h"
#include "Mine.h"
#include "HUD.h"
#include "Star.h"
#include "PowerUp.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QRandomGenerator>
#include <QShortcut>
#include <QTimer>
#include <QTransform>
#include <QPixmap>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QKeyEvent>

static QPixmap bgGalaxy(int W, int H);
static QPixmap bgNebulaStorm(int W, int H);
static QPixmap bgNeon(int W, int H);
static QPixmap bgMoon(int W, int H);
static QPixmap bgToxic(int W, int H);
static QPixmap bgSolarFlare(int W, int H);

static QPixmap bgGalaxy(int W, int H) {
    QPixmap pm(W, H);
    pm.fill(QColor(10, 0, 30));
    QPainter p(&pm);

    for (int i = 0; i < 500; i++) {
        QColor col(100 + rand() % 155, 50, 255);
        p.setPen(col);
        p.drawPoint(rand() % W, rand() % H);
    }

    return pm;
}

static QPixmap bgNebulaStorm(int W, int H) {
    QPixmap pm(W, H);
    pm.fill(QColor(4, 6, 12));

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setRenderHint(QPainter::SmoothPixmapTransform, false);
    p.setPen(Qt::NoPen);

    int starCount = (W * H) / 2400;
    for (int i = 0; i < starCount; ++i) {
        int x = QRandomGenerator::global()->bounded(W);
        int y = QRandomGenerator::global()->bounded(H);
        int brightness = 180 + QRandomGenerator::global()->bounded(75);
        QColor c(brightness, brightness, brightness);
        int size = QRandomGenerator::global()->bounded(1, 3);
        p.fillRect(x, y, size, size, c);
    }

    for (int i = 0; i < 160; ++i) {
        int x = QRandomGenerator::global()->bounded(W);
        int y = QRandomGenerator::global()->bounded(H);
        int block = QRandomGenerator::global()->bounded(20, 60);
        int t = QRandomGenerator::global()->bounded(3);
        QColor neb;

        if (t == 0) {
            neb = QColor(120 + QRandomGenerator::global()->bounded(80),
                         40  + QRandomGenerator::global()->bounded(50),
                         150 + QRandomGenerator::global()->bounded(70), 65);
        } else if (t == 1) {
            neb = QColor(40  + QRandomGenerator::global()->bounded(60),
                         140 + QRandomGenerator::global()->bounded(80),
                         200 + QRandomGenerator::global()->bounded(55), 70);
        } else {
            neb = QColor(200 + QRandomGenerator::global()->bounded(55),
                         120 + QRandomGenerator::global()->bounded(55),
                         40  + QRandomGenerator::global()->bounded(30), 70);
        }

        p.fillRect(x, y, block, block, neb);
    }

    QPen fiber(Qt::white);
    fiber.setWidth(1);
    p.setPen(fiber);

    for (int i = 0; i < 12; ++i) {
        int x = QRandomGenerator::global()->bounded(W);
        int y = QRandomGenerator::global()->bounded(H);
        int dx = QRandomGenerator::global()->bounded(-40, 40);
        int dy = QRandomGenerator::global()->bounded(-40, 40);
        p.drawLine(x, y, x + dx, y + dy);
    }

    QPen scan(QColor(255,255,255,5));
    p.setPen(scan);
    for (int yy = 0; yy < H; yy += 3)
        p.drawLine(0, yy, W, yy);

    p.end();
    return pm;
}

static QPixmap bgNeon(int W, int H) {
    QPixmap pm(W, H);
    pm.fill(QColor(8, 2, 15));

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setRenderHint(QPainter::SmoothPixmapTransform, false);
    p.setPen(Qt::NoPen);

    for (int y = 0; y < H; y += 18) {
        for (int x = 0; x < W; x += 18) {
            if (QRandomGenerator::global()->bounded(7) == 0) {
                QColor noise(40 + QRandomGenerator::global()->bounded(40),
                             0 + QRandomGenerator::global()->bounded(20),
                             70 + QRandomGenerator::global()->bounded(80),
                             25 + QRandomGenerator::global()->bounded(60));
                p.fillRect(x, y, 18, 18, noise);
            }
        }
    }

    int beamCount = 5 + QRandomGenerator::global()->bounded(5);
    for (int i = 0; i < beamCount; i++) {
        int bx = QRandomGenerator::global()->bounded(W);
        int beamW = 3 + QRandomGenerator::global()->bounded(4);
        QColor core(180 + QRandomGenerator::global()->bounded(70), 0, 255, 220);
        p.fillRect(bx, 0, beamW, H, core);

        for (int g = 1; g <= 4; g++) {
            QColor glow(qMax(0, core.red() - g * 30),
                        qMax(0, core.green() - g * 10),
                        qMax(0, core.blue() - g * 40), 150 - g * 30);
            p.fillRect(bx - g, 0, beamW + g * 2, H, glow);
        }
    }

    for (int y = 0; y < H; y += 40) {
        QColor stripe(255, 0, 150, 40);
        p.fillRect(0, y, W, 2, stripe);
        for (int g = 1; g <= 3; g++) {
            QColor glow(255, 0, 150, 40 - g * 8);
            if (y - g >= 0) p.fillRect(0, y - g, W, 1, glow);
            if (y + g < H) p.fillRect(0, y + g, W, 1, glow);
        }
    }

    int dots = (W * H) / 3500;
    for (int i = 0; i < dots; i++) {
        int x = QRandomGenerator::global()->bounded(W);
        int y = QRandomGenerator::global()->bounded(H);
        QColor dot(200 + QRandomGenerator::global()->bounded(55), 0, 255, 255);
        p.fillRect(x, y, 1, 1, dot);
    }

    QPen scan(QColor(255, 255, 255, 8));
    p.setPen(scan);
    for (int y = 0; y < H; y += 3)
        p.drawLine(0, y, W, y);

    p.end();
    return pm;
}

static QPixmap bgMoon(int W, int H) {
    QPixmap pm(W, H);
    pm.fill(QColor(36, 36, 44));

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setPen(Qt::NoPen);

    for (int i = 0; i < (W * H) / 3000; ++i) {
        int x = QRandomGenerator::global()->bounded(W);
        int y = QRandomGenerator::global()->bounded(H);
        int r = 3 + QRandomGenerator::global()->bounded(12);
        QColor c(90 + QRandomGenerator::global()->bounded(60),
                 90 + QRandomGenerator::global()->bounded(60),
                 100 + QRandomGenerator::global()->bounded(40));
        p.setBrush(c);
        p.drawEllipse(x, y, r, r);
    }

    int cx = W - W/6;
    int cy = H/5;
    p.setBrush(QColor(100,100,110,30));
    p.drawEllipse(cx, cy, W/6, W/6);

    p.end();
    return pm;
}

static QPixmap bgToxic(int W, int H) {
    QPixmap pm(W, H);
    pm.fill(QColor(8, 30, 10));

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setPen(Qt::NoPen);

    const int pixel = 4;
    for (int y = 0; y < H; y += pixel) {
        for (int x = 0; x < W; x += pixel) {
            if (QRandomGenerator::global()->bounded(6) == 0) {
                QColor glow(30 + QRandomGenerator::global()->bounded(180),
                            200 + QRandomGenerator::global()->bounded(55),
                            40 + QRandomGenerator::global()->bounded(40));
                p.fillRect(x, y, pixel, pixel, glow);
            } else {
                p.fillRect(x, y, pixel, pixel, QColor(8, 40, 12));
            }
        }
    }

    p.end();
    return pm;
}

static QPixmap bgSolarFlare(int W, int H) {
    QPixmap pm(W, H);
    pm.fill(QColor(10, 3, 3));

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setRenderHint(QPainter::SmoothPixmapTransform, false);
    p.setPen(Qt::NoPen);

    for (int y = 0; y < H; y += 16) {
        for (int x = 0; x < W; x += 16) {
            if (QRandomGenerator::global()->bounded(6) == 0) {
                QColor flare(150 + QRandomGenerator::global()->bounded(80),
                             40 + QRandomGenerator::global()->bounded(60),
                             10 + QRandomGenerator::global()->bounded(40),
                             40 + QRandomGenerator::global()->bounded(60));
                p.fillRect(x, y, 16, 16, flare);
            }
        }
    }

    int bursts = 8 + QRandomGenerator::global()->bounded(8);
    for (int i = 0; i < bursts; i++) {
        int bx = QRandomGenerator::global()->bounded(W - 40);
        int by = QRandomGenerator::global()->bounded(H - 40);
        QColor core(255, 140, 20, 230);
        p.fillRect(bx, by, 6, 6, core);

        for (int r = 1; r <= 6; r++) {
            QColor glow(255 - r * 30, 140 - r * 15, 20 - r * 5, 180 - r * 25);
            p.fillRect(bx - r, by - r, 6 + r * 2, 6 + r * 2, glow);
        }
    }

    QPen scan(QColor(255, 150, 80, 10));
    p.setPen(scan);
    for (int y = 0; y < H; y += 3)
        p.drawLine(0, y, W, y);

    p.end();
    return pm;
}

static QPixmap generatePixelBackgroundForRound(int round, int W, int H)
{
    QPixmap pm(W, H);
    pm.fill(Qt::black);
    QPainter p(&pm);

    switch ((round - 1) % 6) {
    case 0: p.drawPixmap(0, 0, bgGalaxy(W, H));     break;
    case 1: p.drawPixmap(0, 0, bgNebulaStorm(W, H));break;
    case 2: p.drawPixmap(0, 0, bgNeon(W, H));      break;
    case 3: p.drawPixmap(0, 0, bgMoon(W, H));      break;
    case 4: p.drawPixmap(0, 0, bgToxic(W, H));     break;
    case 5: p.drawPixmap(0, 0, bgSolarFlare(W, H));  break;
    }

    p.end();
    return pm;
}

void Game::applyBackgroundForRound(int round) {
    const int W = Const::SCENE_W;
    const int H = Const::SCENE_H;

    QPixmap pm = generatePixelBackgroundForRound(round, W, H);
    {
        QPainter p(&pm);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(35, 35, 35, 30));
        const int gridSize = 6;
        const int pixelSize = 2;
        for (int y = 0; y < H; y += gridSize)
            for (int x = 0; x < W; x += gridSize)
                p.drawRect(x, y, pixelSize, pixelSize);
        p.end();
    }

    if (!m_bgItem) {
        m_bgItem = m_scene->addPixmap(pm);
        m_bgItem->setPos(0, 0);
        m_bgItem->setZValue(-2000);
        m_bgItem->setAcceptedMouseButtons(Qt::NoButton);
    } else {
        m_bgItem->setPixmap(pm);
    }
}

void Game::toggleGrid() {
    m_showGrid = !m_showGrid;
    if (m_rasterItem) {
        m_rasterItem->setVisible(m_showGrid);
    }
}

void Game::decreaseLife(int n, int playerIndex) {
    if (playerIndex == 1) {
        m_lives1 -= n;
        if (m_lives1 < 0) m_lives1 = 0;
    } else {
        m_lives2 -= n;
        if (m_lives2 < 0) m_lives2 = 0;
    }

    emit hudChanged(m_score1, m_hp1, m_lives1, m_is2PlayerMode ? m_score2 : -1, m_is2PlayerMode ? m_hp2 : -1, m_is2PlayerMode ? m_lives2 : -1);

    if (m_lives1 <= 0 && (!m_is2PlayerMode || m_lives2 <= 0)) {
        gameOver();
    }
}

void Game::setupRasterBackground() {
    if (m_rasterItem) {
        m_rasterItem->setVisible(m_showGrid);
        return;
    }

    const int W = Const::SCENE_W;
    const int H = Const::SCENE_H;

    QPixmap bg(W, H);
    bg.fill(Qt::transparent);

    QPainter p(&bg);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setRenderHint(QPainter::SmoothPixmapTransform, false);

    const int gridSize = 6;
    const int pixelSize = 2;

    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(QColor(35, 35, 35, 200)));

    for (int y = 0; y < H; y += gridSize)
        for (int x = 0; x < W; x += gridSize)
            p.drawRect(x, y, pixelSize, pixelSize);

    p.setPen(QColor(80, 80, 80, 40));
    for (int y = 0; y < H; y += 3)
        p.drawLine(0, y, W, y);

    p.end();

    m_rasterItem = m_scene->addPixmap(bg);
    m_rasterItem->setZValue(-1000);
    m_rasterItem->setPos(0, 0);
    m_rasterItem->setFlag(QGraphicsItem::ItemIsFocusable, false);
    m_rasterItem->setAcceptedMouseButtons(Qt::NoButton);
    m_rasterItem->setVisible(m_showGrid);
}

static const QVector<int> kRoundScore = { 0, 500, 1200, 2200, 3500, 5000, 7000, 9500, 12500 };

template<typename T>
static int countItemsOfType(QGraphicsScene* sc) {
    int c = 0;
    for (auto* it : sc->items())
        if (dynamic_cast<T*>(it)) ++c;
    return c;
}

Game* Game::s_inst = nullptr;

Game::Game(QObject* parent)
    : QObject(parent),
    m_scene(new QGraphicsScene(0, 0, Const::SCENE_W, Const::SCENE_H, this)),
    m_view(new QGraphicsView(m_scene))
{
    s_inst = this;
    m_view->setRenderHint(QPainter::Antialiasing, false);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform, false);

    m_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    m_view->setWindowTitle("Void Breaker (C++17 / Qt6)");
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFixedSize(Const::SCENE_W, Const::SCENE_H);

    m_view->installEventFilter(this);
    if (m_view->viewport()) {
        m_view->viewport()->installEventFilter(this);
    }

    setupRasterBackground();
    setupStarfield();

    m_hud = new HUD();
    m_scene->addItem(m_hud);
    connect(this, &Game::hudChanged, m_hud, &HUD::onHudChanged);

    m_view->show();

    connect(&m_tick, &QTimer::timeout, this, &Game::tick);
    m_tick.start(Const::TICK_MS);

    connect(&m_astSpawn, &QTimer::timeout, this, &Game::spawnAsteroid);
    connect(&m_mineSpawn, &QTimer::timeout, this, &Game::spawnMine);

    toMenu();
}

Game::~Game() {
    s_inst = nullptr;
}

Game* Game::instance() {
    return s_inst;
}

bool Game::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        auto* e = static_cast<QKeyEvent*>(event);
        bool isPress = (event->type() == QEvent::KeyPress);
        int k = e->key();

        if (m_state == State::Menu) {
            if (isPress) {
                if (k == Qt::Key_1 || k == Qt::Key_Up || k == Qt::Key_W) {
                    m_is2PlayerMode = false;
                    if (m_hud) m_hud->showMenuOverlay(false);
                } else if (k == Qt::Key_2 || k == Qt::Key_Down || k == Qt::Key_S) {
                    m_is2PlayerMode = true;
                    if (m_hud) m_hud->showMenuOverlay(true);
                } else if (k == Qt::Key_Space || k == Qt::Key_Return || k == Qt::Key_Z) {
                    start();
                }
            }
            return true;
        }

        if (m_state == State::Playing) {
            if (isPress && (k == Qt::Key_P || k == Qt::Key_Escape)) {
                togglePause();
                return true;
            }
            if (isPress && k == Qt::Key_G) {
                toggleGrid();
                return true;
            }

            if (m_player1 && m_lives1 > 0) {
                m_player1->handleInputKey(k, isPress);
            }
            if (m_is2PlayerMode && m_player2 && m_lives2 > 0) {
                m_player2->handleInputKey(k, isPress);
            }
            return true;
        }

        if (m_state == State::Paused) {
            if (isPress && (k == Qt::Key_P || k == Qt::Key_Escape)) {
                togglePause();
                return true;
            }
        }

        if (m_state == State::GameOver) {
            if (isPress && (k == Qt::Key_Space || k == Qt::Key_Return || k == Qt::Key_Z)) {
                toMenu();
                return true;
            }
        }
    }
    return QObject::eventFilter(watched, event);
}

void Game::setupStarfield() {
    for (int i = 0; i < 40; ++i) {
        auto layer = (i % 2 == 0) ? Star::Layer::Back : Star::Layer::Front;
        auto* s = new Star(layer);
        s->setPos(QRandomGenerator::global()->bounded(Const::SCENE_W),
                  QRandomGenerator::global()->bounded(Const::SCENE_H));
        m_scene->addItem(s);
    }
}

int Game::randomColumn() {
    return QRandomGenerator::global()->bounded(Const::GRID_W);
}

void Game::toMenu() {
    m_state = State::Menu;

    const auto itemsCopy = m_scene->items();
    for (auto* it : itemsCopy) {
        if (it == m_hud || it == m_rasterItem || it == m_bgItem) continue;
        if (dynamic_cast<Star*>(it)) continue;
        if (it->parentItem() != nullptr) continue;
        delete it;
    }

    m_player1 = nullptr;
    m_player2 = nullptr;

    if (m_hud) m_hud->showMenuOverlay(m_is2PlayerMode);

    m_astSpawn.stop();
    m_mineSpawn.stop();
    m_view->setTransform(QTransform());
    m_scene->invalidate(QRectF(), QGraphicsScene::BackgroundLayer | QGraphicsScene::ItemLayer);
    if (m_view->viewport()) m_view->viewport()->update();
}

void Game::start() {
    if (m_state == State::Playing) return;
    newRun();
}

void Game::newRun() {
    {
        const auto itemsCopy = m_scene->items();
        for (auto* it : itemsCopy) {
            if (it == m_hud || it == m_rasterItem || it == m_bgItem) continue;
            if (dynamic_cast<Star*>(it)) continue;
            if (it->parentItem() != nullptr) continue;
            delete it;
        }
        m_player1 = nullptr;
        m_player2 = nullptr;
        m_view->setTransform(QTransform());
        m_scene->invalidate(QRectF(), QGraphicsScene::BackgroundLayer | QGraphicsScene::ItemLayer);
        if (m_view->viewport()) m_view->viewport()->update();
    }

    m_state = State::Playing;
    m_score1 = 0;
    m_score2 = 0;
    m_hp1    = Const::PLAYER_MAX_HP;
    m_lives1 = Const::PLAYER_LIVES;
    m_hp2    = Const::PLAYER_MAX_HP;
    m_lives2 = Const::PLAYER_LIVES;
    m_round  = 1;

    applyBackgroundForRound(m_round);
    m_maxAsteroids = 5;
    m_maxMines = 2;

    emit hudChanged(m_score1, m_hp1, m_lives1, m_is2PlayerMode ? m_score2 : -1, m_is2PlayerMode ? m_hp2 : -1, m_is2PlayerMode ? m_lives2 : -1);
    if (m_hud) { m_hud->hideOverlays(); m_hud->showRoundBanner(m_round); }

    if (!m_is2PlayerMode) {
        // Single Player Mode
        m_player1 = new Player(1);
        m_player1->setPos(Const::SCENE_W / 2.0, Const::PLAYER_Y);
        m_scene->addItem(m_player1);
    } else {
        // 1 VS 1 PvP Battle Mode
        m_player1 = new Player(1);
        m_player1->setPos(Const::SCENE_W * 0.35, Const::PLAYER_Y);
        m_scene->addItem(m_player1);

        m_player2 = new Player(2);
        m_player2->setPos(Const::SCENE_W * 0.65, Const::PLAYER_Y);
        m_scene->addItem(m_player2);
    }

    m_astSpawnMs  = Const::INIT_AST_SPAWN_MS;
    m_mineSpawnMs = Const::INIT_MINE_SPAWN_MS;
    m_diffAccumMs = 0;
    m_astSpawn.start(m_astSpawnMs);
    m_mineSpawn.start(m_mineSpawnMs);
}

void Game::togglePause() {
    if (m_state == State::Playing) {
        m_state = State::Paused;
        if (m_hud) m_hud->showPauseOverlay();
        m_astSpawn.stop();
        m_mineSpawn.stop();
    } else if (m_state == State::Paused) {
        m_state = State::Playing;
        if (m_hud) m_hud->hideOverlays();
        m_astSpawn.start(m_astSpawnMs);
        m_mineSpawn.start(m_mineSpawnMs);
    }
}

void Game::addScore(int s, int playerIndex) {
    if (playerIndex == 1) {
        m_score1 += s;
    } else {
        m_score2 += s;
    }
    emit hudChanged(m_score1, m_hp1, m_lives1, m_is2PlayerMode ? m_score2 : -1, m_is2PlayerMode ? m_hp2 : -1, m_is2PlayerMode ? m_lives2 : -1);
}

void Game::healPlayer(int playerIndex, int amount) {
    if (playerIndex == 1 && m_lives1 > 0) {
        m_hp1 = std::min(Const::PLAYER_MAX_HP, m_hp1 + amount);
    } else if (playerIndex == 2 && m_lives2 > 0) {
        m_hp2 = std::min(Const::PLAYER_MAX_HP, m_hp2 + amount);
    }
    emit hudChanged(m_score1, m_hp1, m_lives1, m_is2PlayerMode ? m_score2 : -1, m_is2PlayerMode ? m_hp2 : -1, m_is2PlayerMode ? m_lives2 : -1);
}

void Game::spawnPowerUpAt(const QPointF& pos) {
    if (QRandomGenerator::global()->bounded(100) < 25) {
        int r = QRandomGenerator::global()->bounded(3);
        PowerUp::Type t = static_cast<PowerUp::Type>(r);
        auto* pw = new PowerUp(t);
        pw->setPos(pos);
        m_scene->addItem(pw);
    }
}

void Game::playerHit(int dmg, Player* p) {
    if (m_state != State::Playing) return;

    if (p && p->hasShield()) {
        p->popShield();
        applyShake(Const::SHAKE_MS / 2, Const::SHAKE_INTENSITY * 0.7);
        return;
    }

    int idx = (p && p->playerIndex() == 2) ? 2 : 1;

    if (idx == 1) {
        m_hp1 -= dmg;
        if (m_hp1 < 0) m_hp1 = 0;
        if (m_hp1 == 0) {
            m_lives1 -= 1;
            if (m_lives1 > 0) respawnPlayer(1);
        }
    } else {
        m_hp2 -= dmg;
        if (m_hp2 < 0) m_hp2 = 0;
        if (m_hp2 == 0) {
            m_lives2 -= 1;
            if (m_lives2 > 0) respawnPlayer(2);
        }
    }

    emit hudChanged(m_score1, m_hp1, m_lives1, m_is2PlayerMode ? m_score2 : -1, m_is2PlayerMode ? m_hp2 : -1, m_is2PlayerMode ? m_lives2 : -1);
    applyShake(Const::SHAKE_MS, Const::SHAKE_INTENSITY);

    if (m_lives1 <= 0 || (m_is2PlayerMode && m_lives2 <= 0)) {
        gameOver();
    }
}

void Game::respawnPlayer(int playerIndex) {
    if (playerIndex == 1) {
        m_hp1 = Const::PLAYER_MAX_HP;
        if (!m_player1) {
            m_player1 = new Player(1);
            m_scene->addItem(m_player1);
        }
        m_player1->setPos(m_is2PlayerMode ? Const::SCENE_W * 0.35 : Const::SCENE_W / 2.0, Const::PLAYER_Y);
        m_player1->setInvuln(Const::PLAYER_INVULN_MS);
    } else {
        m_hp2 = Const::PLAYER_MAX_HP;
        if (!m_player2) {
            m_player2 = new Player(2);
            m_scene->addItem(m_player2);
        }
        m_player2->setPos(Const::SCENE_W * 0.65, Const::PLAYER_Y);
        m_player2->setInvuln(Const::PLAYER_INVULN_MS);
    }
    emit hudChanged(m_score1, m_hp1, m_lives1, m_is2PlayerMode ? m_score2 : -1, m_is2PlayerMode ? m_hp2 : -1, m_is2PlayerMode ? m_lives2 : -1);
}

void Game::playerKilledByMine() {
    if (m_state != State::Playing) return;
    m_hp1 = 0;
    m_hp2 = 0;
    emit hudChanged(m_score1, m_hp1, m_lives1, m_is2PlayerMode ? m_score2 : -1, m_is2PlayerMode ? m_hp2 : -1, m_is2PlayerMode ? m_lives2 : -1);
    applyShake(Const::SHAKE_MS * 2, Const::SHAKE_INTENSITY * 1.3);
    gameOver();
}

void Game::applyShake(int ms, qreal intensity) {
    m_shakeRemainMs = ms;
    m_shakeIntensity = intensity;
}

void Game::gameOver() {
    m_state = State::GameOver;

    int winnerIndex = 0;
    if (m_is2PlayerMode) {
        if (m_lives1 > 0 && m_lives2 <= 0) winnerIndex = 1;
        else if (m_lives2 > 0 && m_lives1 <= 0) winnerIndex = 2;
        else winnerIndex = (m_score1 >= m_score2) ? 1 : 2;
    }

    emit hudChanged(m_score1, m_hp1, m_lives1, m_is2PlayerMode ? m_score2 : -1, m_is2PlayerMode ? m_hp2 : -1, m_is2PlayerMode ? m_lives2 : -1);
    m_astSpawn.stop();
    m_mineSpawn.stop();

    if (m_hud) m_hud->showGameOverOverlay(winnerIndex);
}

void Game::tick() {
    m_scene->advance();

    if (m_state == State::Playing) {
        updateDifficulty();
        evaluateRound();

        for (auto* item : m_scene->items()) {
            if (auto* pw = dynamic_cast<PowerUp*>(item)) {
                if (m_player1 && m_lives1 > 0 && m_player1->collidesWithItem(pw)) {
                    m_player1->applyPowerUp(pw->powerUpType());
                    pw->deleteLater();
                } else if (m_player2 && m_lives2 > 0 && m_player2->collidesWithItem(pw)) {
                    m_player2->applyPowerUp(pw->powerUpType());
                    pw->deleteLater();
                }
            }
        }
    }

    updateShake();
}

void Game::spawnAsteroid() {
    if (m_state != State::Playing) return;
    if (countItemsOfType<Asteroid>(m_scene) >= m_maxAsteroids) return;

    const int col = randomColumn();
    auto* a = new Asteroid();
    a->setPos(col * Const::TILE + Const::TILE / 2, -Const::TILE);
    m_scene->addItem(a);
}

void Game::spawnMine() {
    if (m_state != State::Playing) return;
    if (countItemsOfType<Mine>(m_scene) >= m_maxMines) return;

    const int col = randomColumn();
    auto* m = new Mine();
    m->setPos(col * Const::TILE + Const::TILE / 2, -Const::TILE);
    m_scene->addItem(m);
}

void Game::updateDifficulty() {
    m_diffAccumMs += Const::TICK_MS;
    if (m_diffAccumMs >= Const::DIFF_STEP_PERIOD_MS) {
        m_diffAccumMs = 0;
        if (m_astSpawnMs > Const::MIN_AST_SPAWN_MS) {
            m_astSpawnMs -= Const::DIFF_STEP_MS_AST;
            if (m_astSpawnMs < Const::MIN_AST_SPAWN_MS) m_astSpawnMs = Const::MIN_AST_SPAWN_MS;
        }
        if (m_mineSpawnMs > Const::MIN_MINE_SPAWN_MS) {
            m_mineSpawnMs -= Const::DIFF_STEP_MS_MINE;
            if (m_mineSpawnMs < Const::MIN_MINE_SPAWN_MS) m_mineSpawnMs = Const::MIN_MINE_SPAWN_MS;
        }
        m_astSpawn.start(m_astSpawnMs);
        m_mineSpawn.start(m_mineSpawnMs);
    }
}

void Game::evaluateRound() {
    int totalScore = m_score1 + std::max(0, m_score2);
    int desired = 1;
    for (int i = 0; i < kRoundScore.size(); ++i)
        if (totalScore >= kRoundScore[i]) desired = i + 1;
    if (desired == m_round) return;

    if (desired > 12) desired = 12;

    for (int r = m_round + 1; r <= desired; ++r) onRoundUp(r);
    m_round = desired;
}

void Game::onRoundUp(int newRound) {
    applyBackgroundForRound(newRound);

    if (m_hud) m_hud->showRoundBanner(newRound);

    m_astSpawnMs = std::max(Const::MIN_AST_SPAWN_MS, m_astSpawnMs - 80);
    m_mineSpawnMs = std::max(Const::MIN_MINE_SPAWN_MS, m_mineSpawnMs - 60);
    m_astSpawn.start(m_astSpawnMs);
    m_mineSpawn.start(m_mineSpawnMs);

    m_maxAsteroids = std::min(10, m_maxAsteroids + 1);
    if (newRound % 2 == 0) m_maxMines = std::min(5, m_maxMines + 1);
}

void Game::updateShake() {
    if (m_shakeRemainMs <= 0) {
        m_view->setTransform(QTransform());
        return;
    }
    m_shakeRemainMs -= Const::TICK_MS;

    const qreal rdx = QRandomGenerator::global()->generateDouble() * 2.0 - 1.0;
    const qreal rdy = QRandomGenerator::global()->generateDouble() * 2.0 - 1.0;
    m_view->setTransform(QTransform::fromTranslate(rdx * m_shakeIntensity,
                                                   rdy * m_shakeIntensity));
}

qreal Game::difficultySpeedMul() const {
    qreal mul = 1.0 + ((m_score1 + std::max(0, m_score2)) / 300.0) * 0.04 + std::max(0, m_round - 1) * 0.07;
    if (mul > 1.55) mul = 1.55;
    return mul;
}

int Game::leakDamage() const {
    return Const::AST_LEAK_DMG_DEFAULT + std::max(0, m_round - 1) * 3;
}
