#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qdebug.h>

class SMSClass  : public QObject
{
	Q_OBJECT

public:
	SMSClass(QObject *parent);
	~SMSClass();


	void closeSerialPort();
	void readData();
	void writeData(const QByteArray& data);


private:
	QSerialPort* serial = nullptr;
};

