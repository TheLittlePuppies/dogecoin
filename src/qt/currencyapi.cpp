#include "currencyapi.h"
#include "../json/json_spirit.h"
#include "../json/json_spirit_reader.h"
#include <QMessageBox>


using namespace json_spirit;

CurrencyApi::CurrencyApi(QWidget *parent, QString mode) : 
	QObject(parent),
	parent(parent)
{
	// default constructor
	this->status = QString::fromStdString("waiting");
	this->currencyRateValue = 0;
	this->getResponse(mode);
}

CurrencyApi::~CurrencyApi()
{
	// default destructor
}

void CurrencyApi::getResponse(QString mode)
{
	this->status = QString::fromStdString("undefined2");
	QString url = QString::fromStdString("http://api.hitbtc.com//api/1/public/"+ mode.toStdString() +"/ticker");
	QNetworkAccessManager *manager = new QNetworkAccessManager(this->parent);
	connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getFinished(QNetworkReply*)));
	manager->get(QNetworkRequest(QUrl(url)));
}

void CurrencyApi::getFinished(QNetworkReply *reply)
{
	if(reply->error() == QNetworkReply::NoError)
    {
        QString input = reply->readAll();

        Value val;
        if(read(input.toStdString(), val))
        {
            Object root = val.get_obj();   
            Value currencyRate = find_value(root, "last");

            QString temp = QString::fromStdString(currencyRate.get_str());
            this->status = QString::fromStdString("success");
            this->currencyRateValue = temp.toFloat();
        }
    }
    else
    {
    	this->status = QString::fromStdString("failed");
    }
}