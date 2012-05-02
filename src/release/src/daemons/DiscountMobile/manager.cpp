#include "manager.h"
#include "settings.h"

Manager::Manager() : QObject()
{
    DatabaseAdapter::init();
    DatabaseLoader::init();
    DatabaseEngine * engine = DatabaseAdapter::adapter();
    // Внутрение ошибки менеджера системы
    connect(this, SIGNAL(information(Information *)), SLOT(informationHandle(Information *)));
    // Ошибки движка базы данных
    connect(engine, SIGNAL(information(Information *)), SLOT(informationHandle(Information *)));
}

Manager::~Manager()
{
    database->stop();
    delete database;
    DatabaseAdapter::finilize();

    server->close();
    delete server;

    request->stop();
    delete request;
    delete manager;
}

void Manager::informationHandle(Information * information)
{
    information->print();
}

void Manager::restart(QUuid id, int rmk, int cmd)
{
    emit information(new Information(conv("Перезагрузка сервера. Остановка внутренего сервера и системы отправки сообщений на внешний сервер Discount Mobile.")));

    server->close();
    request->stop();

    sleep(1);

    int error = 0;
    QString description = "";

    emit information(new Information(conv("Запуск сервера, считывание и повторная инициализация параметров системы из базы данных.")));

    initValuesFromDatabase();
    manager->initValuesFromDatabase();

    if (startServer())
        emit information(new Information(conv("Сервер запущен на адресе %1 и порту %2.").arg(serverHost.toString()).arg(serverPort)));
    else
    {
        error = 401;
        description = conv("Ошибка создания сервера на адресе %1 и порту %2.").arg(serverHost.toString()).arg(serverPort);
    }

    request->initValuesFromDatabase();
    request->start();

    emit serverRestarted(id, rmk, cmd, error, description);
}

void Manager::manage()
{
    int port;
    QString name, host;
    QString user, password;

    if (Settings::readConfig(name, host, port, user, password) &&
        DatabaseAdapter::connect(name, host, port, user, password))
    {
        initValuesFromDatabase();

        server = new Server;
        if (startServer())
        {
            emit information(new Information(conv("Сервер запущен на адресе %1 и порту %2.").arg(serverHost.toString()).arg(serverPort)));

            manager = new DataManager;

            request = new NetworkRequest;
            request->start();

            database = new Database;
            database->start();

            qRegisterMetaType<QUuid>("QUuid");
            // Ошибки локального сервера
            connect(server, SIGNAL(information(Information *)), SLOT(informationHandle(Information *)));
            // Ошибки разбора сообщений
            connect(manager, SIGNAL(information(Information *)), SLOT(informationHandle(Information *)));
            // Ошибка запросов к серверу дисконтной системы
            connect(request, SIGNAL(information(Information *)), SLOT(informationHandle(Information *)));
            // Ошибки опроса базы данных
            connect(database, SIGNAL(information(Information *)), SLOT(informationHandle(Information *)));
            // Разбор команды, полученной от локального сервера
            connect(server, SIGNAL(receiveCommand(QUuid, QByteArray)), manager, SLOT(parseCommand(QUuid, QByteArray)));
            // Ответ локальному серверу
            connect(manager, SIGNAL(preparedReply(QUuid, QByteArray)), server, SIGNAL(sendReply(QUuid,QByteArray)));
            // Поиск UID, ассоциированных с местом кассира
            connect(manager, SIGNAL(assosiateSession(int, QUuid&)), server, SLOT(assosiateSession(int, QUuid&)));
            // Составление запроса к серверу дисконтной системы
            connect(manager, SIGNAL(preparedRequest(QUuid, int, int, QByteArray)), request, SLOT(addRequest(QUuid, int, int, QByteArray)));
            // Запрос о перезагрузке сервера
            connect(manager, SIGNAL(restart(QUuid, int, int)), SLOT(restart(QUuid, int, int)));
            // Результат перезагрузки сервера
            connect(this, SIGNAL(serverRestarted(QUuid, int, int, int, QString)), manager, SLOT(serverRestarted(QUuid, int, int, int, QString)));
            // Разбор ответа от сервера дисконтной системы
            connect(request, SIGNAL(receiveReply(QUuid, int, int, QByteArray)), manager, SLOT(parseReply(QUuid, int, int, QByteArray)));
            // Занесение данных в базу в случае невозможности выполнить запрос
            connect(request, SIGNAL(addToDatabase(Request *)), database, SLOT(addRequestToDatabase(Request *)));
            // Ошибка выполнения запроса к серверу дисконтной системы
            connect(request, SIGNAL(sendError(QUuid, int, int, int, QString)), manager, SLOT(sendError(QUuid, int, int, int, QString)));
            // Загрузка всех невыполненных запросов
            connect(database, SIGNAL(loadedData(QList<Request*>)), request, SLOT(loadFromDatabase(QList<Request*>)));
        }
        else
            emit information(new Exception(conv("Невозможно создать TCP/IP подключение."), conv("Ошибка сервера %1: %2").arg(server->serverError()).arg(server->errorString()), 102));
    }
}

bool Manager::startServer()
{
    bool connected = false;
    while (!connected && attemptCount > 0)
    {
        connected = !server->isListening() && server->listen(serverHost, serverPort);
        if (!connected)
        {
            sleep(timeout);
            attemptCount--;

            emit information(new Warning(conv("Ошибка создания сервера. Осталось попыток %1.").arg(attemptCount)));
        }
    }
    return connected;
}

void Manager::initValuesFromDatabase()
{
    // Тамйаут повторных запросов на запуск сервера
    timeout = DatabaseLoader::serverConnectionTimeout();
    // Количество попыток запустить сервер
    attemptCount = DatabaseLoader::serverAttemptCount();
    serverPort = DatabaseLoader::serverPort();
    serverHost = QHostAddress(DatabaseLoader::serverHost());
}
