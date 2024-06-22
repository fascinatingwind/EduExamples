#pragma once

namespace Chart
{
	class ChartRecord
	{
	public:
		ChartRecord() = default;
		ChartRecord(float x, float y);

		void SetX(float x);
		float GetX() const;

		void SetY(float y);
		float GetY() const;

	private:
		float m_x = 0;
		float m_y = 0;
	};
}