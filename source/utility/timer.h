#pragma once
#include "types.h"
#include "util_time.h"

namespace util
{
	class CTimer
	{
	private:
		Time	m_prev_tick;
		Time	m_total_time;
		Time	m_delta_time;
		bool	m_started;

	public:
		CTimer(void);

		void Restart(void);
		void Signal(void);

		//Gets
		inline const Time& Delta(void) const { return m_delta_time; }
		inline const Time& Total(void) const { return m_total_time; }
	};
}
