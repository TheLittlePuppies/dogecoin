// Copyright (c) 2011-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ANALYTICSMODEL_H
#define ANALYTICSMODEL_H

#include "walletmodel.h"

#include <QAbstractTableModel>
#include <QStringList>
#include <QDateTime>

class CWallet;

class AnalyticsEntry
{
public:
    AnalyticsEntry() : nVersion(AnalyticsEntry::CURRENT_VERSION), id(0) { }

    static const int CURRENT_VERSION = 1;
    int nVersion;
    int64_t id;
    QDateTime date;
    float amount;
    string samount;
    IMPLEMENT_SERIALIZE
    (
        AnalyticsEntry* pthis = const_cast<AnalyticsEntry*>(this);

        unsigned int nDate = date.toTime_t();

        READWRITE(pthis->nVersion);
        nVersion = pthis->nVersion;
        READWRITE(id);
        READWRITE(nDate);
        READWRITE(amount);
        READWRITE(samount);
        if (fRead)
            pthis->date = QDateTime::fromTime_t(nDate);
    )
};

class AnalyticsEntryLessThan
{
public:
    AnalyticsEntryLessThan(int nColumn, Qt::SortOrder fOrder):
        column(nColumn), order(fOrder) {}
    bool operator()(AnalyticsEntry &left, AnalyticsEntry &right) const;

private:
    int column;
    Qt::SortOrder order;
};

/** Model for list of recently generated payment requests / dogecoin: URIs.
 * Part of wallet model.
 */
class AnalyticsModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit AnalyticsModel(CWallet *wallet, WalletModel *parent);
    ~AnalyticsModel();

    enum ColumnIndex {
        Date = 0,
        Amount = 1,
        NUMBER_OF_COLUMNS
    };
    void addNewRequest(AnalyticsEntry &info);
    void clearData();
    /** @name Methods overridden from QAbstractTableModel
        @{*/
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    // bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Qt::ItemFlags flags(const QModelIndex &index) const;
    /*@}*/

    const AnalyticsEntry &entry(int row) const { return list[row]; }

public slots:
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

private:
    WalletModel *walletModel;
    QStringList columns;
    QList<AnalyticsEntry> list;
    int64_t nReceiveRequestsMaxId;
};

#endif
