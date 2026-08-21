#include "HUD.h"
#include "Constants.h"

#include <QPainter>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QFont>
#include <QBrush>
#include <QFontDatabase>
#include <QGraphicsPixmapItem>

QImage HUD::renderPixelText(const QString &text, int fontSize, const QColor &color) {
    QFont f("Courier", fontSize);
    f.setStyleStrategy(QFont::NoAntialias);  // disable smoothing
    QFontMetrics fm(f);
    int textW = std::max(1, fm.horizontalAdvance(text));
    int textH = std::max(1, fm.height());

    QImage img(textW + 8, textH + 4, QImage::Format_ARGB32);
    img.fill(Qt::transparent);

    QPainter p(&img);
    p.setFont(f);
    p.setPen(color);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setRenderHint(QPainter::TextAntialiasing, false);
    p.drawText(4, fm.ascent() + 2, text);
    p.end();

    return img.scaled(img.width() * 2, img.height() * 2, Qt::KeepAspectRatio, Qt::FastTransformation);
}

HUD::HUD() {
    setZValue(1000);
    setPos(0,0);

    // Dashboard overlay items
    m_menuTitle   = new QGraphicsPixmapItem(this);
    m_menuOption1 = new QGraphicsPixmapItem(this);
    m_menuOption2 = new QGraphicsPixmapItem(this);
    m_menuHint    = new QGraphicsPixmapItem(this);

    m_pauseText    = new QGraphicsPixmapItem(this);
    m_gameOverText = new QGraphicsPixmapItem(this);

    QPixmap pixmap2 = QPixmap::fromImage(renderPixelText("Paused — Press P to Resume", 10, Qt::yellow));
    m_pauseText->setPixmap(pixmap2);
    m_pauseText->setPos((Const::SCENE_W - pixmap2.width())/2, Const::SCENE_H*0.45);

    hideOverlays();

    connect(&m_roundFade, &QTimer::timeout, [this](){
        if (m_roundFadeSteps <= 0) {
            if (m_roundBack)  m_roundBack->setVisible(false);
            if (m_roundTitle) m_roundTitle->setVisible(false);
            if (m_roundQuote) m_roundQuote->setVisible(false);
            m_roundFade.stop();
            return;
        }
        qreal op = 1.0;
        if (m_roundFadeSteps < 20) {
            op = m_roundFadeSteps / 20.0;
        }
        if (m_roundBack)  m_roundBack->setOpacity(op);
        if (m_roundTitle) m_roundTitle->setOpacity(op);
        if (m_roundQuote) m_roundQuote->setOpacity(op);
        --m_roundFadeSteps;
    });
}

void HUD::centerPixmapItem(QGraphicsPixmapItem* item, qreal y)
{
    if (!item) return;

    QRectF b = item->pixmap().rect();
    item->setPos(
        (Const::SCENE_W - b.width()) / 2.0,
        y - b.height() / 2.0
    );
}

QRectF HUD::boundingRect() const {
    return QRectF(0,0, Const::SCENE_W, Const::SCENE_H);
}

void HUD::paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) {
    p->setPen(Qt::NoPen);

    if (m_lives2 < 0) {
        // --- Single Player HUD ---
        p->setBrush(QColor(40,40,40));
        p->drawRect(12, 12, 140, 12);

        p->setBrush(QColor(0, 200, 120));
        int w = int(138 * (std::clamp(m_hp1, 0, 100) / 100.0));
        p->drawRect(13, 13, w, 10);

        p->drawImage(165, 10, renderPixelText(QString("P1 HP"), 7, Qt::cyan));
        p->drawImage(240, 10, renderPixelText(QString("Score: %1").arg(m_score1), 8, Qt::white));
        p->drawImage(420, 10, renderPixelText(QString("Lives: %1").arg(m_lives1), 8, Qt::white));
    } else {
        // --- 1 VS 1 PvP Battle HUD ---
        // Player 1 (Blue Falcon) HP & Score (Left)
        p->setBrush(QColor(40,40,40));
        p->drawRect(12, 12, 110, 10);
        p->setBrush(QColor(0, 190, 255)); // Cyan
        int w1 = int(108 * (std::clamp(m_hp1, 0, 100) / 100.0));
        p->drawRect(13, 13, w1, 8);
        p->drawImage(128, 8, renderPixelText(QString("P1 S:%1 L:%2").arg(m_score1).arg(m_lives1), 7, Qt::cyan));

        // Player 2 (Red Phoenix) HP & Score (Right)
        int p2X = Const::SCENE_W - 122;
        p->setBrush(QColor(40,40,40));
        p->drawRect(p2X, 12, 110, 10);
        p->setBrush(QColor(255, 80, 50)); // Red/Orange
        int w2 = int(108 * (std::clamp(m_lives2 > 0 ? m_hp2 : 0, 0, 100) / 100.0));
        p->drawRect(p2X + 1, 13, w2, 8);
        p->drawImage(p2X - 115, 8, renderPixelText(QString("P2 S:%1 L:%2").arg(m_score2).arg(m_lives2), 7, QColor(255,180,50)));

        // 1 VS 1 Center Title
        p->drawImage(Const::SCENE_W / 2 - 40, 8, renderPixelText(QString("1 VS 1"), 8, Qt::yellow));
    }
}

void HUD::onHudChanged(int score1, int hp1, int lives1, int score2, int hp2, int lives2) {
    m_score1 = score1;
    m_hp1    = hp1;
    m_lives1 = lives1;
    m_score2 = score2;
    m_hp2    = hp2;
    m_lives2 = lives2;
    update();
}

