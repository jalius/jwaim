#ifndef __TYPES_H_
#define __TYPES_H_
#define Assert( _exp ) ((void)0)
#define FastSqrt(x) (sqrt)(x)


struct QAngle {
    float x; // Pitch
    float y; // Yaw
    float z; // Roll
};

class Vector
{
public:
    float x, y, z;
    Vector(void);
    Vector(float X, float Y, float Z);
    void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
    bool IsValid() const;
    float operator[](int i) const;
    float& operator[](int i);
    inline void Zero();
    bool operator==(const Vector& v) const;
    bool operator!=(const Vector& v) const;
    inline Vector&	operator+=(const Vector &v);
    inline Vector&	operator-=(const Vector &v);
    inline Vector&	operator*=(const Vector &v);
    inline Vector&	operator*=(float s);
    inline Vector&	operator/=(const Vector &v);
    inline Vector&	operator/=(float s);
    inline Vector&	operator+=(float fl);
    inline Vector&	operator-=(float fl);
    inline float	Length() const;
    inline float LengthSqr(void) const
    {

        return (x*x + y*y + z*z);
    }
    bool IsZero(float tolerance = 0.01f) const
    {
        return (x > -tolerance && x < tolerance &&
            y > -tolerance && y < tolerance &&
            z > -tolerance && z < tolerance);
    }
    Vector	Normalize();
    Vector	NormalizeInPlace();
    inline float	DistTo(const Vector &vOther) const;
    inline float	DistToSqr(const Vector &vOther) const;
    float	Dot(const Vector& vOther) const;
    float	Length2D(void) const;
    float	Length2DSqr(void) const;
    Vector& operator=(const Vector &vOther);
    Vector	operator-(void) const;
    Vector	operator+(const Vector& v) const;
    Vector	operator-(const Vector& v) const;
    Vector	operator*(const Vector& v) const;
    Vector	operator/(const Vector& v) const;
    Vector	operator*(float fl) const;
    Vector	operator/(float fl) const;
    // Base address...
    float* Base();
    float const* Base() const;
};

