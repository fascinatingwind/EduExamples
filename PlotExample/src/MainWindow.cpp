// support unicode ¤
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <random>
#include <mutex>
#include <chrono>

namespace Strata
{
	using namespace std::chrono_literals;

	static bool run = false;
	static bool pause = true;
	static std::mutex mutex;

	MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
	{
		m_ui = new Ui::MainWindow();
		m_ui->setupUi(this);
		ConnectActions();

		m_chart = new QChart();

		m_value_series = new QScatterSeries();
		m_mapper_value = new QVXYModelMapper();

		m_axis_x = new QValueAxis();
		m_axis_y = new QValueAxis();

		m_model = std::make_shared<ChartModel>();
	}

	MainWindow::~MainWindow()
	{
		if(m_value_series)
			delete m_value_series;

		if(m_mapper_value)
			delete m_mapper_value;

		if(m_axis_x)
			delete m_axis_x;

		if(m_axis_y)
			delete m_axis_y;

		if(m_chart)
			delete m_chart;

		if(m_ui)
			delete m_ui;
	}

	std::vector<ChartRecord> MainWindow::GenerateData() const
	{
		std::vector<ChartRecord> data;
		std::random_device r;
		std::default_random_engine e1(r());
		std::uniform_real_distribution<float> uniform_dist(-50, 50);
		std::seed_seq seed2{ r(), r(), r(), r(), r(), r(), r(), r() };
		std::mt19937 e2(seed2);
		const size_t size = 100;

		for (auto i = 0; i < size; i++)
			data.push_back(ChartRecord(uniform_dist(e2), uniform_dist(e2)));

		return data;
	}

	void MainWindow::DrawChart()
	{
		m_ui->chartView->setChart(m_chart);
		m_chart->setAnimationOptions(QChart::AllAnimations);

		m_axis_x->setTitleText("X");
		m_axis_y->setTitleText("Y");

		m_value_series->setName("Random point");

		m_chart->addAxis(m_axis_x, Qt::AlignBottom);
		m_chart->addAxis(m_axis_y, Qt::AlignLeft);

		m_mapper_value->setXColumn(0);
		m_mapper_value->setYColumn(1);
		m_mapper_value->setModel(m_model.get());

		m_mapper_value->setSeries(m_value_series);

		m_chart->addSeries(m_value_series);
		m_value_series->attachAxis(m_axis_x);
		m_value_series->attachAxis(m_axis_y);
	}

	void MainWindow::ConnectActions() const
	{
		connect(m_ui->btnStart, &QPushButton::clicked, this, &MainWindow::Start);
		connect(m_ui->btnStop, &QPushButton::clicked, this, &MainWindow::Stop);
		connect(m_ui->btnPause, &QPushButton::clicked, this, &MainWindow::Pause);
		connect(this, &MainWindow::Draw, this, &MainWindow::DrawChart);
	}

	void MainWindow::Start()
	{
		Wait();

		if(m_background.joinable())
			m_background.join();

		run = true;
		pause = false;
		m_background = std::thread([&]() {
			while (run)
				if (!pause)
				{
					auto data = GenerateData();

					std::lock_guard<std::mutex> guard(mutex);
					m_model = std::make_shared<ChartModel>();
					if (m_model)
					{
						for (auto& item : data)
						{
							m_model->append(item);
						}
					}
					QMetaObject::invokeMethod(qApp, [&] { DrawChart(); });

					std::this_thread::sleep_for(2s);
				}

			});
	}

	void MainWindow::Pause()
	{
		pause = true;
	}

	void MainWindow::Stop()
	{
		run = false;
	}

	void MainWindow::closeEvent(QCloseEvent*)
	{
		Wait();
	}
	void MainWindow::Wait()
	{
		Stop();
		if (m_background.joinable())
			m_background.join();
	}
}