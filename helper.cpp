#include "helper.h"
#include "settings.h"

namespace helper{
void clampAngle(QAngle *angle){
    while (angle->y>180){
        angle->y-=360;
    }
    while (angle->y<-180){
        angle->y+=360;
    }
    if(angle->x>89){
        angle->x=89;
    }
    else if(angle->x<-89){
        angle->x=-89;
    }
    if(angle->y!=angle->y){
        //cout<<"problem y "<<endl;
        angle->y=0;
    }
    if(angle->x!=angle->x){
        //cout<<"problem x "<<endl;
        angle->x=0;
    }
}
float fShootDistance(int bone){
    switch (bone)
    {
        case 8:case 7:
            return 1;
        case 6: case 0:
            return 3;
        default:
            return 1;
    }
}

Vector WorldToScreen_(Vector &camerapos,Vector &enemy, QAngle &myvang, float FOV)
{
    Vector point(enemy.x, enemy.z,enemy.y);
    Vector newCameraPos(camerapos.x,camerapos.z,camerapos.y);
    point -= newCameraPos;
    QAngle myVang = myvang;
    float magical = 1.3; //just found this thru trial and error, its used to grow or shrink the focallength (for some reason it is needed on my 4:3 monitor because otherwise i get inaccurate focal length.
    myVang.y+=90;
    if(FOV == 0){
        //FOV=90;
    }
    if(FOV >= 90){
        magical = 1.224489796;
    }
    float cosX = cosf((M_PI/180)*myVang.x);
    float cosY = cosf((M_PI/180)*myVang.y);
    float cosZ = cosf((M_PI/180)*myVang.z);
    float sinX = sinf((M_PI/180)*myVang.x);
    float sinY = sinf((M_PI/180)*myVang.y);
    float sinZ = sinf((M_PI/180)*myVang.z);

    float matrix[3][3];
    matrix[0][0] = cosZ * cosY - sinZ * sinX * sinY;
    matrix[0][1] = -cosX * sinZ;
    matrix[0][2] = cosZ * sinY + cosY * sinZ * sinX;
    matrix[1][0] = cosY * sinZ + cosZ * sinX * sinY;
    matrix[1][1] = cosZ * cosX;
    matrix[1][2] = sinZ * sinY - cosZ * cosY * sinX;
    matrix[2][0] = -cosX * sinY;
    matrix[2][1] = sinX;
    matrix[2][2] = cosX * cosY;

    /*Apply rotation matrix to target point*/
    Vector rotatedPoint;
    rotatedPoint.x = matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2] * point.z;
    rotatedPoint.y = matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2] * point.z;
    rotatedPoint.z = matrix[2][0] * point.x + matrix[2][1] * point.y + matrix[2][2] * point.z;

    float focalLength= (settings::wind_height / 2) / tanf((M_PI/180)*(FOV/magical) / 2);
    //std::cout<<"focalL "<<focalLength<<" fov "<<FOV<<std::endl;
    Vector scrPos;
    scrPos.x = focalLength * rotatedPoint.x / rotatedPoint.z + settings::wind_width/2;
    scrPos.y = focalLength * rotatedPoint.y / rotatedPoint.z + settings::wind_height/2;
    scrPos.z = 0;
    if(rotatedPoint.z>0){
        scrPos.x = (settings::wind_width*settings::cofLineTetherX)-(scrPos.x-(settings::wind_width*settings::cofLineTetherX));
        scrPos.y=settings::wind_height+(settings::wind_height*settings::cofLineTetherY-scrPos.y);
        Vector origin(settings::wind_width*settings::cofLineTetherX,settings::wind_height*settings::cofLineTetherY,0);
        Vector end(scrPos.x,scrPos.y,0);
        double len = sqrt(powf(origin.x-end.x,2)+powf(origin.y-end.y,2));
        scrPos.x = end.x +(end.x-origin.x)/len*settings::wind_width;
        scrPos.y = end.y +(end.y-origin.y)/len*settings::wind_width;
        scrPos.z = -1;
    }
    //std::cout<<"scrPos.x "<<scrPos.x<<std::endl;
    //std::cout<<"scrPos.y "<<scrPos.y<<std::endl;
    return scrPos;
}

