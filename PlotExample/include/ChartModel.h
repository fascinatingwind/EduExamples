#pragma once
#include <qabstractitemmodel.h>
#include <QtCharts/qlineseries.h>

#include "ChartRecord.h"

namespace Chart
{
	class ChartModel final : public QAbstractTableModel
	{
	public:
		explicit ChartModel(QObject* parent = nullptr);

		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;

		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		void setModelData(const std::vector<ChartRecord>& data);
		void setModelData(std::vector<ChartRecord>&& data);

	private:
		std::vector<ChartRecord> m_data;
	};
}