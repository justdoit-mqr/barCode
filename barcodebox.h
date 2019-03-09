/*
 *@file:   barcodebox.h
 *@author: 缪庆瑞
 *@date:   2016.9.28
 *@brief:  存放条形码的区域
 */
#ifndef BARCODEBOX_H
#define BARCODEBOX_H

#include <QGroupBox>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>

#define BARCODE_HEIGHT 70 //条形码的高度
#define PEN_WIDTH 2 //画笔宽度 像素数

class BarCodeBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit BarCodeBox(const QString & title,QWidget *parent = 0);
    void setBarCodes(QString barCodes);//设置私有变量barCodes的值
    void drawBarCode(QPainter &painter,int x,int y);
    void savePicture();//保存图片

protected:
    void paintEvent(QPaintEvent * event);//重写绘图事件处理函数

signals:

public slots:

private:
    QString barCodes;//存放将条形码翻译后的bs/01代码
    int barCodeWidth;//存储生成的条码宽度
    int posX;//条形码左上顶点横坐标位置
    int posY;//条形码左上顶点纵坐标位置

};

#endif // BARCODEBOX_H
