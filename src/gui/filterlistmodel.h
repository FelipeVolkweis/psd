#ifndef FILTERLISTMODEL_H
#define FILTERLISTMODEL_H

#include "filters/filterstack.h"
#include <QAbstractListModel>

/**
 * @brief Modelo de lista para gerenciar os filtros aplicados à imagem.
 */
class FilterListModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum FilterRoles { NameRole = Qt::UserRole + 1, IndexRole, PointsRole, ValueRole };

    /**
     * @brief Cria o modleo vinculando a uma pilha de filtros.
     */
    explicit FilterListModel(FilterStack &stack, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void notifyFilterAdded();
    void notifyFilterRemoved(int row);
    void notifyFiltersCleared();
    void notifyFilterChanged(int row);

private:
    FilterStack &stack_;
};

#endif
