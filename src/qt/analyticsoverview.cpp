// Copyright (c) 2011-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "analyticsoverview.h"
#include "ui_analyticsoverview.h"
#include "json/json_spirit.h"
#include "addresstablemodel.h"
#include "bitcoinunits.h"
#include "coincontroldialog.h"
#include "guiutil.h"
#include "optionsmodel.h"
#include "sendcoinsentry.h"
#include "walletmodel.h"
#include "analyticsModel.h"
#include "base58.h"
#include "coincontrol.h"
#include "ui_interface.h"
#include <boost/format.hpp>
#include <QMessageBox>
#include <QScrollBar>
#include <QTextDocument>
#include <QThread>
#include <inttypes.h>
#include <stdio.h>
class StatExecutor : public QObject
{
    Q_OBJECT

    public slots:
    void request();

    signals:
    void reply(QString container);
};

#include "analyticsoverview.moc"


AnalyticsOverview::AnalyticsOverview(QWidget *parent) :
QDialog(parent),
ui(new Ui::AnalyticsOverview),
model(0)
{
    ui->setupUi(this);

    ui->comboBox_time->setFixedWidth(120);
    ui->comboBox_time->addItem(tr("Daily"), Daily);
    ui->comboBox_time->addItem(tr("Monthly"), Monthly);

    connect(ui->comboBox_time, SIGNAL(activated(int)), this, SLOT(dropdownTriggered(int)));

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(LoadBlock()));
    StartThread();

    //LoadBlock();
}

void StatExecutor::request()
{

    CStatistic statistic;


    statistic.reloadBlockForStatistics();
    map <string, float>::iterator m1_Iter;
    map<int,float> mm;
    int i=0;
    string result = "";
    for ( m1_Iter = statistic.storage.begin( ); m1_Iter != statistic.storage.end( ); m1_Iter++ )
    {
        mm[i]=m1_Iter->second;
        i++;

        std::ostringstream ss;
        ss <<fixed<< m1_Iter->second;
        std::string s(ss.str());
        result+=m1_Iter->first+","+s+"|";
    }
    CBlock block=(statistic.Latestone);
    int64_t k=0;
    int j = 0;
    BOOST_FOREACH(CTransaction & tx, block.vtx)
    {
        k+=tx.GetValueOut();
        j++;
    }
    std::ostringstream ss2;
    ss2<< result<<"last,"<<k<<","<<j<<","<<statistic.getTimeString(block.nTime);
    QString s=QString::fromStdString(ss2.str());
    emit reply(s);

}

void AnalyticsOverview::StartThread()
{
    QThread *thread = new QThread;
    StatExecutor *executor = new StatExecutor();
    executor->moveToThread(thread);
    connect(executor, SIGNAL(reply(QString)), this, SLOT(message(QString)));
    connect(this, SIGNAL(LoadBlockSignal()), executor, SLOT(request()));

    // Replies from executor object must go to this object



    // On stopExecutor signal
    // - queue executor for deletion (in execution thread)
    // - quit the Qt event loop in the execution thread
    connect(this, SIGNAL(stopExecutor()), executor, SLOT(deleteLater()));
    connect(this, SIGNAL(stopExecutor()), thread, SLOT(quit()));
    // Queue the thread for deletion (in this thread) when it is finished
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // Default implementation of QThread::run() simply spins up an event loop in the thread,
    // which is what we want.
    thread->start();
}

void AnalyticsOverview::LoadBlock()
{

    QPushButton * senderButton = qobject_cast<QPushButton *>(this->sender());
    senderButton->setEnabled(false);
    senderButton->setText(QString("Loading Blocks"));


    emit LoadBlockSignal();



}
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

int64_t S64(const char *s) {
  int64_t i;
  char c ;
  int scanned = sscanf(s, "%" SCNd64 "%c", &i, &c);
  if (scanned == 1) return i;
  if (scanned > 1) {
    // TBD about extra data found
    return i;
    }
  // TBD failed to scan;
  return 0;
}

