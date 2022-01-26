#include "StdInc.h"

void CEventDeath::InjectHooks()
{
    ReversibleHooks::Install("CEventDeath", "Constructor", 0x4ADDF0, &CEventDeath::Constructor);
    ReversibleHooks::Install("CEventDeath", "Clone_Reversed", 0x4B6E30, &CEventDeath::Clone_Reversed);
}

CEventDeath::CEventDeath(bool bDrowning, uint32 deathTimeInMs)
{
    m_bDrowning = bDrowning;
    m_deathTimeInMs = deathTimeInMs;
}

// // 0x4ADDF0
CEventDeath::CEventDeath(bool bDrowning)
{
    m_bDrowning = bDrowning;
    m_deathTimeInMs = CTimer::GetTimeInMS();
}

CEventDeath::~CEventDeath()
{
    // nothing here
}

CEventDeath* CEventDeath::Constructor(bool bDrowning)
{
    this->CEventDeath::CEventDeath(bDrowning);
    return this;
}

// 0x4B6E30
CEvent* CEventDeath::Clone()
{
    return CEventDeath::Clone_Reversed();
}

CEvent* CEventDeath::Clone_Reversed()
{
    return new CEventDeath(m_bDrowning, m_deathTimeInMs);
}
