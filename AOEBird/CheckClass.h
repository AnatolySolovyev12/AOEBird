#pragma once

#include <QObject>
#include <QTimer>
#include <QDebug>;
#include <QDate>

class CheckClass  : public QObject
{
	Q_OBJECT

public:
	CheckClass(QObject *parent);
	~CheckClass();

	void checkValuesFromDb(QStringList temp);

signals:
	void checkDbForEvent();
	void sendMax(QString phoneNumber, QString messege);
	void sendTelegram(QString messege);
	void sendMail(QString eMail, QString messege, QString files);
	void sendSMS(QString phoneNumber, QString messege);
	void deleteInDbSignal(QString id, QString request, QString position);



private:
	QTimer* checkTimer = nullptr;
};

