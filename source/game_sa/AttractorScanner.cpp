#include "StdInc.h"

#include "AttractorScanner.h"

void CAttractorScanner::InjectHooks() {
    RH_ScopedClass(CAttractorScanner);
    RH_ScopedCategoryGlobal();

    // RH_ScopedInstall(ScanForAttractorsInRange , 0x6060A0);
    RH_ScopedInstall(GetClosestAttractorOfType , 0x6002F0);
    // RH_ScopedInstall(ScanForAttractorsInPtrList , 0x6034B0);
    // RH_ScopedInstall(AddEffect , 0x5FFFD0);
    // RH_ScopedInstall(GetNearestAttractorInRange , 0x600180);
    // RH_ScopedInstall(GetNearestPedNotUsingAttractor , 0x600180);
    // RH_ScopedInstall(Clear, 0x5FFF90);
}

CAttractorScanner::CAttractorScanner() :
    m_bActivated(),
    m_timer(),
    m_pPreviousEffect(),
    m_pPreviousEntity(),
    m_entities(),
    m_effects(),
    m_minDistanceSquared() {
}

CAttractorScanner::~CAttractorScanner() {
}

void CAttractorScanner::ScanForAttractorsInRange(const CPed& ped) {
    plugin::CallMethod<0x6060A0, CAttractorScanner*, const CPed&>(this, ped);
}

void CAttractorScanner::Activate() {
    m_bActivated = true;
}

void CAttractorScanner::DeActivate() {
    m_bActivated = false;
}

bool CAttractorScanner::IsActivated() const {
    return m_bActivated;
}

void CAttractorScanner::Pause(int32) {
}

void CAttractorScanner::SetEffectInUse(C2dEffect* pEffect) {
    m_pPreviousEffect = pEffect;
}

C2dEffect* CAttractorScanner::GetEffectInUse() const {
    return m_pPreviousEffect;
}

bool CAttractorScanner::GetClosestAttractorOfType(const CVector& pos, const float radius, const int32 ModelIndexToFind, const int32 queueType, const char* pTriggeredScriptName, const bool bMustBeFree, const C2dEffect* pEffectToNeglect, C2dEffect*& pClosestEffect, CEntity*& pClosestEntity) {
    CEntity* outEntities[1'024];

    pClosestEffect = nullptr;
    pClosestEntity = nullptr;

    int16 outCount = -1;
    CWorld::FindObjectsInRange(pos, radius, false, &outCount, 1'024, outEntities, true, false, false, true, false);

    if (outCount <= 0)
    {
        return false;
    }

    CPedAttractorManager* pedAttractorManager = GetPedAttractorManager();

    float currentDistance = 10'000 * 10'000;
    bool  result          = false;

    for (int32 entityIndex = 0; entityIndex < outCount; entityIndex++)
    {
        CEntity* entity     = outEntities[entityIndex];
        uint32   modelIndex = entity->GetModelIndex();
        if (ModelIndexToFind == -1 || ModelIndexToFind == modelIndex) {
            CBaseModelInfo* modelInfo = CModelInfo::GetModelInfo(modelIndex);
            int32           num2dEffects = modelInfo->GetNum2dEffects();

            for (int32 effectIndex = 0; effectIndex < num2dEffects; effectIndex++)
            {
                C2dEffect * effect = modelInfo->Get2dEffect(effectIndex);
                if (pEffectToNeglect || effect != pEffectToNeglect)
                {
                    C2dEffectPedAttractor* effectPedAttractor = C2dEffect::DynCast<C2dEffectPedAttractor, C2dEffect>(effect);
                    if (effectPedAttractor != nullptr && (!pTriggeredScriptName || !stricmp(pTriggeredScriptName, effectPedAttractor->m_szScriptName)))
                    {
                        if (!bMustBeFree || pedAttractorManager->HasEmptySlot(effectPedAttractor, entity))
                        {
                            CVector effectPosWS = entity->TransformIntoWorldSpace(effect->m_Pos);
                            CVector relativePos = pos - effectPosWS;
                            float   distanceSquared = relativePos.SquaredMagnitude();
                            if (distanceSquared < currentDistance)
                            {
                                currentDistance = distanceSquared;
                                pClosestEffect = effect;
                                pClosestEntity  = entity;
                                result          = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}

CTaskTimer* CAttractorScanner::GetTimer() {
    return &m_timer;
}

void CAttractorScanner::ScanForAttractorsInPtrList(CPtrList& ptrList, const CPed& ped) {
    plugin::CallMethod<0x6034B0, CAttractorScanner*, CPtrList&, const CPed&>(this, ptrList, ped);
}

void CAttractorScanner::AddEffect(C2dEffect* pEffect, CEntity* pEntity, const CPed& ped) {
    plugin::CallMethod<0x5FFFD0, CAttractorScanner*, C2dEffect*, CEntity*, const CPed&>(this, pEffect, pEntity, ped);
}

void CAttractorScanner::GetNearestAttractorInRange(C2dEffect*& pEffect, CEntity*& pEntity) const {
    plugin::CallMethod<0x600180, const CAttractorScanner*, C2dEffect*&, CEntity*&>(this, pEffect, pEntity);
}

CPed* CAttractorScanner::GetNearestPedNotUsingAttractor(const C2dEffect& effect) {
    CPed* ped = plugin::CallAndReturn<CPed*, 0x600180, const C2dEffect&>(effect);
    return ped;
}

void CAttractorScanner::Clear() {
    plugin::CallMethod<0x5FFF90, CAttractorScanner*>(this);
}
