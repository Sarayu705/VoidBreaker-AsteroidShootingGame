#pragma once
#include <QGraphicsObject>
#include <QTimer>
#include <QImage>
#include <QColor>
#include <QString>

class QGraphicsPixmapItem;
class QGraphicsRectItem;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class HUD : public QGraphicsObject {
    Q_OBJECT
public:
    HUD();

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    // Mode Selection Menu & Overlays
    void showMenuOverlay(bool is2Player = false);
    void showPauseOverlay();
    void showGameOverOverlay(int winnerIndex = 0);
    void hideOverlays();
    void centerPixmapItem(QGraphicsPixmapItem* item, qreal y);

public slots:
    void onHudChanged(int score1, int hp1, int lives1, int score2 = -1, int hp2 = -1, int lives2 = -1);
    void showRoundBanner(int round, const QString& quote = "");

public:
    static QImage renderPixelText(const QString &text, int fontSize, const QColor &color);

private:
    int m_score1 {0};
    int m_hp1    {100};
    int m_lives1 {3};
    int m_score2 {-1};
    int m_hp2    {-1}; // -1 if single player
    int m_lives2 {-1};

    // Dashboard overlay items
    QGraphicsPixmapItem* m_menuTitle    = nullptr;
    QGraphicsPixmapItem* m_menuOption1  = nullptr;
    QGraphicsPixmapItem* m_menuOption2  = nullptr;
    QGraphicsPixmapItem* m_menuHint     = nullptr;

    QGraphicsPixmapItem* m_pauseText    = nullptr;
    QGraphicsPixmapItem* m_gameOverText = nullptr;

    // Round banner pieces
    QGraphicsRectItem*   m_roundBack    = nullptr;
    QGraphicsPixmapItem* m_roundTitle   = nullptr;
    QGraphicsPixmapItem* m_roundQuote   = nullptr;
    QTimer               m_roundFade;
    int                  m_roundFadeSteps = 0;

    void ensureRoundItems();
    void layoutRoundItems();
};
