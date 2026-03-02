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

private:
	QTimer* checkTimer = nullptr;
};

