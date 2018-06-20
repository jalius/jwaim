#ifndef WINDOW_H
#define WINDOW_H
#include <sstream>
#include <qtextstream.h>
#include <QWidget>
#include <QDebug>
#include <QDesktopWidget>
#include <QPushButton>
#include <QPainter>
#include <QGuiApplication>
#include <QScreen>
#include <QTimer>
#include <chrono>

//#include "esp.h"
#include "hack.h"

class qWindow : public QWidget
{
    Q_OBJECT
public:
    explicit qWindow(QWidget *parent = 0);
    void drawESPBoxes(std::array<EntityToScreen,64> &entitiesToScreen, QPainter &painter,QPen &pen, QFont &font);
    void drawToggleMenu(QPainter &painter, QPen &pen, QFont &font);
    void drawHitmarker(float width, QPainter &painter, QPen &pen);
    std::array<EntityToScreen,64> entitiesToScreen;
    Vector rcsCross;
private:
     QPushButton *m_button;
protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event) override;
signals:

public slots:
void callback();
};

#endif // WINDOW_H
