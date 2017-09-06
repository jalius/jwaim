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

//#include "esp.h"
#include "hack.h"

class qWindow : public QWidget
{
    Q_OBJECT
public:
    explicit qWindow(QWidget *parent = 0);
    std::array<EntityToScreen,64> entitiesToScreen;
    Vector rcsCross;
private:
     QPushButton *m_button;
protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:
void callback();
};

#endif // WINDOW_H
