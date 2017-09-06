#include "window.h"

int i = 0;
qWindow::qWindow(QWidget *parent) : QWidget(parent)
{
    QSize size = qApp->screens()[0]->size();
    settings::window::wind_height = size.height();
    settings::window::wind_width = size.width();
    setGeometry(settings::window::wind_x,settings::window::wind_y,settings::window::wind_width,settings::window::wind_height);
    setAutoFillBackground(false);
    //setWindowFlags(Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint|Qt::WindowTransparentForInput|Qt::X11BypassWindowManagerHint);
    //m_button = new QPushButton("gang shit", this);
    //m_button->setGeometry(0,1,200,100);
    QTimer::singleShot(1,this,SLOT(callback()));//or call callback() directly here
    //m_button->setWindowOpacity(qreal(100)/100);
}
void qWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(105,225,225, 0));
    painter.drawRect(settings::window::wind_x,settings::window::wind_y,settings::window::wind_width,settings::window::wind_height);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(QColor(255,255,255,255));
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
    painter.drawText(10,15,"JWAim");
    font.setPointSize(6);

    QColor redFalse(255,050,05);
    QColor greenTrue(050, 255, 05);
    painter.setFont(font);

    pen.setColor(h.ShouldRadarHack?greenTrue:redFalse);
    painter.setPen(pen);
    painter.drawText(12, 25, (h.ShouldRadarHackToggleKey + " RadarHack: " + helper::AtomicBoolToString(&h.ShouldRadarHack)).c_str());

    pen.setColor(h.ShouldGlow?greenTrue:redFalse);
    painter.setPen(pen);
    painter.drawText(12,35,(h.ShouldGlowToggleKey + " Glow: "+helper::AtomicBoolToString(&h.ShouldGlow)).c_str());

    pen.setColor(h.ShouldESP?greenTrue:redFalse);
    painter.setPen(pen);
    painter.drawText(12,45,(h.ShouldESPToggleKey + " ESP: "+helper::AtomicBoolToString(&h.ShouldESP)).c_str());

    pen.setColor(h.ShouldBhop?greenTrue:redFalse);
    painter.setPen(pen);
    painter.drawText(12, 55, (h.ShouldBhopToggleKey + " BHop: " + helper::AtomicBoolToString(&h.ShouldBhop)).c_str());

    pen.setColor(h.ShouldAimAssist?greenTrue:redFalse);
    painter.setPen(pen);
    painter.drawText(12, 65, (h.ShouldAimAssistToggleKey + " AimAssist: " + helper::AtomicBoolToString(&h.ShouldAimAssist)).c_str());

    pen.setColor(h.ShouldRage?greenTrue:redFalse);
    painter.setPen(pen);
    painter.drawText(12, 75, (h.ShouldRageToggleKey + " Rage: "+helper::AtomicBoolToString(&h.ShouldRage)).c_str());

    pen.setColor(h.ShouldNoFlash?greenTrue:redFalse);
    painter.setPen(pen);
    painter.drawText(12, 85, (h.ShouldNoFlashToggleKey + " No Flash: "+helper::AtomicBoolToString(&h.ShouldNoFlash)).c_str());

    pen.setColor(h.ShouldRCS?greenTrue:redFalse);
    painter.setPen(pen);
    painter.drawText(12, 95, (h.ShouldRCSToggleKey + " RCS: "+helper::AtomicBoolToString(&h.ShouldRCS)).c_str());

    pen.setColor(QColor(255,255,255));
    painter.setPen(pen);
    if(h.IsConnected()&&h.ShouldESP){
        for(int i = 0;i<64;i++)
        {
            if(entitiesToScreen[i].origin.x==0&&entitiesToScreen[i].origin.y==0){//||entitiesToScreen[i].origin.x>width*2||entitiesToScreen[i].origin.y>height*2){
               continue;
            }
            if(entitiesToScreen[i].origin.z==0){
            float fheight = entitiesToScreen[i].head.y-entitiesToScreen[i].origin.y;
            float fwidth = fheight / 2.3;
            if(entitiesToScreen[i].entityInfo.entity.m_fFlags&IN_DUCK){
                  fheight*=.7;
            }
            pen.setWidth(2);
            if(entitiesToScreen[i].myTeam&&!h.shootFriends){
                pen.setColor(QColor(h.Colors()[16]*255,h.Colors()[17]*255,h.Colors()[18]*255,255));
                painter.setPen(pen);
            }
            else{
                pen.setColor(QColor(h.Colors()[12]*255,h.Colors()[13]*255,h.Colors()[14]*255,255));
                painter.setPen(pen);
            }
            //qInfo() << "positions[i].x "<<positions[i].second.x<<" positions[i].y "<<positions[i].second.y;
            painter.drawRect(entitiesToScreen[i].origin.x-fwidth/2,entitiesToScreen[i].origin.y,fwidth,fheight);
            pen.setColor(QColor(h.Colors()[20],h.Colors()[21],h.Colors()[22],h.Colors()[23]));
            painter.setPen(pen);
            font.setBold(true);
            font.setFamily("LiberationSansBold");
            painter.setFont(font);
            std::string health;
            health = std::to_string(entitiesToScreen[i].entityInfo.entity.m_iHealth);
            painter.drawText(entitiesToScreen[i].origin.x-(fwidth/2),entitiesToScreen[i].head.y,1000,1000,0,health.c_str());
            if(!entitiesToScreen[i].myTeam||h.shootFriends){
                std::string scoped = entitiesToScreen[i].scoped ? "Scoped":"";
                painter.drawText(entitiesToScreen[i].origin.x-(fwidth/2),entitiesToScreen[i].head.y-fheight*.15,1000,1000,0,scoped.c_str());

                std::string reloading = entitiesToScreen[i].entityInfo.entity.m_fFlags&IN_RELOAD?"Reloading":"";
                painter.drawText(entitiesToScreen[i].origin.x-(fwidth/2),entitiesToScreen[i].head.y-fheight*.30,1000,1000,0,reloading.c_str());

                std::string defusing = entitiesToScreen[i].defusing?"Defusing":"";
                painter.drawText(entitiesToScreen[i].origin.x-(fwidth/2),entitiesToScreen[i].head.y-fheight*.45,1000,1000,0,defusing.c_str());

                painter.drawLine(entitiesToScreen[i].origin.x,entitiesToScreen[i].origin.y,settings::window::wind_width*settings::window::cofLineTetherX,settings::window::wind_height*settings::window::cofLineTetherY);

                //helper::clampAngle(&entitiesToScreen[i].entityInfo.entity.m_angNetworkAngles);
                //if(fabsf(entitiesToScreen[i].lby-entitiesToScreen[i].entityInfo.entity.m_angNetworkAngles.y)>35){
                    //painter.drawText(entitiesToScreen[i].origin.x-(fwidth/2),entitiesToScreen[i].head.y-fheight*.45,1000,1000,0,"Anti-Aim");
                //}
            }
            }
            else if(entitiesToScreen[i].origin.z==-1&&!entitiesToScreen[i].myTeam){
                pen.setColor(QColor(h.Colors()[20],h.Colors()[21],h.Colors()[22],h.Colors()[23]));
                painter.setPen(pen);
                painter.drawLine(entitiesToScreen[i].origin.x,entitiesToScreen[i].origin.y,settings::window::wind_width*settings::window::cofLineTetherX,settings::window::wind_height*settings::window::cofLineTetherY);
            }
        }
        if(!(rcsCross.x==0&&rcsCross.y==0)){
        if(h.drawrcsCrosshair){
            pen.setColor(QColor(h.Colors()[20],h.Colors()[21],h.Colors()[22],h.Colors()[23]));
            pen.setWidthF(1.8);
            painter.setPen(pen);
            painter.drawLine(rcsCross.x+5,rcsCross.y,rcsCross.x-5,rcsCross.y);
            painter.drawLine(rcsCross.x,rcsCross.y+5,rcsCross.x,rcsCross.y-5);
        }
        else if(h.staticCrosshair){
            pen.setColor(QColor(h.Colors()[20],h.Colors()[21],h.Colors()[22],h.Colors()[23]));
            pen.setWidthF(1.8);
            painter.setPen(pen);
            int middleX = settings::window::wind_width/2;
            int middleY = settings::window::wind_height/2;
            painter.drawLine(middleX,middleY+5,middleX,middleY-5);
            painter.drawLine(middleX+5,middleY,middleX-5,middleY);
        }
        }
    }
}
void qWindow::callback()
{
    std::fill(begin(entitiesToScreen),end(entitiesToScreen),EntityToScreen{});
    h.getWorldToScreenData(entitiesToScreen, rcsCross);
    update();
    QTimer::singleShot(16,this,SLOT(callback()));
}
