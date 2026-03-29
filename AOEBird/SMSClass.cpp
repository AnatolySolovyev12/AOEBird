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

void SMSClass::writeData(const QByteArray& data)
{
	QByteArray command = data + "\r\n"; // без добавления модем не поймёт команды
	serial->write(command);
	qDebug() << "TX:" << command;
}


void SMSClass::sendSMS(const QString& phone, const QString& text)
{
	if (!readyForSend)
	{
		qDebug() << serial->portName() + " not Ready for send SMS. Check your device, COM port or other moment";
		return;
	}

	QString translated = transliterate(text);

	writeData("AT+CMGF=1"); // включаем текстовый режим. Отправка будет производится не из хранилища

	QTimer::singleShot(700, [this, phone, translated]() {

		QString temp = "AT+CMGS=\"" + phone + "\""; // передаём номер телефона в соответствующем формате
		writeData(temp.toUtf8());
		});

	QTimer::singleShot(1200, [this, translated]() {
		QByteArray smsData = translated.toUtf8() + char(26);  //передаём текст для отправки и собственно завершаем формирование смс и отправляем его со спец символом. Без него ждём ввода.

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



QString SMSClass::transliterate(const QString& text) 
{
	QString result;

	for (int i = 0; i < text.length(); i++) 
	{
		QChar ch = text[i];

		switch (ch.unicode()) 
		{
			// Строчные буквы

		case 0x0430: result += "a"; break; // а
		case 0x0431: result += "b"; break; // б
		case 0x0432: result += "v"; break; // в
		case 0x0433: result += "g"; break; // г
		case 0x0434: result += "d"; break; // д
		case 0x0435: result += "e"; break; // е
		case 0x0451: result += "yo"; break; // ё
		case 0x0436: result += "zh"; break; // ж
		case 0x0437: result += "z"; break; // з
		case 0x0438: result += "i"; break; // и
		case 0x0439: result += "y"; break; // й
		case 0x043A: result += "k"; break; // к
		case 0x043B: result += "l"; break; // л
		case 0x043C: result += "m"; break; // м
		case 0x043D: result += "n"; break; // н
		case 0x043E: result += "o"; break; // о
		case 0x043F: result += "p"; break; // п
		case 0x0440: result += "r"; break; // р
		case 0x0441: result += "s"; break; // с
		case 0x0442: result += "t"; break; // т
		case 0x0443: result += "u"; break; // у
		case 0x0444: result += "f"; break; // ф
		case 0x0445: result += "h"; break; // х
		case 0x0446: result += "ts"; break; // ц
		case 0x0447: result += "ch"; break; // ч
		case 0x0448: result += "sh"; break; // ш
		case 0x0449: result += "sch"; break; // щ
		case 0x044A: result += ""; break; // ъ
		case 0x044B: result += "y"; break; // ы
		case 0x044C: result += ""; break; // ь
		case 0x044D: result += "e"; break; // э
		case 0x044E: result += "yu"; break; // ю
		case 0x044F: result += "ya"; break; // я

			// Заглавные буквы

		case 0x0410: result += "A"; break; // А
		case 0x0411: result += "B"; break; // Б
		case 0x0412: result += "V"; break; // В
		case 0x0413: result += "G"; break; // Г
		case 0x0414: result += "D"; break; // Д
		case 0x0415: result += "E"; break; // Е
		case 0x0401: result += "Yo"; break; // Ё
		case 0x0416: result += "Zh"; break; // Ж
		case 0x0417: result += "Z"; break; // З
		case 0x0418: result += "I"; break; // И
		case 0x0419: result += "Y"; break; // Й
		case 0x041A: result += "K"; break; // К
		case 0x041B: result += "L"; break; // Л
		case 0x041C: result += "M"; break; // М
		case 0x041D: result += "N"; break; // Н
		case 0x041E: result += "O"; break; // О
		case 0x041F: result += "P"; break; // П
		case 0x0420: result += "R"; break; // Р
		case 0x0421: result += "S"; break; // С
		case 0x0422: result += "T"; break; // Т
		case 0x0423: result += "U"; break; // У
		case 0x0424: result += "F"; break; // Ф
		case 0x0425: result += "H"; break; // Х
		case 0x0426: result += "Ts"; break; // Ц
		case 0x0427: result += "Ch"; break; // Ч
		case 0x0428: result += "Sh"; break; // Ш
		case 0x0429: result += "Sch"; break; // Щ
		case 0x042A: result += ""; break; // Ъ
		case 0x042B: result += "Y"; break; // Ы
		case 0x042C: result += ""; break; // Ь
		case 0x042D: result += "E"; break; // Э
		case 0x042E: result += "Yu"; break; // Ю
		case 0x042F: result += "Ya"; break; // Я

		default:
			result += ch; // Оставляем остальные символы как есть
			break;
		}
	}

	return result;
}