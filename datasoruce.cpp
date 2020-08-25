#include "datasoruce.h"
#include <QDebug>

DataSoruce::DataSoruce(uchar *b,uint32_t h,uint32_t w)
{
    buf = b;
    height = h;
    width = w;
}

DataSoruce::~DataSoruce()
{
    if(buf) free(buf);
}

//用于触发数据更新到其他的模组，包含显示控件,算法控件
void DataSoruce::slotDataUp()
{
    qDebug() << "slotDataUp";
    emit signalDataUp(buf,height,width);
}

