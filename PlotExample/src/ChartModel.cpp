#include "ChartModel.h"

namespace Strata
{
	static size_t ColimnCount = 2;

	ChartModel::ChartModel(QObject* parent) : QAbstractTableModel(parent)
	{
	}

	int ChartModel::rowCount(const QModelIndex& parent) const
	{
		return static_cast<int>(m_data.size());
	}

	int ChartModel::columnCount(const QModelIndex& parent) const
	{
		return static_cast<int>(ColimnCount);
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

	void ChartModel::append(const ChartRecord& record)
	{
		beginInsertRows(QModelIndex(), 0, 0);
		m_data.emplace_back(record);
		endInsertRows();
	}

	void ChartModel::append(ChartRecord&& record)
	{
		beginInsertRows(QModelIndex(), 0, 0);
		m_data.emplace_back(std::move(record));
		endInsertRows();
	}
}