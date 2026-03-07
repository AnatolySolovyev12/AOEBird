#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qdebug>
#include <qtimer>

class SMSClass : public QObject
{
	Q_OBJECT

public:
	SMSClass(QObject* parent, QStringList tempList);
	~SMSClass();


	void closeSerialPort();
	void readData();
	void writeData(QByteArray data);
	void sendSMS(QString phone, QString text);


private:
	QSerialPort* serial = nullptr;
	QByteArray buffer;;
};