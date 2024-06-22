// support unicode ¤
#include "ChartWindow.h"
#include "ui_ChartWindow.h"

#include <random>
#include <chrono>

namespace Chart
{
  using namespace std::chrono_literals;

  ChartWindow::ChartWindow(QWidget* parent) : QMainWindow(parent)
  {
    SetupUi();
    ConnectActions();
  }

  ChartWindow::~ChartWindow()
  {
    if (m_value_series)
      delete m_value_series;

    if (m_mapper_value)
      delete m_mapper_value;

    if (m_axis_x)
      delete m_axis_x;

    if (m_axis_y)
      delete m_axis_y;

    if (m_ui)
      delete m_ui;
  }

  std::vector<ChartRecord> ChartWindow::GenerateData() const
  {
    std::vector<ChartRecord> data(100);
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<float> uniform_dist(-50, 50);
    std::seed_seq seed2{ r(), r(), r(), r(), r(), r(), r(), r() };
    std::mt19937 e2(seed2);

    std::generate(data.begin(), data.end(), [&]() -> ChartRecord
      {
        return ChartRecord(uniform_dist(e2), uniform_dist(e2));
      });
    return data;
  }

  void ChartWindow::SetupUi()
  {
    m_ui = new Ui::ChartWindow();
    m_ui->setupUi(this);

    auto chart = m_ui->chartView->chart();
    chart->setAnimationOptions(QChart::AllAnimations);

    m_axis_x = new QValueAxis();
    m_axis_x->setTitleText("X");

    m_axis_y = new QValueAxis();
    m_axis_y->setTitleText("Y");

    m_value_series = new QScatterSeries();
    m_value_series->setName("Random point");

    chart->addAxis(m_axis_x, Qt::AlignBottom);
    chart->addAxis(m_axis_y, Qt::AlignLeft);

    m_mapper_value = new QVXYModelMapper();
    m_mapper_value->setXColumn(0);
    m_mapper_value->setYColumn(1);
    m_model = new ChartModel();
    m_model->setModelData(GenerateData());
    m_mapper_value->setModel(m_model);

    m_mapper_value->setSeries(m_value_series);

    chart->addSeries(m_value_series);
    m_value_series->attachAxis(m_axis_x);
    m_value_series->attachAxis(m_axis_y);
  }

  void ChartWindow::ConnectActions() const
  {
    connect(m_ui->btnStart, &QPushButton::clicked, this, &ChartWindow::Start);
    connect(m_ui->btnStop, &QPushButton::clicked, this, &ChartWindow::Stop);
    connect(m_ui->btnPause, &QPushButton::clicked, this, &ChartWindow::Pause);
  }

  void ChartWindow::Start()
  {
    if (m_run && !m_pause)
    {
      m_pause = false;
      return;
    }

    m_run = false;
    if (m_background.joinable())
      m_background.join();
    m_run = true;
    m_pause = false;
    m_background = std::jthread([&]()
      {
        while (m_run)
        {
          if(!m_pause)
          {
            std::lock_guard guard(m_mutex);
            m_model->setModelData(GenerateData());
          }
          std::this_thread::sleep_for(2s);
        }
      });
  }

  void ChartWindow::Pause()
  {
    m_pause = true;
  }

  void ChartWindow::Stop()
  {
    m_run = false;
  }

  void ChartWindow::closeEvent(QCloseEvent*)
  {
    Stop();
  }
}