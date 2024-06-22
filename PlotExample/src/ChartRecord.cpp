#include "ChartRecord.h"

namespace Chart
{
	ChartRecord::ChartRecord(float x, float y)
		:m_x(x), m_y(y)
	{
	}

	void ChartRecord::SetX(float x)
	{
		m_x = x;
	}

	float ChartRecord::GetX() const
	{
		return m_x;
	}

	void ChartRecord::SetY(float y)
	{
		m_y = y;
	}

	float ChartRecord::GetY() const
	{
		return m_y;
	}
}