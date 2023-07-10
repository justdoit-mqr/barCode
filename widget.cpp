/*
 *@file:   widget.cpp
 *@author: 缪庆瑞
 *@date:   2016.9.28
 *@brief:  构造条形码生成主界面
 */
#include "widget.h"
#include "barcode.h"
#include <QBoxLayout>
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    label = new QLabel("编号：",this);
    lineEdit = new QLineEdit(this);
    produceCode128Button = new QPushButton("生成code128",this);
    produceEAN13Button = new QPushButton("生成EAN13",this);
    savePictureButton = new QPushButton("保存到图片",this);
    barCodeBox = new BarCodeBox("条形码区域：",this);

    QHBoxLayout *hBoxLayout = new QHBoxLayout();//第一行水平布局
    hBoxLayout->addWidget(label);
    hBoxLayout->addWidget(lineEdit);
    hBoxLayout->addWidget(produceCode128Button);
    hBoxLayout->addWidget(produceEAN13Button);
    hBoxLayout->addWidget(savePictureButton);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);//整体垂直布局
    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addWidget(barCodeBox);

    connect(produceCode128Button,SIGNAL(clicked()),this,SLOT(produceBarCode128Slot()));
    connect(produceEAN13Button,SIGNAL(clicked()),this,SLOT(produceBarCodeEAN13Slot()));
    connect(savePictureButton,SIGNAL(clicked()),this,SLOT(savePictureSlot()));

    this->resize(600,400);
}

Widget::~Widget()
{
}
/*
 *@author: 缪庆瑞
 *@date:   2016.9.28
 *@brief:  生成code128条形码
 */
void Widget::produceBarCode128Slot()
{
    QString code=lineEdit->text();//获取输入的编号
    barCodeBox->setBarCode(code,BarCode::CODE_128B_TYPE);//设置条码
    barCodeBox->update();//重新绘制
}
/*
 *@author: 缪庆瑞
 *@date:   2016.9.29
 *@brief:  生成EAN13条形码
 */
void Widget::produceBarCodeEAN13Slot()
{
    QString code=lineEdit->text();//获取输入的编号
    barCodeBox->setBarCode(code,BarCode::EAN13_TYPE);//设置条码
    barCodeBox->update();//重新绘制
}
/*
 *@author: 缪庆瑞
 *@date:   2016.10.19
 *@brief:  生成条形码的图片
 */
void Widget::savePictureSlot()
{
    barCodeBox->savePicture();
}


