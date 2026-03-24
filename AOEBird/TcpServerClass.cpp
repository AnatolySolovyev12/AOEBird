#include "TcpServerClass.h"

TcpServerClass::TcpServerClass(QObject* parent)
	: QObject(parent), tcpServer(new QTcpServer), clearHash(new QTimer)
{
	srand(static_cast<unsigned int>(time(0)));

	connect(tcpServer, &QTcpServer::newConnection, this, &TcpServerClass::newConnection);
	connect(clearHash, &QTimer::timeout, [this]() { hashArray.clear(); });

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
{
}



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

	qDebug() << "\n" << "Connect from host (" + lastTcpSocket + ") - " + curDate.toString("dd-MM-yyyy") + " " + curTime.toString();
}



void TcpServerClass::clientDisconnected()
{
	if (tcpSocket == nullptr)
	{
		qDebug() << "tcpSocket (" + lastTcpSocket + ") was disconnect but mTcpSocket was nullptr" << "\n";
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
		arrayBuffer += tcpSocket->readAll();

		QDate curDate = QDate::currentDate();
		QTime curTime = QTime::currentTime();

		QRegularExpression strPattern(QString(R"(\%\d*)"));
		QRegularExpressionMatch matchReg = strPattern.match(arrayBuffer);

		QString CRC;

		if (matchReg.hasMatch())
		{
			CRC = matchReg.captured().remove("%");

			arrayBuffer.remove(arrayBuffer.indexOf("%"), matchReg.captured().length());
		}

		if (CRC.toInt() == arrayBuffer.length())
		{
			qDebug() << "CRC EQUAL " << CRC.toInt() << " " << arrayBuffer.length();

			if (arrayBuffer.contains("$&auth&$"))
			{
				emit sendVerifyData(arrayBuffer);
			}
			else if (arrayBuffer.contains("$&register&$"))
			{
				emit sendVerifyData(arrayBuffer);
			}
			else if (arrayBuffer.contains("$&CodeForReg&$"))
			{
				QJsonDocument jDoc = QJsonDocument::fromJson(arrayBuffer.constData());

				if (jDoc.isNull()) {
					qDebug() << "JSON parse error in TcpServerClass::serverRead()";
					arrayBuffer.clear();
					return;
				}

				QJsonObject rootArray = jDoc.object();

				if (hashArray.find(rootArray["codeMail"].toString()) != hashArray.constEnd())
				{
					clearHash->stop();
					emit setNewUser(rootArray["login"].toString(), rootArray["password"].toString());
					
					QJsonObject objWithParam
					{
						{ "status", "$&REGISTERISDONE&$" }
					};

					jDoc.setObject(objWithParam);

					QByteArray bytes = jDoc.toJson(QJsonDocument::Compact);

					tcpSocket->write(bytes);
				}
				else
				{
					QJsonObject objWithParam
					{
						{ "status", "$&INCORRECTREGISTERCODE&$" }
					};

					jDoc.setObject(objWithParam);

					QByteArray bytes = jDoc.toJson(QJsonDocument::Compact);

					tcpSocket->write(bytes);
				}
			}
			else
			{
				tcpSocket->write(QByteArray("$&OK&$"));
				emit sendNewRecordToDb(arrayBuffer);
			}

			qDebug() << "TcpServer read from (" + lastTcpSocket + ") : " + curDate.toString("dd-MM-yyyy") + " " + curTime.toString() << "\n" << arrayBuffer.constData();
		}
		else
		{
			qDebug() << "CRC NOT EQUAL " << CRC.toInt() << " " << arrayBuffer.length();
			tcpSocket->write(QByteArray("$&RESEND&$"));
		}
	}

	arrayBuffer.clear();
}





void TcpServerClass::sendVerithyResult(QByteArray result)
{
	tcpSocket->write(result);
}



void TcpServerClass::sendRegResult(QByteArray result)
{
	if (result.contains("$&REGISTER&$"))
	{
		QJsonDocument jDoc = QJsonDocument::fromJson(arrayBuffer.constData());

		if (jDoc.isNull()) {
			qDebug() << "JSON parse error in TcpServerClass::sendRegResult()";
			return;
		}

		QString tempCode = getRandomCode();

		QJsonObject rootArray = jDoc.object();

		hashArray.insert(tempCode, QPair<QString, QString>(rootArray["login"].toString(), rootArray["password"].toString()));

		qDebug() << hashArray;

		emit sendEmailForRegistration(rootArray["login"].toString(), "Регистрация", tempCode, "");

		clearHash->start(120000);
	}

	tcpSocket->write(result);
}



int TcpServerClass::getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}



QString TcpServerClass::getRandomCode()
{
	QString idString;

	for (int val = 0; val < 8; val++)
	{
		int someNumber = 0;

		if (getRandomNumber(0, 1))
		{
			someNumber = getRandomNumber(97, 102);
		}
		else
		{
			someNumber = getRandomNumber(48, 57);
		}

		char randomChar = static_cast<char>(someNumber);
		QChar qch = randomChar;
		idString += qch;
	}

	return idString;
}