//===============================================
inline void Vector::Init(float ix, float iy, float iz)
{
    x = ix; y = iy; z = iz;

}
//===============================================
inline Vector::Vector(float X, float Y, float Z)
{
    x = X; y = Y; z = Z;

}
//===============================================
inline Vector::Vector(void){ }
//===============================================
inline void Vector::Zero()
{
    x = y = z = 0.0f;
}
//===============================================
inline void VectorClear(Vector& a)
{
    a.x = a.y = a.z = 0.0f;
}
//===============================================
inline Vector& Vector::operator=(const Vector &vOther)
{
    x = vOther.x; y = vOther.y; z = vOther.z;
    return *this;
}
//===============================================
inline float& Vector::operator[](int i)
{
    Assert((i >= 0) && (i < 3));
    return ((float*)this)[i];
}
//===============================================
inline float Vector::operator[](int i) const
{
    Assert((i >= 0) && (i < 3));
    return ((float*)this)[i];
}
//===============================================
inline bool Vector::operator==(const Vector& src) const
{
    return (src.x == x) && (src.y == y) && (src.z == z);
}
//===============================================
inline bool Vector::operator!=(const Vector& src) const
{
    return (src.x != x) || (src.y != y) || (src.z != z);
}
//===============================================
inline void VectorCopy(const Vector& src, Vector& dst)
{
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}
//===============================================
inline  Vector& Vector::operator+=(const Vector& v)
{
    x += v.x; y += v.y; z += v.z;
    return *this;
}
//===============================================
inline  Vector& Vector::operator-=(const Vector& v)
{
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}
//===============================================
inline  Vector& Vector::operator*=(float fl)
{
    x *= fl;
    y *= fl;
    z *= fl;

    return *this;
}
//===============================================
inline  Vector& Vector::operator*=(const Vector& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;

    return *this;
}
//===============================================
inline Vector&	Vector::operator+=(float fl)
{
    x += fl;
    y += fl;
    z += fl;

    return *this;
}
//===============================================
inline Vector&	Vector::operator-=(float fl)
{
    x -= fl;
    y -= fl;
    z -= fl;

    return *this;
}
//===============================================
inline  Vector& Vector::operator/=(float fl)
{
    Assert(fl != 0.0f);
    float oofl = 1.0f / fl;
    x *= oofl;
    y *= oofl;
    z *= oofl;

    return *this;
}
//===============================================
inline  Vector& Vector::operator/=(const Vector& v)
{
    Assert(v.x != 0.0f && v.y != 0.0f && v.z != 0.0f);
    x /= v.x;
    y /= v.y;
    z /= v.z;

    return *this;
}
//===============================================
inline float Vector::Length(void) const
{
    float root = 0.0f;

    float sqsr = x*x + y*y + z*z;

    root = sqrt(sqsr);

    return root;
}
//===============================================
inline float Vector::Length2D(void) const
{
    float root = 0.0f;

    float sqst = x*x + y*y;

    root = sqrt(sqst);

    return root;
}
//===============================================
inline float Vector::Length2DSqr(void) const
{
    return (x*x + y*y);
}
//===============================================
inline Vector CrossProduct(const Vector& a, const Vector& b)
{
    return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
//===============================================
float Vector::DistTo(const Vector &vOther) const
{
    Vector delta;

    delta.x = x - vOther.x;
    delta.y = y - vOther.y;
    delta.z = z - vOther.z;

    return delta.Length();
}
float Vector::DistToSqr(const Vector &vOther) const
{
    Vector delta;

    delta.x = x - vOther.x;
    delta.y = y - vOther.y;
    delta.z = z - vOther.z;

    return delta.LengthSqr();
}
//===============================================
inline Vector Vector::Normalize()
{
    Vector vector;
    float length = this->Length();

    if (length != 0)
    {
        vector.x = x / length;
        vector.y = y / length;
        vector.z = z / length;
    }
    else
    {
        vector.x = vector.y = 0.0f; vector.z = 1.0f;
    }

    return vector;
}
//===============================================
inline Vector Vector::NormalizeInPlace()
{
    Vector& v = *this;

    float iradius = 1.f / (this->Length() + 1.192092896e-07F); //FLT_EPSILON

    v.x *= iradius;
    v.y *= iradius;
    v.z *= iradius;

    return v;
}
//===============================================
inline float VectorNormalize(Vector& v)
{
    Assert(v.IsValid());
    float l = v.Length();
    if (l != 0.0f)
    {
        v /= l;
    }
    else
    {
        // FIXME:
        // Just copying the existing implemenation; shouldn't res.z == 0?
        v.x = v.y = 0.0f; v.z = 1.0f;
    }
    return l;
}
//===============================================
inline float VectorNormalize(float * v)
{
    return VectorNormalize(*(reinterpret_cast<Vector *>(v)));
}
//===============================================
inline Vector Vector::operator+(const Vector& v) const
{
    Vector res;
    res.x = x + v.x;
    res.y = y + v.y;
    res.z = z + v.z;
    return res;
}

//===============================================
inline Vector Vector::operator-(const Vector& v) const
{
    Vector res;
    res.x = x - v.x;
    res.y = y - v.y;
    res.z = z - v.z;
    return res;
}
//===============================================
inline Vector Vector::operator*(float fl) const
{
    Vector res;
    res.x = x * fl;
    res.y = y * fl;
    res.z = z * fl;
    return res;
}
//===============================================
inline Vector Vector::operator*(const Vector& v) const
{
    Vector res;
    res.x = x * v.x;
    res.y = y * v.y;
    res.z = z * v.z;
    return res;
}
//===============================================
inline Vector Vector::operator/(float fl) const
{
    Vector res;
    res.x = x / fl;
    res.y = y / fl;
    res.z = z / fl;
    return res;
}
//===============================================
inline Vector Vector::operator/(const Vector& v) const
{
    Vector res;
    res.x = x / v.x;
    res.y = y / v.y;
    res.z = z / v.z;
    return res;
}
inline float Vector::Dot(const Vector& vOther) const
{
    const Vector& a = *this;
    return(a.x*vOther.x + a.y*vOther.y + a.z*vOther.z);
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------

inline float VectorLength(const Vector& v)
{
    return (float)FastSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

//VECTOR SUBTRAC
inline void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
}

//VECTORADD
inline void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* Vector::Base()
{
    return (float*)this;
}

inline float const* Vector::Base() const
{
    return (float const*)this;
}

inline void VectorMAInline(const float* start, float scale, const float* direction, float* dest)
{
    dest[0] = start[0] + direction[0] * scale;
    dest[1] = start[1] + direction[1] * scale;
    dest[2] = start[2] + direction[2] * scale;
}

inline void VectorMAInline(const Vector& start, float scale, const Vector& direction, Vector& dest)
{
    dest.x = start.x + direction.x*scale;
    dest.y = start.y + direction.y*scale;
    dest.z = start.z + direction.z*scale;
}

inline void VectorMA(const Vector& start, float scale, const Vector& direction, Vector& dest)
{
    VectorMAInline(start, scale, direction, dest);
}

inline void VectorMA(const float * start, float scale, const float *direction, float *dest)
{
    VectorMAInline(start, scale, direction, dest);
}

struct Color {
    unsigned char _color[4];
};

struct Vector2D {
    float x, y;
};

struct BoneMatrix {
    char __buff_0x00[0xC];//0x00
    float x;//0xC
    char __buff_0x10[0xC];//0x10
    float y;//0x1c
    char __buff_0x20[0xC];//0x20
    float z;//0x2c
};

struct Entity {
    char __buf_0x00[0x94]; // 0x0
    int ID;// 0x94
    char __buf_0x98[0x34]; // 0x98
    Vector m_vecAbsVelocity; // 0xCC
    Vector m_vecAbsOrigin; // 0xD8
    Vector m_vecOrigin; // 0xE4
    Vector m_vecAngVelocity; // 0xF0
    QAngle m_angAbsRotation; // 0xFC
    QAngle m_angRotation; // 0x108
    float m_flGravity; // 0x114
    float m_flProxyRandomValue; // 0x118
    int m_iEFlags; // 0x11C
    unsigned char m_nWaterType; // 0x120
    unsigned char m_bDormant; // 0x121
    char __buf_0x122[0x2]; // 0x122
    float m_fEffects; // 0x124
    int m_iTeamNum; // 0x128
    int m_iPendingTeamNum; // 0x12C
    char __buf_0x130[0x4]; // 0x130
    int m_iHealth; // 0x134
    int m_fFlags; // 0x138
    Vector m_vecViewOffset; // 0x13C
    Vector m_vecVelocity; // 0x148
    Vector m_vecBaseVelocity; // 0x154
    QAngle m_angNetworkAngles   ; // 0x160
    Vector m_vecNetworkOrigin; // 0x16C
};

enum WeaponIndex {
    WEAPON_NONE,
    WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
    WEAPON_MAX
};

struct EntityInfo{
    Entity entity;
    void* entityPtr;
};

struct EntityToScreen{
    Vector head;
    Vector origin;
    Vector eyes;
    Vector aimLine;
    EntityInfo entityInfo;
    std::string name;
    int health;
    bool scoped;
    bool myTeam;
    bool defusing;
    bool reloading;
    float lby;
    bool spectatingMe;
};

struct FOV_t{
    int iFOV;
    int iFOVStart;
};

#endif
