#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDate>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class TcpServerClass  : public QObject
{
	Q_OBJECT

public:
	TcpServerClass(QObject *parent);
	~TcpServerClass();

	void newConnection();
	void clientDisconnected();
	void serverRead();

signals:
	void sendNewRecordToDb(QByteArray newRecord);

private:
	QTcpServer* tcpServer = nullptr;
	QTcpSocket* tcpSocket = nullptr;
	QString lastTcpSocket = "";
	QByteArray arrayBuffer;
};