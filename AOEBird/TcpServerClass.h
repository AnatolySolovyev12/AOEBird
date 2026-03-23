#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDate>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QHash>
#include <QPair>
#include <cstdlib> // для функций rand(), srand(a), exit().
#include <ctime> // для функции time(a)
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class TcpServerClass  : public QObject
{
	Q_OBJECT

public:
	TcpServerClass(QObject *parent);
	~TcpServerClass();

	void newConnection();
	void clientDisconnected();
	void serverRead();
	void sendVerithyResult(QByteArray result);
	int getRandomNumber(int min, int max);
	QString getRandomCode();
	void sendRegResult(QByteArray result);

signals:
	void sendNewRecordToDb(QByteArray newRecord);
	void sendVerifyData(QByteArray byteData);
	void checkUserForRegister(QByteArray byteData);
	void sendEmailForRegistration(QString eMail, QString subject, QString messege, QString files);
	void setNewUser(QString log, QString pass);


private:
	QTcpServer* tcpServer = nullptr;
	QTcpSocket* tcpSocket = nullptr;
	QString lastTcpSocket = "";
	QByteArray arrayBuffer;
	QHash <QString, QPair<QString, QString>> hashArray;
	QTimer* clearHash;
};