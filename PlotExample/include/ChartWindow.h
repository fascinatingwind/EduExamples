// support unicode ¤
#pragma once
#include <QtWidgets/QMainWindow>
#include <QtCharts/qchart.h>
#include <QtCharts/qvxymodelmapper.h>
#include <QtCharts/qvalueaxis.h>
#include <QtCharts/qscatterseries.h>

#include <thread>
#include <mutex>

#include "ChartModel.h"
#include "ChartRecord.h"

namespace Ui
{
	class ChartWindow;
}

namespace Chart
{
	class ChartWindow final : public QMainWindow
	{
		Q_OBJECT;
	public:
		explicit ChartWindow(QWidget* parent = Q_NULLPTR);
		~ChartWindow();

	private:
		Ui::ChartWindow* m_ui = nullptr;
		ChartModel* m_model = nullptr;

		QVXYModelMapper* m_mapper_value = nullptr;
		QValueAxis* m_axis_x = nullptr;
		QValueAxis* m_axis_y = nullptr;

		QScatterSeries* m_value_series = nullptr;
		std::jthread m_background;

		bool m_run = false;
		bool m_pause = true;

	private:
		void SetupUi();
		void ConnectActions() const;

		void Start();
		void Pause();
		void Stop();

		void closeEvent(QCloseEvent*);

		std::vector<ChartRecord> GenerateData() const;
	};
}
