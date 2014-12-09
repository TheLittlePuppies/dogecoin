// Copyright (c) 2011-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "analyticsModel.h"

#include "bitcoinunits.h"
#include "guiutil.h"
#include "optionsmodel.h"

AnalyticsModel::AnalyticsModel(CWallet *wallet, WalletModel *parent) :
walletModel(parent)
{

    Q_UNUSED(wallet);
    nReceiveRequestsMaxId = 0;
    /* These columns must match the indices in the ColumnIndex enumeration */
    columns << tr("Date") << tr("Amount");

}

AnalyticsModel::~AnalyticsModel()
{
    /* Intentionally left empty */
}

int AnalyticsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return list.length();
}

int AnalyticsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return columns.length();
}

QVariant AnalyticsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= list.length())
        return QVariant();

    const AnalyticsEntry *rec = &list[index.row()];

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column())
        {
            case Date:
            return rec->date.toString("MM/dd/yyyy");
            case Amount:
            if (rec->amount == 0 && role == Qt::DisplayRole)
                return tr("(no amount)");
            else

                return QString::fromStdString(rec->samount);
        }
    }
    return QVariant();
}

bool AnalyticsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true;
}

QVariant AnalyticsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole && section < columns.size())
        {
            return columns[section];
        }
    }
    return QVariant();
}

QModelIndex AnalyticsModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return createIndex(row, column);
}

Qt::ItemFlags AnalyticsModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}


// actually add to table in GUI
void AnalyticsModel::addNewRequest(AnalyticsEntry &info)
{

    beginInsertRows(QModelIndex(), 0, 0);
    list.prepend(info);
    endInsertRows();
}

void AnalyticsModel::clearData()
{

    beginRemoveRows(QModelIndex(), 0, list.size()-1);
    list.clear();
    endRemoveRows();
}

void AnalyticsModel::sort(int column, Qt::SortOrder order)
{
    qSort(list.begin(), list.end(), AnalyticsEntryLessThan(column, order));
    emit dataChanged(index(0, 0, QModelIndex()), index(list.size() - 1, NUMBER_OF_COLUMNS - 1, QModelIndex()));
}

bool AnalyticsEntryLessThan::operator()(AnalyticsEntry &left, AnalyticsEntry &right) const
{
    AnalyticsEntry *pLeft = &left;
    AnalyticsEntry *pRight = &right;
    if (order == Qt::DescendingOrder)
        std::swap(pLeft, pRight);

    switch(column)
    {
        case AnalyticsModel::Date:
        return pLeft->date.toTime_t() < pRight->date.toTime_t();
        case AnalyticsModel::Amount:
        return pLeft->amount < pRight->amount;
        default:
        return pLeft->id < pRight->id;
    }
}

