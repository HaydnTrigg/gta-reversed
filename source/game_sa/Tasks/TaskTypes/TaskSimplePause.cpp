#include "StdInc.h"

#include "TaskSimplePause.h"

void CTaskSimplePause::InjectHooks()
{
    ReversibleHooks::Install("CTaskSimplePause", "CTaskSimplePause", 0x48E750, &CTaskSimplePause::Constructor);
    ReversibleHooks::Install("CTaskSimplePause", "ProcessPed", 0x48E830, &CTaskSimplePause::ProcessPed_Reversed);
    ReversibleHooks::Install("CTaskSimplePause", "MakeAbortable", 0x48E810, &CTaskSimplePause::MakeAbortable_Reversed);
}

// 0x48E750
CTaskSimplePause::CTaskSimplePause(int32 time)
{
    m_nTime = time;
}

CTaskSimplePause* CTaskSimplePause::Constructor(int32 time)
{
    this->CTaskSimplePause::CTaskSimplePause(time);
    return this;
}

CTask* CTaskSimplePause::Clone()
{
    return plugin::CallMethodAndReturn<CTask*, 0x48E780, CTask*>(this);
}

bool CTaskSimplePause::MakeAbortable_Reversed(CPed* ped, eAbortPriority priority, const CEvent* event)
{
    m_timer.m_nStartTime = CTimer::GetTimeInMS();
    m_timer.m_nInterval = -1;
    m_timer.m_bStarted = true;
    return true;
}

// 0x48E810
bool CTaskSimplePause::MakeAbortable(CPed* ped, eAbortPriority priority, const CEvent* event)
{
    return CTaskSimplePause::MakeAbortable_Reversed(ped, priority, event);
}

bool CTaskSimplePause::ProcessPed_Reversed(CPed* ped)
{
    if (!m_timer.m_bStarted && !m_timer.Start(m_nTime)) {
        return false;
    }
    m_timer.Reset();
    return m_timer.IsOutOfTime();
}

// 0x48E830
bool CTaskSimplePause::ProcessPed(CPed* ped)
{
    return CTaskSimplePause::ProcessPed_Reversed(ped);
}
