#include "SMSClass.h"

SMSClass::SMSClass(QObject* parent, QStringList tempList)
	: QObject(parent), serial(new QSerialPort), checkComTimer(new QTimer)
{
	connect(serial, &QSerialPort::errorOccurred, [=](QSerialPort::SerialPortError error)
		{
			if (error != QSerialPort::NoError)
			{
				qDebug() << "Error occurred: " << error;

				if (error == QSerialPort::ResourceError)
				{
					qDebug() << "Device disconnected or other problem:" << serial->errorString();
					serial->close();
				}
			}
		});

	connect(serial, &QSerialPort::readyRead, this, &SMSClass::readData);

	serial->setPortName(tempList[0]); // задаём имя последовательного порта
	serial->setBaudRate(tempList[1].toInt(), QSerialPort::AllDirections); // скорость обмена и тип направления
	serial->setDataBits(QSerialPort::DataBits(tempList[2].toInt())); // количество бит данных в кадре
	serial->setFlowControl(QSerialPort::NoFlowControl); // контроль управления потоком
	serial->setParity(static_cast<QSerialPort::Parity>(tempList[3].toInt())); // контроль четности
	serial->setStopBits(QSerialPort::StopBits(tempList[4].toInt())); // устанавливаем стоп биты

	if (!serial->open(QIODeviceBase::ReadWrite)) // открываем в режиме чтения и записи
	{
		std::cout << "Error in SMSClass when try to open " << tempList[0].toStdString() << ". Error:\n" << serial->errorString().toStdString() << std::endl;
		readyForSend = false;
	}
	else
	{
		serial->setDataTerminalReady(true); //DTS
		serial->setRequestToSend(true); //RTS
		readyForSend = true;
		qDebug() << "SMSClass: " + serial->portName() + " is OPEN";
	}

	connect(checkComTimer, &QTimer::timeout, this, &SMSClass::checkAndReconnectComPort);
	checkComTimer->start(180000);
}

SMSClass::~SMSClass()
{
	closeSerialPort();
}

void SMSClass::closeSerialPort()
{
	if (serial->isOpen())
		serial->close();
}

void SMSClass::readData()
{
	buffer += serial->readAll(); // обязательно буферризируем т.к. модем порой отвечает кусками. Там будем получаеть полный ответ для полноценной его обработки

	if (buffer.contains("OK") || buffer.contains("ERROR"))
	{
		if (testIsRunningForConnect || testIsRunningForSmsCentre)
		{
			if (testIsRunningForConnect)
			{
				qDebug() << "Test " + serial->portName() + " at \"AT\" with buffer: " << buffer << '\n'; // проверка доступности модема
				testIsRunningForConnect = false;
			}

			if (testIsRunningForSmsCentre)
			{
				qDebug() << "Test " + serial->portName() + " at \"AT+CSCA?\" with buffer: " << buffer << '\n'; // проверка регистрации симкарты в сети и наличияя СМС центра
				testIsRunningForSmsCentre = false;

				if (buffer.contains("ERROR"))
				{
					QTimer::singleShot(100, [this]() {
						QByteArray rebootComand = "AT#REBOOT\r"; // перезапуск в случае получения ошибки вместо номера СМС центра
						qint64 rebootRetunrByte = serial->write(rebootComand);

						if (rebootRetunrByte == -1)
							qDebug() << "Device " + serial->portName() + " is not reboot on \"AT#REBOOT\"";
						else
							qDebug() << "Reboot " + serial->portName() + " at \"AT#REBOOT\"" << '\n';
						});
				}
			}
		}
		else
		{
			qDebug() << "RX:" << buffer << '\n';
		}
		buffer.clear();
	}
}

void SMSClass::writeData(QByteArray data)
{
	QByteArray command = data + "\r\n"; // без добавления модем не поймёт команды
	serial->write(command);
	qDebug() << "TX:" << command;
}


void SMSClass::sendSMS(QString phone, QString text)
{
	if (!readyForSend)
	{
		qDebug() << serial->portName() + " not Ready for send SMS. Check your device, COM port or other moment";
		return;
	}

	writeData("AT+CMGF=1"); // включаем текстовый режим. Отправка будет производится не из хранилища

	QTimer::singleShot(700, [this, phone, text]() {

		QString temp = "AT+CMGS=\"" + phone + "\""; // передаём номер телефона в соответствующем формате
		writeData(temp.toUtf8());
		});

	QTimer::singleShot(1200, [this, text]() {
		QByteArray smsData = text.toUtf8() + char(26);  //передаём текст для отправки и собственно завершаем формирование смс и отправляем его со спец символом. Без него ждём ввода.

		//Из мануала:
		//To send the message issue Ctrl - Z char(0x1A hex).
		//To exit without sending the message issue ESC char(0x1B hex).

		serial->write(smsData);
		//	qDebug() << "SMS sent:" << smsData;
		});
}



void SMSClass::checkAndReconnectComPort()
{
	if (serial->isOpen())
	{
		testIsRunningForConnect = true;
		QByteArray testCommand = "AT\r";
		qint64 bytesWrittenFirst = serial->write(testCommand); // проверка доступности модема

		if (bytesWrittenFirst == -1)
		{
			qDebug() << "Device " + serial->portName() + " is not answer at \"AT\"";
		}

		QTimer::singleShot(50, [this]() {
			QByteArray testCommandSmsCentre = "AT+CSCA?\r";
			testIsRunningForSmsCentre = true;
			qint64 bytesWrittenSecond = serial->write(testCommandSmsCentre); // проверка регистрации симкарты в сети и наличияя СМС центра
			if (bytesWrittenSecond == -1)
			{
				qDebug() << "Device " + serial->portName() + " is not answer at \"AT+CSCA?\"";
			}
			});
	}
	else
	{
		if (!serial->open(QIODeviceBase::ReadWrite)) // открываем в режиме чтения и записи
		{
			std::cout << "Error in SMSClass wneh try to open " << serial->portName().toStdString() << ". Error:\n" << serial->errorString().toStdString() << std::endl;
			readyForSend = false;
		}
		else
		{
			std::cout << "Reconnect to " << serial->portName().toStdString() << std::endl;
		}
	}
}