void HUD::showMenuOverlay(bool is2Player) {
    hideOverlays();

    QPixmap titlePm = QPixmap::fromImage(renderPixelText("VOID BREAKER", 16, Qt::yellow));
    m_menuTitle->setPixmap(titlePm);
    centerPixmapItem(m_menuTitle, Const::SCENE_H * 0.28);

    QColor col1 = (!is2Player) ? Qt::green : QColor(160, 160, 160);
    QString opt1Str = (!is2Player) ? "> [1] SINGLE PLAYER  (WASD + Space)" : "  [1] SINGLE PLAYER  (WASD + Space)";
    m_menuOption1->setPixmap(QPixmap::fromImage(renderPixelText(opt1Str, 9, col1)));
    centerPixmapItem(m_menuOption1, Const::SCENE_H * 0.44);

    QColor col2 = (is2Player) ? Qt::green : QColor(160, 160, 160);
    QString opt2Str = (is2Player) ? "> [2] 1 VS 1 BATTLE   (P1: WASD | P2: Arrows)" : "  [2] 1 VS 1 BATTLE   (P1: WASD | P2: Arrows)";
    m_menuOption2->setPixmap(QPixmap::fromImage(renderPixelText(opt2Str, 9, col2)));
    centerPixmapItem(m_menuOption2, Const::SCENE_H * 0.52);

    m_menuHint->setPixmap(QPixmap::fromImage(renderPixelText("Press 1/2 or UP/DOWN to Choose | SPACE to Launch", 7, Qt::white)));
    centerPixmapItem(m_menuHint, Const::SCENE_H * 0.68);

    m_menuTitle->setVisible(true);
    m_menuOption1->setVisible(true);
    m_menuOption2->setVisible(true);
    m_menuHint->setVisible(true);
}

void HUD::showPauseOverlay() {
    hideOverlays();
    centerPixmapItem(m_pauseText, Const::SCENE_H * 0.45);
    m_pauseText->setVisible(true);
}

void HUD::showGameOverOverlay(int winnerIndex) {
    hideOverlays();
    QString winMsg;
    QColor winCol;

    if (winnerIndex == 1) {
        winMsg = "PLAYER 1 WINS! — Press SPACE";
        winCol = Qt::cyan;
    } else if (winnerIndex == 2) {
        winMsg = "PLAYER 2 WINS! — Press SPACE";
        winCol = QColor(255, 180, 50);
    } else {
        winMsg = "GAME OVER — Press SPACE";
        winCol = Qt::red;
    }

    QPixmap pixmap = QPixmap::fromImage(renderPixelText(winMsg, 10, winCol));
    m_gameOverText->setPixmap(pixmap);
    centerPixmapItem(m_gameOverText, Const::SCENE_H * 0.45);
    m_gameOverText->setVisible(true);
}

void HUD::hideOverlays() {
    if (m_menuTitle)   m_menuTitle->setVisible(false);
    if (m_menuOption1) m_menuOption1->setVisible(false);
    if (m_menuOption2) m_menuOption2->setVisible(false);
    if (m_menuHint)    m_menuHint->setVisible(false);
    if (m_pauseText)    m_pauseText->setVisible(false);
    if (m_gameOverText) m_gameOverText->setVisible(false);
}

void HUD::ensureRoundItems() {
    if (!m_roundBack) {
        m_roundBack = new QGraphicsRectItem(0,0, 1,1, this);
        m_roundBack->setZValue(1001);
        m_roundBack->setBrush(QColor(0,0,0,180));
        m_roundBack->setPen(QPen(QColor(255,255,255,60), 2));
    }

    if (!m_roundTitle) {
        m_roundTitle = new QGraphicsPixmapItem(this);
        m_roundTitle->setZValue(1002);
    }
    if (!m_roundQuote) {
        m_roundQuote = new QGraphicsPixmapItem(this);
        m_roundQuote->setZValue(1002);
    }
}

void HUD::layoutRoundItems() {
    if (!m_roundBack || !m_roundTitle)
        return;

    const qreal padX = 30.0;
    const qreal padY = 18.0;

    QPixmap titlePm = m_roundTitle->pixmap();

    const qreal tw = titlePm.width();
    const qreal th = titlePm.height();

    const qreal w = tw + padX * 2.0;
    const qreal h = th + padY * 2.0;

    const qreal x = (Const::SCENE_W - w) / 2.0;
    const qreal y = (Const::SCENE_H - h) / 2.0;

    m_roundBack->setRect(x, y, w, h);

    const qreal tx = x + (w - tw) / 2.0;
    const qreal ty = y + (h - th) / 2.0;

    m_roundTitle->setPos(tx, ty);
}

void HUD::showRoundBanner(int round, const QString& /*quote*/) {
    ensureRoundItems();

    m_roundTitle->setPixmap(
        QPixmap::fromImage(renderPixelText(QString("ROUND %1").arg(round), 14, Qt::white))
    );

    if (m_roundQuote) {
        m_roundQuote->setVisible(false);
    }

    m_roundBack->setOpacity(1.0);
    m_roundTitle->setOpacity(1.0);

    layoutRoundItems();

    m_roundBack->setVisible(true);
    m_roundTitle->setVisible(true);

    m_roundFadeSteps = 60;
    m_roundFade.start(60);
}
