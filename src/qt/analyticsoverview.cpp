// Copyright (c) 2011-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "analyticsoverview.h"
#include "ui_analyticsoverview.h"

#include "addresstablemodel.h"
#include "bitcoinunits.h"
#include "coincontroldialog.h"
#include "guiutil.h"
#include "optionsmodel.h"
#include "sendcoinsentry.h"
#include "walletmodel.h"

#include "base58.h"
#include "coincontrol.h"
#include "ui_interface.h"

#include <QMessageBox>
#include <QScrollBar>
#include <QTextDocument>

AnalyticsOverview::AnalyticsOverview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyticsOverview),
    model(0)
{
    ui->setupUi(this);

#ifdef Q_OS_MAC // Icons on push buttons are very uncommon on Mac
    // ui->addButton->setIcon(QIcon());
    // ui->clearButton->setIcon(QIcon());
    // ui->sendButton->setIcon(QIcon());
#endif

    // GUIUtil::setupAddressWidget(ui->lineEditCoinControlChange, this);

    //addEntry();

    //connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
    //connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clear()));

    // Coin Control
    //connect(ui->pushButtonCoinControl, SIGNAL(clicked()), this, SLOT(coinControlButtonClicked()));
    //connect(ui->checkBoxCoinControlChange, SIGNAL(stateChanged(int)), this, SLOT(coinControlChangeChecked(int)));
    //connect(ui->lineEditCoinControlChange, SIGNAL(textEdited(const QString &)), this, SLOT(coinControlChangeEdited(const QString &)));

    // Coin Control: clipboard actions
    // QAction *clipboardQuantityAction = new QAction(tr("Copy quantity"), this);
    // QAction *clipboardAmountAction = new QAction(tr("Copy amount"), this);
    // QAction *clipboardFeeAction = new QAction(tr("Copy fee"), this);
    // QAction *clipboardAfterFeeAction = new QAction(tr("Copy after fee"), this);
    // QAction *clipboardBytesAction = new QAction(tr("Copy bytes"), this);
    // QAction *clipboardPriorityAction = new QAction(tr("Copy priority"), this);
    // QAction *clipboardLowOutputAction = new QAction(tr("Copy low output"), this);
    // QAction *clipboardChangeAction = new QAction(tr("Copy change"), this);
    // connect(clipboardQuantityAction, SIGNAL(triggered()), this, SLOT(coinControlClipboardQuantity()));
    // connect(clipboardAmountAction, SIGNAL(triggered()), this, SLOT(coinControlClipboardAmount()));
    // connect(clipboardFeeAction, SIGNAL(triggered()), this, SLOT(coinControlClipboardFee()));
    // connect(clipboardAfterFeeAction, SIGNAL(triggered()), this, SLOT(coinControlClipboardAfterFee()));
    // connect(clipboardBytesAction, SIGNAL(triggered()), this, SLOT(coinControlClipboardBytes()));
    // connect(clipboardPriorityAction, SIGNAL(triggered()), this, SLOT(coinControlClipboardPriority()));
    // connect(clipboardLowOutputAction, SIGNAL(triggered()), this, SLOT(coinControlClipboardLowOutput()));
    // connect(clipboardChangeAction, SIGNAL(triggered()), this, SLOT(coinControlClipboardChange()));
    // ui->labelCoinControlQuantity->addAction(clipboardQuantityAction);
    // ui->labelCoinControlAmount->addAction(clipboardAmountAction);
    // ui->labelCoinControlFee->addAction(clipboardFeeAction);
    // ui->labelCoinControlAfterFee->addAction(clipboardAfterFeeAction);
    // ui->labelCoinControlBytes->addAction(clipboardBytesAction);
    // ui->labelCoinControlPriority->addAction(clipboardPriorityAction);
    // ui->labelCoinControlLowOutput->addAction(clipboardLowOutputAction);
    // ui->labelCoinControlChange->addAction(clipboardChangeAction);

    // fNewRecipientAllowed = true;

    ui->comboBox_time->setFixedWidth(120);
    ui->comboBox_time->addItem(tr("Daily"), Daily);
    ui->comboBox_time->addItem(tr("Monthly"), Monthly);

    connect(ui->comboBox_time, SIGNAL(activated(int)), this, SLOT(dropdownTriggered(int)));
}

void AnalyticsOverview::setModel(WalletModel *model)
{
    this->model = model;

    // if(model && model->getOptionsModel())
    // {
    //     for(int i = 0; i < ui->entries->count(); ++i)
    //     {
    //         SendCoinsEntry *entry = qobject_cast<SendCoinsEntry*>(ui->entries->itemAt(i)->widget());
    //         if(entry)
    //         {
    //             entry->setModel(model);
    //         }
    //     }

    //     setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance());
    //     connect(model, SIGNAL(balanceChanged(qint64, qint64, qint64)), this, SLOT(setBalance(qint64, qint64, qint64)));
    //     connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));

    //     // Coin Control
    //     connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(coinControlUpdateLabels()));
    //     connect(model->getOptionsModel(), SIGNAL(coinControlFeaturesChanged(bool)), this, SLOT(coinControlFeatureChanged(bool)));
    //     connect(model->getOptionsModel(), SIGNAL(transactionFeeChanged(qint64)), this, SLOT(coinControlUpdateLabels()));
    //     ui->frameCoinControl->setVisible(model->getOptionsModel()->getCoinControlFeatures());
    //     coinControlUpdateLabels();
    // }
}

AnalyticsOverview::~AnalyticsOverview()
{
    delete ui;
}

void AnalyticsOverview::dropdownTriggered(int index)
{
    QMessageBox test;
    test.setText("Changed!");
    test.exec();
}
