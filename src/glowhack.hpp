#ifndef GLOWHACK_HPP
#define GLOWHACK_HPP
#include <cstdint>
class remote_process;
class pattern_scanner;

template <class T>
class CUtlVector
{
  public:
    T *DataPtr;               // 0000 
    unsigned int Max;         // 0008 
    unsigned int unk02;       // 000C 
    unsigned int Count;       // 00010 
    unsigned int DataPtrBack; // 0014
};//sizeof() == 0x18

struct GlowObjectDefinition_t
{
    bool ShouldDraw(int nSlot) const
    {
        return m_pEntity && (m_nSplitScreenSlot == -1 || m_nSplitScreenSlot == nSlot) && (m_bRenderWhenOccluded || m_bRenderWhenUnoccluded);
    }
    
    long writeStart()
    {
        return (long(&(this)->m_flGlowRed) - long(this));
    }

    long writeEnd()
    {
        return (long(&(this)->m_nNextFreeSlot) - long(this));
    }
    //FIXME: Figure out some of the missing values
    void *m_pEntity;                  //0x0000
    float m_flGlowRed;                //0x0008
    float m_flGlowGreen;              //0x000C
    float m_flGlowBlue;               //0x00010
    float m_flGlowAlpha;              //0x0014
    uint8_t pad_0014[4];              //0x0018
    float m_flSomeFloat;              //0x001C
    uint8_t pad_001C[4];              //0x0020
    float m_flAnotherFloat;           //0x0024
    bool m_bRenderWhenOccluded : 8;   //0x0028
    bool m_bRenderWhenUnoccluded : 8; //0x0029
    bool m_bFullBloomRender : 8;      //0x002A
    uint8_t pad_0027[5];              //0x002B
    int m_nGlowStyle;                 //0x0030
    int m_nSplitScreenSlot;           //0x0034
    int m_nNextFreeSlot;              //0x0038

    //static const int END_OF_FREE_LIST = -1;
    //static const int ENTRY_IN_USE = -2;
}__attribute__((packed)); // sizeof() == 0x3c

class CGlowObjectManager
{
  public:
    CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions; // 0000
    int m_nFirstFreeSlot;                                       // 0014 (054612D4)
    unsigned int unk1;                                          // 0018 (054612D8)
    unsigned int unk2;                                          // 001C (054612DC)
    unsigned int unk3;                                          // 0020 (054612E0)
    unsigned int unk4;                                          // 0024 (054612E4)
    unsigned int unk5;                                          // 0028 (054612E8)
};

class glowhack
{
  public:
    glowhack(remote_process const &csgo_process);
    ~glowhack();
    int run();

  private:
    remote_process const &csgo_process;
    pattern_scanner const csgo_pattern_scanner;
};
#endif