void AnalyticsOverview::reload()
{

    model->getAnalyticsModel()->clearData();
    QMap<QDateTime, float>::iterator m1_Iter;
    if(ui->comboBox_time->currentIndex()==0){
        for ( m1_Iter = this->storeMap.begin( ); m1_Iter != this->storeMap.end( ); m1_Iter++ )
        {
            AnalyticsEntry info;
            float fCoin = m1_Iter.value();
            info.date=m1_Iter.key();
            info.amount=fCoin;
            std::ostringstream ss;
            ss <<fixed<< fCoin;
            std::string s(ss.str());
            info.samount= s;
            model->getAnalyticsModel()->addNewRequest(info);
        }
    }else
    {
        QMap<QString,float> yearMap;
        QMap<QString, float>::iterator m2_Iter;
        for ( m1_Iter = this->storeMap.begin( ); m1_Iter != this->storeMap.end( ); m1_Iter++ )
        {

            if(yearMap.contains(m1_Iter.key().toString("MM/01/yyyy")))
            {
                yearMap[m1_Iter.key().toString("MM/01/yyyy")]+=m1_Iter.value();
            }else
            {
                yearMap[m1_Iter.key().toString("MM/01/yyyy")]=m1_Iter.value();
            }
        }
        for ( m2_Iter = yearMap.begin( ); m2_Iter != yearMap.end( ); m2_Iter++ )
        {
            AnalyticsEntry info;
            float fCoin = m2_Iter.value();
            info.date=QDateTime::fromString(m2_Iter.key(),"MM/dd/yyyy");
            info.amount=fCoin;
            std::ostringstream ss;
            ss <<fixed<< fCoin;
            std::string s(ss.str());
            info.samount= s;
            model->getAnalyticsModel()->addNewRequest(info);
        }

    }
}

void AnalyticsOverview::message(QString stringContainer)
{
    // statistic = statModel;


    // AnalyticsEntry info;
    // //info.date=QDateTime(QDate(current.year(), current.month()-1, 1))
    // info.date=QDateTime(QDate(2014, 11, 1));
    // info.amount=100000;
    // model->getAnalyticsModel()->addNewRequest(info);

    string result =stringContainer.toUtf8().constData();
    vector<string> ivector = split(result,'|');
    vector<string> blockVector;
    for(std::vector<int>::size_type i = 0; i < ivector.size(); i++) {
        vector<string> jvector = split(ivector[i],',');
        if(jvector[0]!="last")
        {
            this->storeMap[QDateTime::fromString(jvector[0].c_str(),"MM/dd/yyyy")]
            =atof(jvector[1].c_str())/COIN;
        }else
        {
            blockVector=jvector;
        }

    }




    int64_t k=S64(blockVector[1].c_str());

    string valueOut= boost::str(boost::format("%d.%08d") % (k/COIN) % (k%COIN));
    string transactions = blockVector[2];;
    string date = blockVector[3];
    ui->BlockDisplay->setText(QString::fromStdString(date));
    ui->TransactionDisplay->setText(QString::fromStdString(transactions));
    ui->ValueOutDisplay->setText(QString::fromStdString(valueOut));
    ui->pushButton->setEnabled(true);
    ui->pushButton->setText(QString("Load Blocks"));
    ui->label->setText("Last sync time:"+QDateTime::currentDateTime().toString("MM/dd/yyyy HH:mm:ss"));
    reload();
}
void AnalyticsOverview::setModel(WalletModel *model)
{
    this->model = model;

    if(model && model->getOptionsModel())
    {
        QTableView* tableView = ui->statisticsTableView;

        // tableView->verticalHeader()->hide();
        // tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView->setModel(model->getAnalyticsModel());

        // tableView->setAlternatingRowColors(true);
        // tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        // tableView->setSelectionMode(QAbstractItemView::ContiguousSelection);
        // tableView->setColumnWidth(AnalyticsModel::Date, DATE_COLUMN_WIDTH);
    }
}

AnalyticsOverview::~AnalyticsOverview()
{
    delete ui;
}

void AnalyticsOverview::dropdownTriggered(int index)
{
    reload();
}



