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
	void sendStringListForMainDb();
	void writeParamsSmtp();
	void sendStringListForSmtpClass();
	void writeParamMax();
	void sendStringListForMaxClass();



signals:
	void errorLog(const QString&);
	void signalFromParamsClassForConnectToMainDb(QStringList tempListSending);
	void signalFromParamsClassForSmtpWithParams(QStringList tempListSending);
	void signalFromParamsClassForMaxWithParams(QStringList tempListSending);



private:
	QSqlDatabase mainConnection;
};

