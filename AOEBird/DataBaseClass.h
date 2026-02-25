#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>;
#include <QFile>
#include <qsqlerror>
#include <QSqlQuery>
#include <iostream>
#include <QDate>

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


private:
	QSqlDatabase mainDbConnection;
};

