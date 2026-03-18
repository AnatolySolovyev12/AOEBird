#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>;
#include <QFile>
#include <qsqlerror>
#include <QSqlQuery>
#include <iostream>
#include <QDate>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class DataBaseClass  : public QObject
{
	Q_OBJECT

public:
	DataBaseClass(QObject *parent);
	~DataBaseClass();

	void connectionToMainDb(QStringList signalList);
	void createMainTables();
	void insertInUsers(QString tempMail, QString tempPass);
	void insertInQueueAndHistory(QByteArray arrFromClient);
	void getQueueValue();
	void deleteFromDb(QString Id, QString request, QString pos);
	void insertInTelegramPhoneTable(QString chat, QString phone);
	void verifyFuncDb(QByteArray verData);

signals:
	void sendStringListFromQueue(QStringList temp, QString chatIdTg);
	void signalFromVerifyFunc(QByteArray result);
	void sendVerifyResult(QByteArray result);


private:
	QSqlDatabase mainDbConnection;
};

