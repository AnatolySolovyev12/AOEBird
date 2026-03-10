#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>;
#include <QFile>
#include <qsqlerror>
#include <QSqlQuery>
#include <iostream>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <qtimer>
#include <QString>

class ParamsClass  : public QObject
{
	Q_OBJECT

public:
	ParamsClass(QObject* parent);
	~ParamsClass();

	bool createOrConnectParamsBd();
	bool createTable();
	void writeFirstParamsDb();
	std::string validateHost(std::string tempHost);
	std::string validatePort(std::string tempPort);
	std::string validateText(std::string tempString);
	bool sendStringListForMainDb();
	void writeParamsSmtp();
	bool sendStringListForSmtpClass();
	void writeParamMax();
	bool sendStringListForMaxClass();
	void writeParamTg();
	bool sendStringListForTelegramClass();
	void writeParamSms();
	bool sendStringListForSmsClass();



signals:
	void errorLog(const QString&);
	void signalFromParamsClassForConnectToMainDb(QStringList tempListSending);
	void signalFromParamsClassForSmtpWithParams(QStringList tempListSending);
	void signalFromParamsClassForMaxWithParams(QStringList tempListSending);
	void signalFromParamsClassForTelegramWithParams(QStringList tempListSending);
	void signalFromParamsClassForStartCheckClass(bool max, bool tg, bool mail, bool sms);
	void signalFromParamsClassForSmsClassWithParams(QStringList tempListSending);





private:
	QSqlDatabase mainConnection;
	bool readySQLite = false;
	bool readyPostreSQL = false;
	bool readyMax = false;
	bool readyTelegram = false;
	bool readyMail = false;
	bool readySms = false;

};

