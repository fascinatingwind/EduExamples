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
    if (target.x() < bounds.right() && target.x() > bounds.left())
    {
      // remember display coord
      // --> x
      // |
      // |
      // v
      // y
      if (target.y() > bounds.top() || target.y() < bounds.bottom())
      {
        result.setY(target.y() - bounds.center().y());
      }
    }
    else if (target.y() > bounds.top() && target.y() < bounds.bottom())
    {
      if (target.x() > bounds.right() || target.x() < bounds.left())
      {
        result.setX(target.x() - bounds.center().x());
      }
    }
    else
    {
      result = target - bounds.center();
    }
    return result;
  }
};

#endif