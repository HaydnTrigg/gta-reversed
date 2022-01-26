#include "StdInc.h"

void CEventHitByWaterCannon::InjectHooks()
{
    ReversibleHooks::Install("CEventHitByWaterCannon", "Constructor", 0x4B1290, &CEventHitByWaterCannon::Constructor);
    ReversibleHooks::Install("CEventHitByWaterCannon", "AffectsPed_Reversed", 0x4B1330, &CEventHitByWaterCannon::AffectsPed_Reversed);
}

// 0x4B1290
CEventHitByWaterCannon::CEventHitByWaterCannon(CVector const& point, CVector const& moveSpeed)
{
    m_point = point;
    m_moveSpeed = moveSpeed;
}

CEventHitByWaterCannon* CEventHitByWaterCannon::Constructor(CVector const& point, CVector const& moveSpeed)
{
    this->CEventHitByWaterCannon::CEventHitByWaterCannon(point, moveSpeed);
    return this;
}

// 0x4B1330
bool CEventHitByWaterCannon::AffectsPed(CPed* ped)
{
    return CEventHitByWaterCannon::AffectsPed_Reversed(ped);
}

bool CEventHitByWaterCannon::AffectsPed_Reversed(CPed* ped)
{
    if (ped->IsAlive()) {
        CTask* task = ped->GetTaskManager().GetActiveTask();
        if (!task || task->GetTaskType() != TASK_COMPLEX_FALL_AND_GET_UP)
            return true;
    }
    return false;
}