//thx to Reactiion @ unknowncheats!
bool WorldToScreen( const Vector &vecOrigin, Vector &vecScreen, float *pflViewMatrix )
{
    //std::cout<<"vecOrigin.x "<<vecOrigin.x<<" vecOrigin.y "<<vecOrigin.y<<std::endl;
    vecScreen.x = pflViewMatrix[ 0 ] * vecOrigin.x + pflViewMatrix[ 1 ] * vecOrigin.y + pflViewMatrix[ 2 ] * vecOrigin.z + pflViewMatrix[ 3 ];
    vecScreen.y = pflViewMatrix[ 4 ] * vecOrigin.x + pflViewMatrix[ 5 ] * vecOrigin.y + pflViewMatrix[ 6 ] * vecOrigin.z + pflViewMatrix[ 7 ];

    auto flTemp = pflViewMatrix[ 12 ] * vecOrigin.x + pflViewMatrix[ 13 ] * vecOrigin.y + pflViewMatrix[ 14 ] * vecOrigin.z + pflViewMatrix[ 15 ];
    if( flTemp < 0.01f )
        return false;

    auto invFlTemp = 1.f / flTemp;
    vecScreen.x *= invFlTemp;
    vecScreen.y *= invFlTemp;

    static int iResolution[ 2 ] = { 0 };
    if( !iResolution[ 0 ] || !iResolution[ 1 ] )
    {
        iResolution[ 0 ] = 1600;
        iResolution[ 1 ] = 1200;
    }

    auto x = ( float )iResolution[ 0 ] / 2.f;
    auto y = ( float )iResolution[ 1 ] / 2.f;

    x += 0.5f * vecScreen.x * ( float )iResolution[ 0 ] + 0.5f;
    y -= 0.5f * vecScreen.y * ( float )iResolution[ 1 ] + 0.5f;
    vecScreen.x = x;
    vecScreen.y = y;
    //std::cout<<"x, y"<<x<<", "<<y<<std::endl;
    return true;
}
void Smoothing(QAngle* source, QAngle* target, float percentSmoothing){
    QAngle delta;
    QAngle smoothed;
    delta.x = target->x - source->x;
    delta.y = target->y - source->y;
    //cout<<endl;
    //cout<< "Delta.x, y = "<<delta.x<<", "<<delta.y<<endl;
    clampAngle(&delta);
    float sqDistance = sqrt((delta.x*delta.x)+(delta.y*delta.y));
    smoothed.x = delta.x/(sqDistance/percentSmoothing);
    smoothed.y = delta.y/(sqDistance/percentSmoothing);
    float smoothedSqDistance = sqrt((smoothed.x*smoothed.x)+(smoothed.y*smoothed.y));
    if(smoothedSqDistance<sqDistance){
        delta.x = smoothed.x;
        delta.y = smoothed.y;
        //std::cout<<"smoothed "<<smoothed.x<<", "<<smoothed.y<<std::endl;
    }
    /*delta.x=abs(delta.x/sqDistance)<abs(delta.x)?delta.x/sqDistance:delta.x;
    delta.y=abs(delta.y/sqDistance)<abs(delta.y)?delta.y/sqDistance:delta.y;
    delta.x*=percentSmoothing;
    delta.y*=percentSmoothing;*/
    //std::cout<<"abs(delta.x/sqDistance): "<<fabsf(delta.x/sqDistance)<<" abs delta x: "<<fabsf(delta.x)<<" sqdist: " <<sqDistance<<std::endl;
    //std::cout<<"abs(delta.y/sqDistance): "<<fabsf(delta.y/sqDistance)<<" abs delta y: "<<fabsf(delta.y)<<" sqdist: " <<sqDistance<<std::endl;    clampAngle(&delta);
    //std::cout<<"sqDistance "<<sqDistance<<"smthDist: "<<smoothedSqDistance<<std::endl;
    //std::cout<< "Delta.x, y = "<<delta.x<<", "<<delta.y<<" sqdist: "<<sqDistance<<std::endl;
    target->x=source->x+delta.x;
    target->y=source->y+delta.y;
    usleep(200);

}
QAngle calcAngle(Vector* source, Vector* target){
    QAngle angle;
    Vector delta = {
        (source->x-target->x), (source->y-target->y), (source->z-target->z)
    };
    //cout<<"delta x y z "<<delta.x<<", "<<delta.y<<", "<<delta.z<<endl;
    double hyp = sqrt(delta.x*delta.x + delta.y*delta.y);
    angle.x = (float) (atanf(delta.z/hyp) * 180.0f/M_PI);
    angle.y = (float) (atanf(delta.y/delta.x) * 180.0f/M_PI);
    angle.z = 0.0f;
    //cout<<"angle x y z "<<angle.x<<", "<<angle.y<<", "<<angle.z<<endl;
    if(delta.x>=0){
        angle.y+=180;
    }
    return angle;
}
Vector RecoilCrosshair(QAngle &vpunch, float FOV){
    Vector crosshair;
    float xDelta = (tanf( (vpunch.y / 2) * M_PI/180) / tanf( (FOV / 2) * M_PI/180))*settings::wind_width;//x delta takes the y vpunch because it is yaw
    float yDelta = (tanf( (vpunch.x / 2) * M_PI/180) / tanf( (FOV / 2) * M_PI/180))*settings::wind_height;
    //std::cout<<"xDelta: "<<xDelta<<" yDelta: "<<yDelta<<std::endl;
    crosshair.x = settings::wind_width/2 - xDelta;
    crosshair.y = settings::wind_height/2 + yDelta;
    //std::cout<<" crosshair.x: "<<crosshair.x<<" crosshair.y: "<<crosshair.y<<std::endl;
    return crosshair;
}
float getDistanceFov(QAngle* delta,Vector* sourcePos,Vector* targetPos){
    //float fDistance = sqrt((sourcePos->x-targetPos->x)*(sourcePos->x-targetPos->x)+(sourcePos->y-targetPos->y)*(sourcePos->y-targetPos->y)+(sourcePos->z-targetPos->z)*(sourcePos->z-targetPos->z));
    /*float deltaAngle;
    if(sourcePos->x-targetPos->x>0){
        deltaAngle = sqrt((delta->x)*(delta->x)+(delta->y)*(delta->y));
    }
    else{
        deltaAngle = sqrt((delta->x)*(delta->x)+(delta->y)*(delta->y));
    }
    //cout<<"vecdistance: "<<fDistance<<endl;
    float opposite = sinf(deltaAngle*M_PI/180)*fDistance;
    cout<<"dAngle "<<deltaAngle<<endl;
    cout<<"sinf "<< sinf(deltaAngle*M_PI/180)<<endl;
    cout<<"opposite: "<<opposite<<endl;
    return sinf(deltaAngle*M_PI/180)*fDistance;*/
    //cout<<"deltay "<<delta->y<<endl;
    //cout<<"sinfy "<< sinf(delta->y*M_PI/180/2)<<endl;
    //float distx = sinf(fabsf(delta->x/2)*M_PI/180);
    //float disty = sinf(fabsf(delta->y/2)*M_PI/180);
    /*distx*=90;
    disty*=180;
    float ffov = sqrt(distx*distx+disty*disty);
    cout<<"deltax deltay distx, disty, ffov: "<<delta->x<<" "<<delta->y<<" "<<distx<<" "<<disty<<" "<<ffov<<endl;
    return ffov;*/
    float yawHyp = sqrt((sourcePos->y-targetPos->y)*(sourcePos->y-targetPos->y)+(sourcePos->x-targetPos->x)*(sourcePos->x-targetPos->x));
    float pitchHyp = sqrt(yawHyp*yawHyp+(sourcePos->z-targetPos->z)*(sourcePos->z-targetPos->z));
    if(yawHyp<150){//if we are too close things get weird, so we are just going to pretend we are never closer than 150 units
        yawHyp=150;
    }
    if(pitchHyp<150){
        pitchHyp=150;
    }
    float realYaw = yawHyp*sinf(fabsf(delta->y/2*M_PI/180));
    float realPitch = pitchHyp*sinf(fabsf(delta->x*M_PI/180));
    //cout<<" yawHyp "<<yawHyp<<" pitchHyp "<<pitchHyp<<endl;
    float fov = sqrt(realYaw*realYaw+realPitch*realPitch);
    //cout<<"fov: "<<fov<<endl;
    return fov;
    //cout<<"realYaw: "<<realYaw<<" yawHyp: "<<yawHyp<<endl;
    //cout<<"realPitch: "<<realPitch<<" pitchHtp: "<<pitchHyp<<endl;
}
bool IgnoreWeapon( int iWeaponID )
{
    return( iWeaponID == WEAPON_NONE || iWeaponID == WEAPON_KNIFE || iWeaponID == WEAPON_FLASHBANG || iWeaponID == WEAPON_CT_MOLOTOV || iWeaponID == WEAPON_SMOKE
            || iWeaponID == WEAPON_HEGRENADE || iWeaponID == WEAPON_DECOY || iWeaponID == WEAPON_T_MOLOTOV || iWeaponID == WEAPON_C4 || iWeaponID == WEAPON_KNIFEGG );
}
bool ShouldAutoShoot( int iWeaponID )
{
    return( iWeaponID == WEAPON_DUAL|| iWeaponID == WEAPON_FIVE7 || iWeaponID == WEAPON_GLOCK
            || iWeaponID == WEAPON_P250_CZ75 || iWeaponID == WEAPON_TEC9 || iWeaponID == WEAPON_HKP2000 );
}
void resolve(Entity* ent, Vector* target, float lby){
    /*float x = target->x -ent->m_vecOrigin.x;
    float y = target->y -ent->m_vecOrigin.y;
    float angle = lby-ent->m_angNetworkAngles.y;
    cout<<"angle: "<<angle<<" x: "<<x<<" y: "<<y<<" lby: "<<lby<<" ang.y: "<<ent->m_angNetworkAngles.y<<endl;
    float f = (x*(cosf(angle*M_PI/180.0f)*180.0f/M_PI));
    float s = (y*(sinf(angle*M_PI/180.0f)*180.0f/M_PI));
    float t = (y*(cosf(angle*M_PI/180.0f)*180.0f/M_PI));
    float fourth = (x*(sinf(angle*M_PI/180.0f)*180.0f/M_PI));
    cout<<"1: "<<f<<" 2: "<<s<<" 3: "<<t<<" 4: "<<fourth<<endl;*/
    clampAngle(&(ent->m_angNetworkAngles));
    float x = target->x -ent->m_vecOrigin.x;
    float y = target->y -ent->m_vecOrigin.y;
    float angle = lby-ent->m_angNetworkAngles.y;
    //cout<<"angle: "<<angle<<" x: "<<x<<" y: "<<y<<" lby: "<<lby<<" ang.y: "<<ent->m_angNetworkAngles.y<<endl;
    float xC = (x*(cosf(angle*M_PI/180.0f)));
    float yS = (y*(sinf(angle*M_PI/180.0f)));
    float yC = (y*(cosf(angle*M_PI/180.0f)));
    float xS = (x*(sinf(angle*M_PI/180.0f)));
    //cout<<"1: "<<xC<<" 2: "<<yS<<" 3: "<<yC<<" 4: "<<xS<<endl;
    float xNew = xC-yS;
    float yNew = yC+xS;
    //cout<<"yNew: "<<yNew<<" xNew: "<<xNew<<endl;
    target->x+=xNew;
    target->y+=yNew;
}
std::string getConfigValue(std::string property, libconfig::Config &cfg) {
    try {
        std::string name = cfg.lookup(property);
        return name;
    } catch (const libconfig::SettingNotFoundException &nfex) {
        std::stringstream ss;
        ss << "Cannot find property: '" << property << "' in cfg file";
        std::cout<<ss.str();
    }
    return NULL;
}
}

