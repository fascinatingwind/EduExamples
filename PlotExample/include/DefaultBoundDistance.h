#pragma once
#ifndef DEFAULT_BOUND_DISTANCE_H
#define DEFAULT_BOUND_DISTANCE_H

#include "BoundDistanceInterface.h"

class DefaultBoundDistance final : public BoundDistanceInterface
{
public:
  DefaultBoundDistance() = default;
  ~DefaultBoundDistance() = default;

  QPointF distance(const QPointF& target, const QRectF& bounds) const override
  {
    QPointF result{ 0, 0 };
    if (bounds.contains(target))
      return result;
    result = target - bounds.center();
    if (target.x() < bounds.right() && target.x() > bounds.left())
    {
      if (target.y() > bounds.top() || target.y() < bounds.bottom())
      {
        result.setX(0);
      }
    }
    else if (target.y() > bounds.top() && target.y() < bounds.bottom())
    {
      if (target.x() > bounds.right() || target.x() < bounds.left())
      {
        result.setY(0);
      }
    }
    return result;
  }
};

#endif