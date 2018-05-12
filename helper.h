#ifndef HELPER_H
#define HELPER_H

#include <cmath>
#include <iostream>
#include <libconfig.h++>
#include <stdlib.h>

#include "types.h"
#include "offsets.h"
#include "remote.h"
#include "settings.h"
#include "atomic"
namespace helper
{
        void clampAngle(QAngle *angle);
        void Smoothing(QAngle* source,QAngle* target, float percentSmoothing, bool willsalt, float errormargin);
        float getDistanceFov(QAngle*delta, Vector* sourcePos,Vector * targetPos);
        float getAngleBasedFOV(const QAngle &aimDelta);
        QAngle calcAngle(Vector *source, Vector *target);
        QAngle addSalt(QAngle angle, float margin);
        bool IgnoreWeapon(int iWeaponID);
        bool ShouldAutoShoot(int iWeaponID);
        void resolve(Entity* ent,Vector* target, float lby);
        std::string getConfigValue(std::string property, libconfig::Config &cfg);
        BoneMatrix getBones(void* bonePtr, int bone);
        Vector WorldToScreen(Vector &camerapos,Vector &enemy, QAngle &myvang, float FOV); //2d vector
        float fShootDistance(int bone);
        Vector RecoilCrosshair(QAngle &vpunch, float FOV); //2d vector
        std::string AtomicBoolToString(std::atomic<bool> *b);
        bool ShouldNotRCS( int iWeaponID );
        float random(float Min, float Max);
}

#endif // HELPER_H
