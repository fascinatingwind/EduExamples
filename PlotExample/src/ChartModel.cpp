#include "ChartModel.h"

namespace Chart
{
	ChartModel::ChartModel(QObject* parent) : QAbstractTableModel(parent)
	{
	}

	int ChartModel::rowCount(const QModelIndex& parent) const
	{
		return static_cast<int>(m_data.size());
	}

	int ChartModel::columnCount(const QModelIndex& parent) const
	{
		return 2;
	}

	QVariant ChartModel::data(const QModelIndex& index, int role) const
	{
		if (role != Qt::DisplayRole)
			return QVariant();

		const int column = static_cast<size_t>(index.column());
		if (column >= columnCount())
			return QVariant();

		const auto rowCount = static_cast<size_t>(index.row());
		if (rowCount >= m_data.size())
			return QVariant();

		const auto item = m_data.at(rowCount);

		switch (column)
		{
		case 0:
			return item.GetX();
		case 1:
			return item.GetY();
		default:
			return QVariant();
		}
	}

	void ChartModel::setModelData(const std::vector<ChartRecord>& data)
	{
		beginResetModel();
		m_data = data;
		endResetModel();
	}

	void ChartModel::setModelData(std::vector<ChartRecord>&& data)
	{
		beginResetModel();
		m_data = std::move(data);
		endResetModel();
	}
}