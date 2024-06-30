#pragma once
#ifndef BOUND_DISTANCE_INTERFACE_H
#define BOUND_DISTANCE_INTERFACE_H
#include <QRectF>

class BoundDistanceInterface
{
public:
  BoundDistanceInterface() = default;
  virtual ~BoundDistanceInterface() = default;

  virtual QPointF distance(const QPointF& target, const QRectF& bounds) const = 0;
};
#endif