#include <QX11Info>
#include "window.h"
#include "monitor.h"

int i = 0;
qWindow::qWindow(QWidget *parent) : QWidget(parent)
{ 
    QSize size = qApp->screens()[0]->size();
    settings::window::wind_height = size.height();
    settings::window::wind_width = size.width();
    setGeometry(settings::window::wind_x, settings::window::wind_y, settings::window::wind_width, settings::window::wind_height);
    setAutoFillBackground(false);
    //setWindowFlags(Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput | Qt::X11BypassWindowManagerHint);
    //m_button = new QPushButton("gang shit", this);
    //m_button->setGeometry(0,1,200,100);
    QTimer::singleShot(1, this, SLOT(callback())); //or call callback() directly here
    //m_button->setWindowOpacity(qreal(100)/100);
}

void qWindow::showEvent(QShowEvent *event)
{

    QWidget::showEvent(event);
    if (QX11Info::isPlatformX11()) {
        if (!QX11Info::isCompositingManagerRunning()) {
            qInfo() << "No compositing manager found.  Disabling overlay.";
            QTimer::singleShot(0, this, &QWidget::hide);
        }
    }
}

void qWindow::paintEvent(QPaintEvent *)
{
    const QColor redFalse(255, 050, 05);
    const QColor greenTrue(050, 255, 05);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(105, 225, 225, 0));
    //painter.drawRect(settings::window::wind_x, settings::window::wind_y, settings::window::wind_width, settings::window::wind_height); sorta forgot why i have this here

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(QColor(255, 255, 255, 255));
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    QFont font("DroidSansBold", 7, QFont::Bold, false);
    painter.setFont(font);
    /*if(i<width())
    {
        i++;
    }
    else
    {
        i = 0;
    }*/
    painter.drawText(settings::window::wind_width * .2, 15, "JWAim");

    font.setPointSize(6);
    painter.setFont(font);

    pen.setColor(h.ShouldRadarHack ? greenTrue : redFalse);
    painter.setPen(pen);
    painter.drawText(settings::window::wind_width * .2, 25, (h.ShouldRadarHackToggleKey + " RadarHack: " + helper::AtomicBoolToString(&h.ShouldRadarHack)).c_str());

    pen.setColor(h.ShouldGlow ? greenTrue : redFalse);
    painter.setPen(pen);
    painter.drawText(settings::window::wind_width * .2, 35, (h.ShouldGlowToggleKey + " Glow: " + helper::AtomicBoolToString(&h.ShouldGlow)).c_str());

    pen.setColor(h.ShouldESP ? greenTrue : redFalse);
    painter.setPen(pen);
    painter.drawText(settings::window::wind_width * .2, 45, (h.ShouldESPToggleKey + " ESP: " + helper::AtomicBoolToString(&h.ShouldESP)).c_str());

    pen.setColor(h.ShouldBhop ? greenTrue : redFalse);
    painter.setPen(pen);
    painter.drawText(settings::window::wind_width * .2, 55, (h.ShouldBhopToggleKey + " BHop: " + helper::AtomicBoolToString(&h.ShouldBhop)).c_str());

    pen.setColor(h.ShouldAimAssist ? greenTrue : redFalse);
    painter.setPen(pen);
    painter.drawText(settings::window::wind_width * .2, 65, (h.ShouldAimAssistToggleKey + " AimAssist: " + helper::AtomicBoolToString(&h.ShouldAimAssist)).c_str());

    pen.setColor(h.ShouldRage ? greenTrue : redFalse);
    painter.setPen(pen);
    painter.drawText(settings::window::wind_width * .2, 75, (h.ShouldRageToggleKey + " Rage: " + helper::AtomicBoolToString(&h.ShouldRage)).c_str());

    pen.setColor(h.ShouldNoFlash ? greenTrue : redFalse);
    painter.setPen(pen);
    painter.drawText(settings::window::wind_width * .2, 85, (h.ShouldNoFlashToggleKey + " No Flash: " + helper::AtomicBoolToString(&h.ShouldNoFlash)).c_str());

    pen.setColor(h.ShouldRCS ? greenTrue : redFalse);
    painter.setPen(pen);
    painter.drawText(settings::window::wind_width * .2, 95, (h.ShouldRCSToggleKey + " RCS: " + helper::AtomicBoolToString(&h.ShouldRCS)).c_str());
    if (h.IsConnected())
    {
        static int waitForABit_it = -1;
        if (h.drawHitmarker)
        {
            if (h.totalHitsIncreased())
            {
                qWindow::drawHitmarker(settings::misc::hitmarker_width, painter, pen);
                waitForABit_it = 0;
            }
            else if (waitForABit_it < settings::misc::hitmarker_time && waitForABit_it != -1)
            {
                qWindow::drawHitmarker(settings::misc::hitmarker_width, painter, pen);                
                waitForABit_it++;
            }
            else
            {
                waitForABit_it = -1;
            }
        }
        std::vector<std::string> spectators;
        spectators.reserve(64);
        for (int i = 0; i < 64; i++)
        {
            if (entitiesToScreen[i].spectatingMe)
            {
                spectators.push_back(entitiesToScreen[i].name);
                //cout<<"spectator added: "<<entitiesToScreen[i].name<<endl;
            }
        }
        if (h.ShouldESP)
        {
            qWindow::drawESPBoxes(entitiesToScreen, painter, pen, font);
        }
        if (!(rcsCross.x == 0 && rcsCross.y == 0)&&h.drawrcsCrosshair)
        {
            pen.setColor(QColor(h.Colors()[20], h.Colors()[21], h.Colors()[22], h.Colors()[23]));
            pen.setWidthF(1.8);
            painter.setPen(pen);
            painter.drawLine(rcsCross.x + 5, rcsCross.y, rcsCross.x - 5, rcsCross.y);
            painter.drawLine(rcsCross.x, rcsCross.y + 5, rcsCross.x, rcsCross.y - 5);
        }
        else if (h.staticCrosshair)
        {
            pen.setColor(QColor(h.Colors()[20], h.Colors()[21], h.Colors()[22], h.Colors()[23]));
            pen.setWidthF(1.8);
            painter.setPen(pen);
            int middleX = settings::window::wind_width / 2;
            int middleY = settings::window::wind_height / 2;
            painter.drawLine(middleX, middleY + 5, middleX, middleY - 5);
            painter.drawLine(middleX + 5, middleY, middleX - 5, middleY);
        }
        if(spectators.size()==0)
        {
            pen.setColor(QColor(255,255,255));
            painter.setPen(pen);
            font.setPointSize(10);
            font.setBold(true);
            painter.setFont(font);
            painter.drawText(50, settings::window::wind_height / 2.7, " Spectators: ");            
        }
        else
        {
            pen.setColor(redFalse);
            painter.setPen(pen);
            font.setPointSize(10);
            font.setBold(true);
            painter.setFont(font);
            painter.drawText(50, settings::window::wind_height / 2.7, " Spectators: ");
            
            font.setPointSize(9);
            painter.setPen(pen);
            painter.setFont(font);
            for (int i = 0; i < spectators.size(); i++)
            {
                painter.drawText(60, settings::window::wind_height / 2.7 + 15 * (i + 1), spectators[i].c_str());
            }
        }
    }
}
void qWindow::callback()
{
    if (isCSGOActive()) {
        show();
        std::fill(begin(entitiesToScreen), end(entitiesToScreen), EntityToScreen{});
        h.getWorldToScreenData(entitiesToScreen, rcsCross);
        update();
        QTimer::singleShot(16, this, &qWindow::callback);
    }else {
        hide();
        QTimer::singleShot(1000, this, &qWindow::callback);
    }
}
void qWindow::drawESPBoxes(std::array<EntityToScreen, 64> &entitiesToScreen, QPainter &painter, QPen &pen, QFont &font)
{
    font.setPointSize(7);
    font.setBold(false);
    pen.setWidth(2);
    for (int i = 0; i < 64; i++)
    {
        if (entitiesToScreen[i].origin.x == 0 && entitiesToScreen[i].origin.y == 0)
        { //||entitiesToScreen[i].origin.x>width*2||entitiesToScreen[i].origin.y>height*2){
            continue;
        }
        if (entitiesToScreen[i].origin.z == 0)
        {
            float fheight = entitiesToScreen[i].head.y - entitiesToScreen[i].origin.y;
            float fwidth = fheight / 2.3;
            if (entitiesToScreen[i].entityInfo.entity.m_fFlags & IN_DUCK)
            {
                fheight *= .7;
            }
            if (entitiesToScreen[i].myTeam && !h.shootFriends)
            {
                pen.setColor(QColor(h.Colors()[16] * 255, h.Colors()[17] * 255, h.Colors()[18] * 255, 255));
                painter.setPen(pen);
            }
            else
            {
                pen.setColor(QColor(h.Colors()[12] * 255, h.Colors()[13] * 255, h.Colors()[14] * 255, 255));
                painter.setPen(pen);
            }
            //qInfo() << "positions[i].x "<<positions[i].second.x<<" positions[i].y "<<positions[i].second.y;
            painter.drawRect(entitiesToScreen[i].origin.x - fwidth / 2, entitiesToScreen[i].origin.y, fwidth, fheight);
            pen.setColor(QColor(h.Colors()[20], h.Colors()[21], h.Colors()[22], h.Colors()[23]));
            painter.setPen(pen);
            font.setBold(true);
            font.setFamily("LiberationSansBold");
            painter.setFont(font);
            std::string health;
            health = std::to_string(entitiesToScreen[i].entityInfo.entity.m_iHealth);
            painter.drawText(entitiesToScreen[i].origin.x - (fwidth / 2), entitiesToScreen[i].head.y, 1000, 1000, 0, entitiesToScreen[i].name.c_str());

            std::vector<std::string> stringsToDraw;
            stringsToDraw.reserve(3);

            stringsToDraw.push_back(health.c_str());
            if (!entitiesToScreen[i].myTeam || h.shootFriends)
            {

                if (entitiesToScreen[i].scoped)
                {
                    stringsToDraw.push_back("Scoped");
                }
                if (entitiesToScreen[i].defusing)
                {
                    stringsToDraw.push_back("Defusing");
                }
                painter.drawLine(entitiesToScreen[i].origin.x, entitiesToScreen[i].origin.y, settings::window::wind_width * settings::window::cofLineTetherX, settings::window::wind_height * settings::window::cofLineTetherY);

                //helper::clampAngle(&entitiesToScreen[i].entityInfo.entity.m_angNetworkAngles);
                //if(fabsf(entitiesToScreen[i].lby-entitiesToScreen[i].entityInfo.entity.m_angNetworkAngles.y)>35){
                //painter.drawText(entitiesToScreen[i].origin.x-(fwidth/2),entitiesToScreen[i].head.y-fheight*.45,1000,1000,0,"Anti-Aim");
                //}
            }
            int j = 0;
            for (auto str : stringsToDraw)
            {
                j++;
                painter.drawText(entitiesToScreen[i].origin.x - (fwidth / 2), entitiesToScreen[i].head.y + 10 * j, 1000, 1000, 0, str.c_str());
            }
        }
        else if (entitiesToScreen[i].origin.z == -1 && !entitiesToScreen[i].myTeam)
        {
            //draws a line to the entity that is behind me (origin z ==-1 is set earlier if they are behind me)
            pen.setColor(QColor(h.Colors()[20], h.Colors()[21], h.Colors()[22], h.Colors()[23]));
            painter.setPen(pen);
            painter.drawLine(entitiesToScreen[i].origin.x, entitiesToScreen[i].origin.y, settings::window::wind_width * settings::window::cofLineTetherX, settings::window::wind_height * settings::window::cofLineTetherY);
        }
    }
}
void qWindow::drawHitmarker(float width, QPainter &painter, QPen &pen)
{
    pen.setColor(QColor(255, 255, 255));
    pen.setWidthF(width);
    painter.setPen(pen);
    painter.drawLine(settings::window::wind_width / 2 + 3, settings::window::wind_height / 2 + 3, settings::window::wind_width / 2 + settings::misc::hitmarker_length, settings::window::wind_height / 2 + settings::misc::hitmarker_length);
    painter.drawLine(settings::window::wind_width / 2 + 3, settings::window::wind_height / 2 - 3, settings::window::wind_width / 2 + settings::misc::hitmarker_length, settings::window::wind_height / 2 - settings::misc::hitmarker_length);
    painter.drawLine(settings::window::wind_width / 2 - 3, settings::window::wind_height / 2 + 3, settings::window::wind_width / 2 - settings::misc::hitmarker_length, settings::window::wind_height / 2 + settings::misc::hitmarker_length);
    painter.drawLine(settings::window::wind_width / 2 - 3, settings::window::wind_height / 2 - 3, settings::window::wind_width / 2 - settings::misc::hitmarker_length, settings::window::wind_height / 2 - settings::misc::hitmarker_length);
}
