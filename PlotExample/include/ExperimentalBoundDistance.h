#pragma once
#ifndef EXPERIMENTAL_BOUND_DISTANCE_H
#define EXPERIMENTAL_BOUND_DISTANCE_H

#include "BoundDistanceInterface.h"

template <typename T> int sign(T val)
{
  return (T(0) < val) - (val < T(0));
}

class ExperimentalBoundDistance final : public BoundDistanceInterface
{
public:
  ExperimentalBoundDistance() = default;
  ~ExperimentalBoundDistance() = default;

  QPointF distance(const QPointF& target, const QRectF& bounds) const override
  {
    const auto moved = target - bounds.center();
    return
    {
      moved.x() - bounds.width() / 2 * sign(moved.x()),
      moved.y() - bounds.height() / 2 * sign(moved.y())
    };
  }
};
#endif