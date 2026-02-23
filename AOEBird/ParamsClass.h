#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QDebug.h>;
#include <QFile.h>
#include <qsqlerror.h>
#include <QSqlQuery.h>
#include <iostream>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

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

signals:
	void errorLog(const QString&);


private:
	QSqlDatabase mainConnection;
};

