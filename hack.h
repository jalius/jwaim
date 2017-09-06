#ifndef __HACK_H_
#define __HACK_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include <X11/extensions/XTest.h>

#include <chrono>
#include <thread>
#include <stddef.h>
#include <libconfig.h++>
#include <iostream>
#include <atomic>
#include <cmath>
#include <mutex>
#include <math.h>
#include <memory.h>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include "offsets.h"
#include "remote.h"
#include "types.h"
#include "helper.h"
#include "settings.h"


using namespace std;
using namespace libconfig;

#define TEAM_SPECTATOR			1

#define LIFE_ALIVE			0 // alive
#define LIFE_DYING			1 // playing death animation or still falling off of a ledge waiting to hit ground
#define LIFE_DEAD			2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

#define ON_GROUND (1<<0)
#define IN_JUMP   (1<<1) // Jump
#define IN_DUCK   (1<<2)
#define IN_RELOAD (1 << 13) // Reload weapon

#define MAX_TRAIL_LENGTH		30
#define MAX_PLAYER_NAME_LENGTH		128

class hack {

	template<class T> class CUtlVector {
	public:
		T* DataPtr; // 0000 (054612C0)
		unsigned int Max; // 0004 (054612C4)
		unsigned int unk02; // 0008 (054612C8)
		unsigned int Count; // 000C (054612CC)
		unsigned int DataPtrBack; // 0010 (054612D0)
	};
    public:
	struct GlowObjectDefinition_t {
		bool ShouldDraw( int nSlot ) const {
			return m_pEntity && ( m_nSplitScreenSlot == -1 || m_nSplitScreenSlot == nSlot ) && ( m_bRenderWhenOccluded || m_bRenderWhenUnoccluded );
		}

		bool IsUnused() const {
			return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE;
		}

		long writeStart() {
			return (long(&(this)->m_flGlowRed) - long(this));
		}

		long writeEnd() {
			return (long(&(this)->unk2) - long(this));
		}

		void* m_pEntity;
		float m_flGlowRed;
		float m_flGlowGreen;
		float m_flGlowBlue;
		float m_flGlowAlpha;
		uint8_t unk0[16];
		bool m_bRenderWhenOccluded : 8;
		bool m_bRenderWhenUnoccluded : 8;
		bool m_bFullBloomRender : 8;
		unsigned char unk1;
		int m_nFullBloomStencilTestValue;
		int m_nSplitScreenSlot;
		int m_nNextFreeSlot;
		unsigned char unk2[4];

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	}; // sizeof() == 0x34

	class CGlowObjectManager
	{
	public:
		CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions; // 0000
		int m_nFirstFreeSlot; // 0014 (054612D4)
		unsigned int unk1; // 0018 (054612D8)
		unsigned int unk2; // 001C (054612DC)
		unsigned int unk3; // 0020 (054612E0)
		unsigned int unk4; // 0024 (054612E4)
		unsigned int unk5; // 0028 (054612E8)
	};

public:
    void init();
    bool checkKeys();
    void bhop();
    void noFlash();
    bool glow();
    void trigger();
    void aim();
    void setFov();
    void setHands();
    void setupIsConnected();
    bool IsConnected();
    double *Colors();
    //settings
    Config cfg;
    //togglable settings

    std::atomic<bool> ShouldGlow;
    std::string ShouldGlowToggleKey;
    std::atomic<bool> ShouldBhop;
    std::string ShouldBhopToggleKey;
    std::atomic<bool> ShouldNoFlash;
    std::string ShouldNoFlashToggleKey;
    std::atomic<bool> ShouldRCS;
    std::string ShouldRCSToggleKey;
    std::atomic<bool> ShouldESP;
    std::string ShouldESPToggleKey;
    std::atomic<bool> ShouldRage;
    std::string ShouldRageToggleKey;
    std::atomic<bool> ShouldRadarHack;
    std::string ShouldRadarHackToggleKey;
    std::atomic<bool> ShouldAimAssist;
    std::string ShouldAimAssistToggleKey;
    //runtime set settings
    int preferredBone;
    std::vector<int> preferredBones;
    float flashMax;
    float fov;
    float percentSmoothing;
    int viewFov;
    bool noHands;
    bool resolver;
    bool shootFriends;
    bool legitGlow;
    bool drawrcsCrosshair;
    bool staticCrosshair;
    //end settings
    bool getWorldToScreenData(std::array<EntityToScreen,64> &output, Vector &rcsCross);

private:
    void setVAng(QAngle *newAngle, unsigned long addressOfViewAngle);
    int getClosestBone(unsigned long m_pStudioBones, std::vector<int> &preferredBones, QAngle &curViewAngle, QAngle &aimPunch, Vector &myPos);
    int getLifeState(unsigned long entityPtr);

    std::atomic<bool> isConnected;

    void readEntities(std::array<EntityInfo,64> &rentities);
    void writeEntities(std::array<EntityInfo,64> &wentities);

    boost::shared_mutex entities_access;
    std::array<EntityInfo,64> entities;

    std::array<std::pair<Vector,Vector>,64> screenPositions;


    remote::Handle csgo;
    Display* display;
    Screen* screen;
    char keys[32];
    char lastkeys[32];
    remote::MapModuleMemoryRegion client;
    remote::MapModuleMemoryRegion engine;

    double* colors;
    int keycodeBhop;
    int keycodeGlow;
    int keycodeNoFlash;
    int keycodeRage;
    int keycodeRCS;
    int keycodeESP;
    int keycodeRadarHack;
    int keycodeAim;
    unsigned char spotted;
    int toggleOn = 5;
    int toggleOff = 4;

    //
    std::atomic<bool> entityInCrossHair;

    //static addresses (set at hack::init())
    unsigned long m_addressOfGlowPointer;
    unsigned long m_addressOfLocalPlayer;
    unsigned long m_addressOfForceAttack;
    unsigned long m_addressOfAlt1;
    unsigned long m_addressOfAlt2;
    unsigned long m_addressOfJump;
    unsigned long basePointerOfViewAngle;
    unsigned long addressServerDetail;
    unsigned long addressIsConnected;

};
extern hack h;
#endif
