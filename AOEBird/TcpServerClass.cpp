#include "TcpServerClass.h"

TcpServerClass::TcpServerClass(QObject *parent)
	: QObject(parent), tcpServer(new QTcpServer)
{
	connect(tcpServer, &QTcpServer::newConnection, this, &TcpServerClass::newConnection);

	QTimer::singleShot(500, [this]() {

		if (tcpServer->listen(QHostAddress::Any, 49555))
		{
			qDebug() << "TCP Server start listening port 49555";
		}
		else
		{
			qDebug() << "Error: TCP Server NOT listening port 49555";
		}
		});
}



TcpServerClass::~TcpServerClass()
{}



void TcpServerClass::newConnection()
{
	tcpSocket = tcpServer->nextPendingConnection();

	if (!tcpSocket)
	{
		qDebug() << "Warning: No pending connection in TcpServerClass::newConnection()";
		return;
	}

	connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpServerClass::serverRead);
	connect(tcpSocket, &QTcpSocket::disconnected, this, &TcpServerClass::clientDisconnected);

	QDate curDate = QDate::currentDate();
	QTime curTime = QTime::currentTime();

	lastTcpSocket = tcpSocket->peerAddress().toString().sliced(7);

	qDebug() << "Connect from host (" + lastTcpSocket + ") - " + curDate.toString("dd-MM-yyyy") + " " + curTime.toString();
}



void TcpServerClass::clientDisconnected()
{
	if (tcpSocket == nullptr)
	{
		qDebug() << "tcpSocket (" + lastTcpSocket + ") was disconnect but mTcpSocket was nullptr";
		return;
	}

	tcpSocket->close();
	qDebug() << "tcpSocket (" + lastTcpSocket + ") was disconnect and close";

	delete tcpSocket;
	tcpSocket = nullptr;
}



void TcpServerClass::serverRead()
{
	if (!tcpSocket) 
	{
		qDebug() << "Warining: Socket is null in start TcpServerClass::serverRead()";
		return;
	}

	while (tcpSocket->bytesAvailable() > 0)
	{
		QByteArray array = tcpSocket->readAll();

		QDate curDate = QDate::currentDate();
		QTime curTime = QTime::currentTime();

		qDebug() << "TcpServer read from (" + lastTcpSocket + ") : " + curDate.toString("dd-MM-yyyy") + " " + curTime.toString() << "\n" << array;
	}
}