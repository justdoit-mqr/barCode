/*
 *@author: 缪庆瑞
 *@date:   2016.9.28
 *@update: 2023.7.10
 *@brief:  存放条形码的区域
 */
#ifndef BARCODEBOX_H
#define BARCODEBOX_H

#include <QGroupBox>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include "barcode.h"

#define LR_MARGIN_CODE128B 16  //code128B条形码左右留白宽度
#define LR_MARGIN_EAN13 2      //EAN13条形码左右留白宽度
#define TB_MARGIN_CODE128B 16  //code128B条形码上下留白宽度
#define TB_MARGIN_EAN13 22     //code128B条形码上下留白宽度
#define PEN_WIDTH 3 //画笔宽度 像素数

class BarCodeBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit BarCodeBox(const QString & title,QWidget *parent = 0);
    bool setBarCode(QString barCodeSrc,BarCode::BarCodeType barCodeType);//设置条码
    void savePicture();//保存图片

protected:
    void paintEvent(QPaintEvent * event);//重写绘图事件处理函数

signals:

public slots:

private:
    void drawBarCode(QPainter &painter,int x,int y);

    BarCode barCode;//负责条形码编码
    BarCode::BarCodeType barCodeType;//条码类型
    QString barCodeSrc;//存放条码原数据
    QString barCodeEncoded;//存放将条形码编码后的bs/01代码

    QPixmap pixmap;//保存条形码图片
    int barCodeWidth;//存储生成的条码宽度
    const int barCodeHeight;//条码高度

};

#endif // BARCODEBOX_H
