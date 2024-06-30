// support unicode ¤
#pragma once
#include <QMainWindow>
#include <QVXYModelMapper>
#include <QValueAxis>
#include <QScatterSeries>
#include <QPointer>

#include <thread>
#include <mutex>

#include "ChartModel.h"
#include "BoundDistanceInterface.h"

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
    QPointer<ChartModel> m_model = nullptr;

    QPointer<QVXYModelMapper> m_mapper_value = nullptr;
    QPointer<QValueAxis> m_axis_x = nullptr;
    QPointer<QValueAxis> m_axis_y = nullptr;

    QPointer<QScatterSeries> m_value_series = nullptr;
    QPointer<QScatterSeries> m_center_series = nullptr;

    QPointer<QLineSeries> m_bound_rect = nullptr;

    std::unique_ptr<BoundDistanceInterface> m_bound_distance = nullptr;

    std::mutex m_mutex;
    std::jthread m_background;

    std::atomic<bool> m_run = false;
    std::atomic<bool> m_pause = false;

    int m_rect_length = 10;
    QRectF m_bounds{ 0, 0, 150, 150 };

  signals:
    void redraw();

  private:
    void SetupUi();
    void ConnectActions() const;

    void Start();
    void Pause();
    void Stop();
    void Resize();
    void closeEvent(QCloseEvent*);
    void pointClicked(const QPointF& point);
    void drawCenter();
    void drawRectangle();

    QPointF getViewPortCenter() const;
    QPointF getAxisValueCenter() const;
    std::vector<QPointF> GenerateData() const;
  };
}
