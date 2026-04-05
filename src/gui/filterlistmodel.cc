#include "filterlistmodel.h"
#include "filters/contrastfilter.h"
#include "filters/gammafilter.h"
#include "filters/logfilter.h"
#include "filters/slidingwindowfilter.h"
#include <QVariantMap>

FilterListModel::FilterListModel(FilterStack &stack, QObject *parent)
    : QAbstractListModel(parent), stack_(stack) {}

int FilterListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(stack_.filters().size());
}

QVariant FilterListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= rowCount())
        return QVariant();

    const auto &filter = stack_.filters().at(index.row());
    if (role == NameRole || role == Qt::DisplayRole) {
        return QString::fromStdString(filter->name());
    } else if (role == IndexRole) {
        return index.row();
    } else if (role == ValueRole) {
        if (auto f = dynamic_cast<GammaFilter *>(filter.get())) {
            return f->gamma();
        } else if (auto f = dynamic_cast<LogFilter *>(filter.get())) {
            return f->c();
        } else if (auto f = dynamic_cast<SlidingWindowFilter *>(filter.get())) {
            return f->windowValue();
        }
    } else if (role == PointsRole) {
        if (auto contrastFilter = dynamic_cast<ContrastFilter *>(filter.get())) {
            QVariantList points;
            const auto &contrastPoints = contrastFilter->points();
            for (int i = 0; i < static_cast<int>(contrastPoints.size()); ++i) {
                QVariantMap point;
                point["index"] = i;
                point["r"] = contrastPoints[i].r;
                point["s"] = contrastPoints[i].s;
                points.append(point);
            }
            return points;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> FilterListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[IndexRole] = "index";
    roles[PointsRole] = "points";
    roles[ValueRole] = "value";
    return roles;
}

void FilterListModel::notifyFilterAdded() {
    int row = rowCount() - 1;
    beginInsertRows(QModelIndex(), row, row);
    endInsertRows();
}

void FilterListModel::notifyFilterRemoved(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    endRemoveRows();
}

void FilterListModel::notifyFiltersCleared() {
    beginResetModel();
    endResetModel();
}

void FilterListModel::notifyFilterChanged(int row) {
    if (row >= 0 && row < rowCount()) {
        QModelIndex idx = index(row);
        emit dataChanged(idx, idx, {PointsRole, ValueRole});
    }
}
