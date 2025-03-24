/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "TaskTimer.h"

class C2dEffect;

class CAttractorScanner {
public:
    static void InjectHooks();

public:
    static constexpr int ms_iScanPeriod = 1500;
    static constexpr int ms_iScanLatency = 3000;

    CAttractorScanner();
    ~CAttractorScanner();

    void ScanForAttractorsInRange(const CPed& ped);
    void Activate();
    void DeActivate();
    bool IsActivated() const;
    void Pause(int32);
    void Clear();

    void SetEffectInUse(C2dEffect* pEffect);
    C2dEffect* GetEffectInUse() const;

    static bool GetClosestAttractorOfType(
        const CVector&   pos,
        const float      radius,
        const int32      ModelIndexToFind,
        const int32      queueType,
        const char*      pTriggeredScriptName,
        const bool       bMustBeFree,
        const C2dEffect* pEffectToNeglect,
        C2dEffect*&      pClosestEffect,
        CEntity*&        pClosestEntity
    );

    CTaskTimer* GetTimer();

private:
    bool m_bActivated;
    CTaskTimer m_timer;
    C2dEffect* m_pPreviousEffect;
    CEntity*   m_pPreviousEntity;
    CEntity*   m_entities[10];
    C2dEffect* m_effects[10];
    float      m_minDistanceSquared[10];

    void ScanForAttractorsInPtrList(CPtrList& ptrList, const CPed& ped);
    void AddEffect(C2dEffect* pEffect, CEntity* pEntity, const CPed& ped);
    void GetNearestAttractorInRange(C2dEffect*& pEffect, CEntity*& pEntity) const;
    static CPed* GetNearestPedNotUsingAttractor(const C2dEffect& effect);
};

VALIDATE_SIZE(CAttractorScanner, 0x90);
