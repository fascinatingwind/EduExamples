// support unicode ¤
#include "ChartWindow.h"
#include "ui_ChartWindow.h"

#include <random>
#include <chrono>

#include <QChart>

#include "DefaultBoundDistance.h"
#include "ExperimentalBoundDistance.h"

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
    delete m_ui;
  }

  std::vector<QPointF> ChartWindow::GenerateData() const
  {
    std::vector<QPointF> data(100);
    std::random_device device;
    std::uniform_real_distribution<double> uniform_dist(-50, 50);
    std::seed_seq seed2{ device(), device(), device(), device(), device(), device(), device(), device() };
    std::mt19937 engine_mt(seed2);

    std::generate(data.begin(), data.end(), [&]() -> QPointF
      {
        return QPointF(uniform_dist(engine_mt), uniform_dist(engine_mt));
      });
    return data;
  }

  void ChartWindow::SetupUi()
  {
    m_ui = new Ui::ChartWindow();
    m_ui->setupUi(this);

    auto chart = m_ui->chartView->chart();
    chart->setAnimationOptions(QChart::AllAnimations);

    m_axis_x = new QValueAxis(chart);
    m_axis_x->setTitleText("X");
    m_axis_y = new QValueAxis(chart);
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

    m_center_series = new QScatterSeries();
    m_center_series->setName("Center dot");
    m_center_series->setColor(Qt::red);

    m_bound_rect = new QLineSeries();
    m_bound_rect->setName("Center bounds");

    chart->addSeries(m_value_series);
    chart->addSeries(m_center_series);
    chart->addSeries(m_bound_rect);

    m_value_series->attachAxis(m_axis_x);
    m_value_series->attachAxis(m_axis_y);
    m_center_series->attachAxis(m_axis_x);
    m_center_series->attachAxis(m_axis_y);
    m_bound_rect->attachAxis(m_axis_x);
    m_bound_rect->attachAxis(m_axis_y);

    m_bound_distance = std::make_unique<DefaultBoundDistance>();
    //m_bound_distance = std::make_unique<ExperimentalBoundDistance>();
  }

  void ChartWindow::ConnectActions() const
  {
    connect(m_ui->btnStart, &QPushButton::clicked, this, &ChartWindow::Start);
    connect(m_ui->btnStop, &QPushButton::clicked, this, &ChartWindow::Stop);
    connect(m_ui->btnPause, &QPushButton::clicked, this, &ChartWindow::Pause);
    connect(m_ui->btnResize, &QPushButton::clicked, this, &ChartWindow::Resize);
    connect(m_value_series, &QScatterSeries::clicked, this, &ChartWindow::pointClicked);
    connect(m_model, &QAbstractItemModel::modelReset, this, &ChartWindow::redraw);
    connect(this, &ChartWindow::redraw, this, &ChartWindow::drawCenter);
    connect(this, &ChartWindow::redraw, this, &ChartWindow::drawRectangle);
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
          if (!m_pause)
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

  void ChartWindow::Resize()
  {
    auto points = m_value_series->points();
    auto xcomp = [](const auto& l, const auto& r)
      {
        return l.x() < r.x();
      };
    auto ycomp = [](const auto& l, const auto& r)
      {
        return l.y() < r.y();
      };
    auto bottom = *std::min_element(points.begin(), points.end(), ycomp);
    auto top = *std::max_element(points.begin(), points.end(), ycomp);
    auto left = *std::min_element(points.begin(), points.end(), xcomp);
    auto right = *std::max_element(points.begin(), points.end(), xcomp);
    m_axis_x->setRange(left.x() - std::abs(left.x()) / 10, right.x() + std::abs(right.x()) / 10);
    m_axis_y->setRange(bottom.y() - std::abs(bottom.y()) / 10, top.y() + std::abs(top.y() / 10));
    emit redraw();
  }

  void ChartWindow::closeEvent(QCloseEvent*)
  {
    Stop();
  }

  void ChartWindow::pointClicked(const QPointF& point)
  {
    auto chart = m_ui->chartView->chart();
    const auto positionPoint = chart->mapToPosition(point);
    const auto distance = m_bound_distance->distance(positionPoint, m_bounds);
    chart->scroll(distance.x(), -distance.y());
    emit redraw();
  }

  void ChartWindow::drawCenter()
  {
    m_center_series->clear();
    m_center_series->append(getAxisValueCenter());
  }

  void ChartWindow::drawRectangle()
  {
    m_bound_rect->clear();
    const auto chart = m_ui->chartView->chart();
    m_bounds.moveCenter(getViewPortCenter());
    m_bound_rect->append(chart->mapToValue(m_bounds.bottomLeft()));
    m_bound_rect->append(chart->mapToValue(m_bounds.bottomRight()));
    m_bound_rect->append(chart->mapToValue(m_bounds.topRight()));
    m_bound_rect->append(chart->mapToValue(m_bounds.topLeft()));
    m_bound_rect->append(chart->mapToValue(m_bounds.bottomLeft()));
  }

  QPointF ChartWindow::getViewPortCenter() const
  {
    const auto viewPortCenter = m_ui->chartView->viewport()->size() / 2;
    return
    {
      static_cast<double>(viewPortCenter.width()) ,
      static_cast<double>(viewPortCenter.height())
    };
  }

  QPointF ChartWindow::getAxisValueCenter() const
  {
    return m_ui->chartView->chart()->mapToValue(getViewPortCenter());
  }
}
