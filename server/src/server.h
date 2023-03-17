#pragma once

#include <QCoreApplication>
#include <QObject>

class Server : public QCoreApplication
{
    Q_OBJECT
public:
    explicit Server(int &argc, char **argv);
    ~Server();
    bool init();

private:
    QVector<QObject*> m_modules;
};

