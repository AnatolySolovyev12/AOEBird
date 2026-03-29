#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qdebug>
#include <qtimer>
#include <iostream>
#include <QHash>
#include <QMap>


class SMSClass : public QObject
{
	Q_OBJECT

public:
	SMSClass(QObject* parent, QStringList tempList);
	~SMSClass();

	void closeSerialPort();
	void readData();
	void writeData(const QByteArray& data);
	void sendSMS(const QString& phone, const QString& text);
	void checkAndReconnectComPort();
	QString transliterate(const QString& text);

private:
	QSerialPort* serial = nullptr;
	QByteArray buffer;;
	bool readyForSend = false;
	QTimer* checkComTimer = nullptr;
	bool testIsRunningForConnect = false;
	bool testIsRunningForSmsCentre = false;
};