#include "timer.h"
#include <assert.h>

namespace util
{
	CTimer::CTimer(void)
		: m_delta_time(0)
		, m_total_time(0)
		, m_prev_tick(0)
		, m_started(false)
	{
	}

	void CTimer::Restart(void)
	{
		m_started = true;
		m_delta_time = 0;
		m_total_time = 0;
		m_prev_tick = Time::GetTimeSinceEpoch();
	}

	void CTimer::Signal(void)
	{
		assert(m_started && "CTime::Signal - timer has not been started");
		const Time now = Time::GetTimeSinceEpoch();
		m_delta_time = now - m_prev_tick;
		m_total_time += m_delta_time;
		m_prev_tick = now;
	}
}