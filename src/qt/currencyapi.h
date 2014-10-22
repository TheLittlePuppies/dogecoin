#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

class CurrencyApi : public QObject
{
	Q_OBJECT

public:
	QString status;
	float currencyRateValue;

	CurrencyApi(QWidget *parent, QString mode);
	~CurrencyApi();

	void getResponse(QString mode);

public slots:
	void getFinished(QNetworkReply *reply);

private:
	QWidget *parent;
};