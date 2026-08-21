#include <QApplication>
#include "Game.h"
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    int fontId = QFontDatabase::addApplicationFont(":/fonts/PressStart2P.ttf");
    if (fontId < 0) {
        qDebug() << "Failed to load custom NES font!";
    } else {
        qDebug() << "NES Font Loaded!";
    }
    
    Game g;
    // Launch directly to Dashboard Start Menu (toMenu is called in Game constructor)
    
    return app.exec();
}
