/*
 *@file:   barcodebox.cpp
 *@author: 缪庆瑞
 *@date:   2016.9.28
 *@brief:  存放条形码的区域
 */
#include "barcodebox.h"
#include <QDebug>
#include <QMessageBox>

BarCodeBox::BarCodeBox(const QString &title, QWidget *parent) :
    QGroupBox(title,parent)
{
    barCodes="";
    barCodeWidth = 0;
}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.28
 *@brief:   给私有变量设置值，在重绘函数中使用
 *@param:   barCodes:存放编码的bs串
 */
void BarCodeBox::setBarCodes(QString barCodes)
{
    this->barCodes=barCodes;
    int num = barCodes.size();//获取bs或01编码串的大小 bs：code128  01：EAN13
    barCodeWidth=num*PEN_WIDTH;//存储生成条形码宽度 2016-10-19
}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.28
 *@brief:   绘制条形码
 *@param:   painter:用来绘图
 *@param:   x:条形码左顶点的x坐标
 *@param:   y:条形码左顶点的y坐标
 */
void BarCodeBox::drawBarCode(QPainter &painter, int x, int y)
{
    int num = barCodes.size();
    painter.setPen(QPen(Qt::black,PEN_WIDTH));//设置绘图画笔 黑色 两个像素
    for(int i=0;i<num;i++)
    {
        if(barCodes.at(i)=='b')//画两个像素宽的黑线,一个像素太窄，扫描不出来 针对code128
        {
            painter.drawLine(x+i*PEN_WIDTH,y,x+i*PEN_WIDTH,y+BARCODE_HEIGHT);
        }
        else if(barCodes.at(i)=='1')//针对EAN13 因为起始符 中间分隔符 终止符的条形线较长一些，需要单独处理
        {
            //因为EAN13的数字位数是固定13个 所以所占的条形模块数是固定的，起始符 中间分隔符 终止符位置也是固定的
            if((i>10&&i<14)||(i>55&&i<61)||(i>102&&i<106))
            {
                painter.drawLine(x+i*PEN_WIDTH,y,x+i*PEN_WIDTH,y+BARCODE_HEIGHT+6);
            }
            else
            {
                painter.drawLine(x+i*PEN_WIDTH,y,x+i*PEN_WIDTH,y+BARCODE_HEIGHT);
            }
        }
    }
}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.28
 *@brief:   保存图片
 */
void BarCodeBox::savePicture()
{
    if(barCodeWidth==0)
    {
        return;
    }
    QPixmap pixmap(barCodeWidth+8,BARCODE_HEIGHT+24);
    //将部件的指定区域渲染到pixmap上,第一个参数是渲染的目标设备，第二个参数是在目标设备的起始渲染点，
    //第三个参数指定被渲染部件的区域
    this->render(&pixmap,QPoint(0,0),QRegion(posX-4,posY-12,barCodeWidth+8,BARCODE_HEIGHT+24));//将widget页渲染到pixmap上
    pixmap=pixmap.scaled(400,300);//将图片伸缩到指定大小，默认参数忽略宽高比例
    //第一个参数指定保存的文件名，第二个图片格式，第三个是图片质量0-100
    if(pixmap.save("条形码.png","PNG",90))//保存图片
    {
        QMessageBox::information(this,"information","图片已成功保存");
    }
    else
    {
        QMessageBox::information(this,"information","图片保存失败");
    }
}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.28
 *@brief:   重写重绘事件处理函数，根据条形码编码值（bs/01）生成条形码
 *@param:   event:绘制事件
 */
void BarCodeBox::paintEvent(QPaintEvent *event)
{
    QGroupBox::paintEvent(event);//先调用父类的重绘事件处理函数，不然默认的groupbox的边框和标题都不在显示
    if(barCodeWidth==0)
    {
        return;
    }
    //QPainter 默认只能在paintEvent中调用，且被绘图的设备也需要重写了paintEvent
    QPainter painter(this);//在box上定义一个绘图对象
    painter.setPen(Qt::NoPen);//因为要使用drawRect()只填充颜色，不画边缘线，也就不需要画笔
    painter.setBrush(QBrush(Qt::white));//二维码背景区域为白色
    posX = (this->width()-barCodeWidth)/2;//条形码左顶点在box中的x位置
    posY = (this->height()-BARCODE_HEIGHT)/2;//条形码左顶点在box中的y位置
    //先画一个比条形码略大的区域
    painter.drawRect(posX-4,posY-12,barCodeWidth+8,BARCODE_HEIGHT+24);
    //画条形码
    drawBarCode(painter,posX,posY);

}
