/*
 *@file:   widget.h
 *@author: 缪庆瑞
 *@date:   2016.9.28
 *@brief:  Widget类的头文件
 */
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "barcodebox.h"


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void produceBarCode128Slot();//生成code128类型的条形码
    void produceBarCodeEAN13Slot();//生成EAN13类型的条形码
    void savePictureSlot();//保存到图片的槽

private:
    QLabel *label;
    QLineEdit *lineEdit;//输入编号
    QPushButton *produceCode128Button;//生成code128条形码的按钮
    QPushButton *produceEAN13Button;//生成EAN13条形码的按钮
    QPushButton *savePictureButton;//保存条形码到图片的按钮
    BarCodeBox *barCodeBox;//显示条形码区域
};

#endif // WIDGET_H
