#include <QRgb>
#include "lampclient.h"

LampClient::LampClient(QObject *parent)
    : QObject(parent), socket(new QTcpSocket)
{
    connect(socket.data(), SIGNAL(readyRead()), SLOT(readData()));
    connect(socket.data(), SIGNAL(error(QAbstractSocket::SocketError)), SLOT(error(QAbstractSocket::SocketError)));
    connect(socket.data(), SIGNAL(disconnected()), SIGNAL(disconnected()));

    autoRegistration();
}

LampClient::~LampClient()
{
    qDeleteAll(commandList);
    commandList.clear();
}

bool LampClient::connectTo(const QString &address, quint16 port)
{
    socket->connectToHost(address, port, QIODevice::ReadOnly);
    return socket->waitForConnected(waitingTime);
}

void LampClient::registerCommand(Command *cmd)
{
    if (!cmd)
        return;
    auto it = commandList.find(cmd);
    if (it != commandList.constEnd()) {
        Command * tmp = *it;
        commandList.remove(tmp);
        delete tmp;
    }
    commandList.insert(cmd);
}

void LampClient::autoRegistration()
{
    // Регистрируем комманды
    // Комманда включения
    registerCommand(makeCommand<0x12>(this, &LampClient::lampOn,
                                      [](const char *)->bool { return true; }));
    // Комманда выключения
    registerCommand(makeCommand<0x13>(this, &LampClient::lampOn,
                                      [](const char *)->bool { return false; }));
    // Комманда смены цвета
    registerCommand(makeCommand<0x20>(this, &LampClient::lampChangeColor,
                                      [](const char *value)->QRgb { return qRgb(value[2], value[1], value[0]); }));
}

void LampClient::readData()
{
    QByteArray bdata = socket->readAll();
    const char *data = bdata.constData();

    int16_t dataSize = bdata.size();
    if (dataSize < 3)
        return;

    char key = data[0];

    int16_t length = 0;
    {
        char tmp[2] = {data[2], data[1]};
        length = *reinterpret_cast<int16_t*>(tmp);
    }

    if (dataSize != 3 + length)
        return;

    char value[length];
    if (length > 0) {
        for (int16_t i = 0; i < length; ++i)
            value[i] = data[length + 2 - i];
    }

    parseCommand(key, value);
}

void LampClient::error(QAbstractSocket::SocketError /*error*/)
{
    emit socketError(socket->errorString());
}

bool LampClient::parseCommand(char type, char *value)
{
    for (auto c : commandList)
        if ((*c)(type, value))
            return true;
    return false;
}
