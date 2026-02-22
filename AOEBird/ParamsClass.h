#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QDebug.h>;

class ParamsClass  : public QObject
{
	Q_OBJECT

public:
	ParamsClass(QObject *parent);
	~ParamsClass();

	bool createOrConnectParamsBd();
	bool createTable();

signals:
	void errorLog(const QString&);


private:
	
	QSqlDatabase mainConnection;
};

