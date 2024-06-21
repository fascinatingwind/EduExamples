// support unicode ¤
#pragma once
#include <QtWidgets/QMainWindow>
#include <QtCharts/qchart.h>
#include <QtCharts/qvxymodelmapper.h>
#include <QtCharts/qvalueaxis.h>
#include <QtCharts/qscatterseries.h>

#include <thread>

#include "ChartModel.h"

namespace Ui
{
	class MainWindow;
}

namespace Strata
{
	class MainWindow : public QMainWindow
	{
		Q_OBJECT;
	public:
		MainWindow(QWidget* parent = Q_NULLPTR);
		~MainWindow();

	private:
		Ui::MainWindow* m_ui = nullptr;
		ChartModelPtr m_model = nullptr;

		QChart* m_chart = nullptr;

		QVXYModelMapper* m_mapper_value = nullptr;
		QValueAxis* m_axis_x = nullptr;
		QValueAxis* m_axis_y = nullptr;

		QScatterSeries* m_value_series = nullptr;

		std::vector<ChartRecord> GenerateData() const;

		void DrawChart();

		void ConnectActions() const;

		void Start();
		void Pause();
		void Stop();

		void closeEvent(QCloseEvent*);

		void Wait();

		std::thread m_background;

	signals:
		void Draw();
	};
}
