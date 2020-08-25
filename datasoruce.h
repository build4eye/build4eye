#ifndef DATASORUCE_H
#define DATASORUCE_H

#include <QObject>

class DataSoruce : public QObject
{
    Q_OBJECT
public:
    DataSoruce(uchar *b,uint32_t h,uint32_t w);
    ~DataSoruce();

signals:
    void signalDataUp(uchar *b,uint32_t h,uint32_t w);

public slots:
    void slotDataUp();

public:
    uchar *buf;
    uint32_t height;
    uint32_t width;
};

#endif // DATASORUCE_H
