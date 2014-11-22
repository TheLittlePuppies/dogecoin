// Copyright (c) 2011-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ANALYTICSOVERVIEW_H
#define ANALYTICSOVERVIEW_H

#include "walletmodel.h"

#include <QDialog>
#include <QString>

class OptionsModel;
class SendCoinsEntry;
class SendCoinsRecipient;

QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

namespace Ui {
    class AnalyticsOverview;
}

/** Dialog for sending bitcoins */
class AnalyticsOverview : public QDialog
{
    Q_OBJECT

public:

    enum TimeEnum
    {
        Daily,
        Monthly
    };

    explicit AnalyticsOverview(QWidget *parent = 0);
    ~AnalyticsOverview();

    void setModel(WalletModel *model);


public slots:
    

private:
    Ui::AnalyticsOverview *ui;
    WalletModel *model;
    bool fNewRecipientAllowed;

private slots:
    void dropdownTriggered(int index);

signals:
    // Fired when a message should be reported to the user
    // void message(const QString &title, const QString &message, unsigned int style);
};

#endif // ANALYTICSOVERVIEW_H
