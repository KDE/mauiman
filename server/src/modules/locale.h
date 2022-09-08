#ifndef LOCALE_H
#define LOCALE_H

#include <QObject>

class Locale : public QObject
{
    Q_OBJECT
public:
    explicit Locale(QObject *parent = nullptr);

signals:

};

#endif // LOCALE_H
