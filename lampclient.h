#ifndef LAMPCLIENT_H
#define LAMPCLIENT_H

#include <QTcpSocket>
#include <QScopedPointer>
#include <QObject>
#include <QColor>

// Интерфейс комманды
struct Command
{
    virtual ~Command() {}
    virtual bool operator ()(char type, const char *value) = 0;
};

// Шаблонная комманда:
// Type - тип комманды
// Obj - объект отвечающий за отправление данных в GUI
// Method - метод объекта Obj оптравляющий данные в GUI
// Func - функтор преобразующий данные "сырые данные" в данные "понятные" методу Method
template <char Type, typename Obj, typename Method, typename Func>
struct TemplateCommand : public Command
{
    TemplateCommand(Obj *obj_, Method func_, Func functor_)
        : obj(obj_), func(func_), functor(functor_)
    { }

    bool operator ()(char type, const char *value)
    {
        if (type != Type)
            return false;
        (obj->*func)(functor(value));
        return true;
    }

private:
    Obj *obj;
    Method func;
    Func functor;
};

// Вспомогательная функция по конструирования шаблонных комманд
template <char Type, typename Func, typename Obj, typename Method>
Command * makeCommand(Obj *obj, Method func, Func functor)
{
    return new TemplateCommand<Type, Obj, Method, Func>(obj, func, functor);
}

// TCP клиент лампы - принимает данные от сервера и отправляет комманды в GUI
class LampClient : public QObject
{
    Q_OBJECT

public:
    LampClient(QObject *parent = 0);
    ~LampClient();

public slots:
    // Подключиться к серверу по адресу
    Q_INVOKABLE bool connectTo(const QString &address, quint16 port);
    // зарегестрировать комманду
    void registerCommand(Command *cmd);
    // регистрация "всех" команд
    void autoRegistration();

private slots:
    // Считать данные из сокета
    void readData();
    // Реагируем на ошибки
    void error(QAbstractSocket::SocketError);
    // Выбираем нужную комманду
    bool parseCommand(char type, char *value);

signals:
    // Сигнал вкл/выкл лампы
    void lampOn(bool flag);
    // Сигнал смены цвета лампы
    void lampChangeColor(QColor color);

    // Соединение разорвалось
    void disconnected();
    // Сигнал передаем ошибку в GUI
    void socketError(QString txt);

private:
    // tcp socket
    QScopedPointer<QTcpSocket> socket;
    // время ожидание сокета при некоторых действиях
    static const uint waitingTime = 10000;
    // список зарегестрированных комманд
    QSet<Command*> commandList;
};

#endif // LAMPCLIENT_H
