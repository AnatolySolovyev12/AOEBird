#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>;
#include <QFile>
#include <qsqlerror>
#include <QSqlQuery>
#include <iostream>
#include <QDate>
#include <qsqlerror>

class DataBaseClass  : public QObject
{
	Q_OBJECT

public:
	DataBaseClass(QObject *parent);
	~DataBaseClass();

	void connectionToMainDb(QStringList signalList);
	void createMainTables();
	void insertInUsers(QString tempMail, QString tempPass);
	void insertInQueueAndHistory(QStringList tempList);
	void getQueueValue();
	void deleteFromDb(QString Id, QString request, QString pos);

signals:
	void sendStringListFromQueue(QStringList temp);


private:
	QSqlDatabase mainDbConnection;
};

