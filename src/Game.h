#pragma once
#include <QObject>
#include <QTimer>
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QEvent>
#include "PowerUp.h"

class QGraphicsScene;
class QGraphicsView;
class Player;
class HUD;
class Star;

class Game : public QObject {
    Q_OBJECT
public:
    enum class State { Menu, Playing, Paused, GameOver };

    explicit Game(QObject* parent=nullptr);
    ~Game();

    static Game* instance();
    void setupRasterBackground();
    void decreaseLife(int n, int playerIndex = 1);
    void start();
    void toMenu();
    void togglePause();
    void applyBackgroundForRound(int round);

    // Central Event Filter for 100% reliable 1P / 2P keyboard input
    bool eventFilter(QObject* watched, QEvent* event) override;

    // Scoring, health, powerups
    void addScore(int s, int playerIndex = 1);
    void playerHit(int dmg, Player* p = nullptr);
    void playerKilledByMine();
    void healPlayer(int playerIndex, int amount);
    void spawnPowerUpAt(const QPointF& pos);
    void respawnPlayer(int playerIndex = 1);
    
    bool m_showGrid = true;
    QGraphicsPixmapItem* m_rasterItem = nullptr;
    void toggleGrid();

    // Difficulty helpers
    qreal difficultySpeedMul() const;
    int   leakDamage() const;

signals:
    void hudChanged(int score1, int hp1, int lives1, int score2 = -1, int hp2 = -1, int lives2 = -1);

private slots:
    void tick();
    void spawnAsteroid();
    void spawnMine();

private:
    static Game* s_inst;

    // World
    QGraphicsScene* m_scene = nullptr;
    QGraphicsView*  m_view  = nullptr;
    Player* m_player1 = nullptr;
    Player* m_player2 = nullptr;
    bool    m_is2PlayerMode = false; // Mode selected on Start Dashboard Menu

    HUD*    m_hud    = nullptr;
    QGraphicsPixmapItem* m_bgItem = nullptr;
    void drawPixelBackground(QPainter &p);
    int currentTheme = 0;

    // Timers
    QTimer m_tick;
    QTimer m_astSpawn;
    QTimer m_mineSpawn;

    // State
    State m_state = State::Menu;
    int m_score1 = 0;
    int m_score2 = 0;
    
    int m_hp1 = 100;
    int m_lives1 = 3;

    int m_hp2 = 100;
    int m_lives2 = 3;

    // Difficulty / pacing
    int m_round = 1;
    int m_astSpawnMs = 0;
    int m_mineSpawnMs = 0;
    int m_diffAccumMs = 0;
    int m_maxAsteroids = 5;
    int m_maxMines = 2;

    // Shake
    int   m_shakeRemainMs = 0;
    qreal m_shakeIntensity = 0;

    // Setup / utils
    void setupStarfield();
    int  randomColumn();

    void newRun();
    void updateDifficulty();
    void evaluateRound();
    void onRoundUp(int newRound);

    void updateShake();
    void applyShake(int ms, qreal intensity);

    void gameOver();
};
