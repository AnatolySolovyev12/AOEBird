#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDate>

class TcpServerClass  : public QObject
{
	Q_OBJECT

public:
	TcpServerClass(QObject *parent);
	~TcpServerClass();

	void newConnection();
	void clientDisconnected();
	void serverRead();

private:
	QTcpServer* tcpServer = nullptr;
	QTcpSocket* tcpSocket = nullptr;
	QString lastTcpSocket = "";
};