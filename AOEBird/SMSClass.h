#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qdebug>
#include <qtimer>
#include <iostream>

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
	void checkAndReconnectComPort();

private:
	QSerialPort* serial = nullptr;
	QByteArray buffer;;
	bool readyForSend = false;
	QTimer* checkComTimer = nullptr;
	bool testIsRunning = false;
};