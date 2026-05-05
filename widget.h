#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPointF>
#include <vector>
#include <list>
#include <QPixmap>
#include <QRectF>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "Enemy.h"
#include "Tower.h"
#include "CostSystem.h"
#include "Map.h"
#include "Level.h"
#include "CannonTower.h"
#include "LightTower.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private slots:
    void Updategamelogic();

private:
    enum GameState { MainMenu, Playing, GameOver_Win, GameOver_Lose };
    enum BuildState { None, PreparingBuild, PreparingBuildCannon, PreparingBuildLight };

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void resetGame(int level = 1);
    void drawButton(QPainter &painter, const QRectF &rect, const QString &text);
    void paintMainMenu(QPainter &painter);
    void paintPlaying(QPainter &painter);
    void paintGameOverWin(QPainter &painter);
    void paintGameOverLose(QPainter &painter);

    Ui::Widget *ui;
    QTimer *Gametimer;

    GameState m_gameState = MainMenu;

    std::list<Enemy*> EnemyList;
    std::list<Tower*> TowerList;
    std::list<FakeBullet> BulletList;
    void bullet_move();

    QPixmap m_grassTexture;
    QPixmap m_wallTexture;
    QPixmap m_spawnTexture;
    QPixmap m_homeTexture;
    QPixmap m_towerIconTexture;
    QPixmap m_cannonIconTexture;
    QPixmap m_lightIconTexture;
    QPixmap m_menuBackground;

    QRectF m_cannonShopButton;
    QRectF m_lightShopButton;

    BuildState m_buildState = None;
    QRectF m_towerShopButton;
    const double m_shopButtonSize = 50.0;
    QPointF m_mousePos;

    int spawnCounter = 0;
    int spawnedCount = 0;

    CostSystem costsystem;
    Map m_map;
    Level* current_level = nullptr;

    bool islose = false;
    bool iswin  = false;
    int m_initHomehp = 0;

    QRectF m_btnStart;
    QRectF m_btnExit;
    QRectF m_btnRetry;
    QRectF m_btnNextLevel;
    QRectF m_btnMenuFromWin;
    QRectF m_btnMenuFromLose;

    QMediaPlayer* m_bgmPlayer;
    QAudioOutput* m_audioOutput;
};

#endif // WIDGET_H
