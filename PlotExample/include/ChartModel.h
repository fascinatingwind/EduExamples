#pragma once
#include <qabstractitemmodel.h>
#include <QtCharts/qlineseries.h>

#include "ChartRecord.h"

namespace Strata
{
	class ChartModel final : public QAbstractTableModel
	{
	public:
		ChartModel(QObject* parent = nullptr);

		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;

		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		void append(const ChartRecord& record);
		void append(ChartRecord&& record);

	private:
		std::vector<ChartRecord> m_data;
	};
	using ChartModelPtr = std::shared_ptr<ChartModel>;
}