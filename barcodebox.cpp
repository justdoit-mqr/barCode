/*
 *@author: 缪庆瑞
 *@date:   2016.9.28
 *@update: 2023.7.10
 *@brief:  存放条形码的区域
 */
#include "barcodebox.h"
#include <QDebug>
#include <QMessageBox>
#include <QTime>

BarCodeBox::BarCodeBox(const QString &title, QWidget *parent) :
    QGroupBox(title,parent),barCodeHeight(150)
{
    barCodeType = BarCode::UNKNOWN_TYPE;
    barCodeWidth = 0;
}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.28
 *@update:  2023.7.10
 *@brief:   设置条码
 *@param:   barCodeSrc:存放条码原数据
 *@param:   barCodeType:条码类型
 *@return:  bool:true=条码有效  false=条码无效
 */
bool BarCodeBox::setBarCode(QString barCodeSrc, BarCode::BarCodeType barCodeType)
{
    //根据条码类型调用接口进行编码
    QString encodedStr;
    if(barCodeType == BarCode::CODE_128B_TYPE)
    {
        encodedStr = barCode.process128BCode(barCodeSrc);
    }
    else if(barCodeType == BarCode::EAN13_TYPE)
    {
        encodedStr = barCode.processEAN13Code(barCodeSrc);
    }
    else
    {
        encodedStr = "";
    }
    if(encodedStr.isEmpty())
    {
        return false;
    }
    //记录有效的编码信息
    this->barCodeEncoded = encodedStr;
    this->barCodeWidth = encodedStr.size()*PEN_WIDTH;
    this->barCodeType = barCodeType;
    this->barCodeSrc = barCodeSrc;
    return true;
}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.28
 *@update:  2023.7.10
 *@brief:   保存图片
 */
void BarCodeBox::savePicture()
{
    if(this->barCodeType == BarCode::UNKNOWN_TYPE
            || barCodeWidth == 0)
    {
        return;
    }
    QPixmap save_pixmap = pixmap.scaled(400,300,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);//将图片伸缩到指定大小，默认参数忽略宽高比例
    //第一个参数指定保存的文件名，第二个图片格式，第三个是图片质量0-100
    QString filename = QString("条形码%1.png").arg(QTime::currentTime().toString("HHmmss"));
    if(save_pixmap.save(filename,"PNG",100))//保存图片
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
 *@update:  2023.7.10
 *@brief:   重写重绘事件处理函数，根据条形码编码值（bs/01）生成条形码
 *@param:   event:绘制事件
 */
void BarCodeBox::paintEvent(QPaintEvent *event)
{
    QGroupBox::paintEvent(event);//先调用父类的重绘事件处理函数，不然默认的groupbox的边框和标题都不在显示
    if(this->barCodeType == BarCode::UNKNOWN_TYPE
            || barCodeWidth == 0 )
    {
        return;
    }
    //根据条码类型定义pixmap的大小
    if(this->barCodeType == BarCode::CODE_128B_TYPE)
    {
        QPixmap tmp_pixmap(barCodeWidth+LR_MARGIN_CODE128B*2,barCodeHeight+TB_MARGIN_CODE128B*2);
        pixmap = tmp_pixmap;
    }
    else if(this->barCodeType == BarCode::EAN13_TYPE)
    {
        QPixmap tmp_pixmap(barCodeWidth+LR_MARGIN_EAN13*2,barCodeHeight+TB_MARGIN_EAN13*2);
        pixmap = tmp_pixmap;
    }
    /***************在pixmap上绘制条形码**************/
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white));
    //填充背景
    painter.drawRect(0,0,pixmap.width(),pixmap.height());
    //画条形码
    int x = (pixmap.width()-barCodeWidth)/2;//条形码左顶点在box中的x位置
    int y = (pixmap.height()-barCodeHeight)/2;//条形码左顶点在box中的y位置
    drawBarCode(painter,x,y);

    /**************将pixmap绘制到界面中心***********/
    QPainter painter2(this);
    painter2.drawPixmap((this->width()-pixmap.width())/2,
                        (this->height()-pixmap.height())/2,pixmap);
}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.28
 *@update:  2023.7.10
 *@brief:   绘制条形码
 *@param:   painter:用来绘图
 *@param:   x:条形码左顶点的x坐标
 *@param:   y:条形码左顶点的y坐标
 */
void BarCodeBox::drawBarCode(QPainter &painter, int x, int y)
{
    int num = barCodeEncoded.size();
    painter.setPen(QPen(Qt::black,PEN_WIDTH));//设置绘图画笔 黑色
    int end_y = y+barCodeHeight;//条码线条(非分割符)y底端位置
    //绘制条形码线条
    for(int i=0;i<num;i++)
    {
        int start_x = x+i*PEN_WIDTH;//条码线条x位置
        if(barCodeEncoded.at(i)=='b')//画指定像素宽的黑线,一个像素太窄，扫描不出来 针对code128
        {
            painter.drawLine(start_x,y,start_x,end_y);
        }
        else if(barCodeEncoded.at(i)=='1')//针对EAN13 因为起始符 中间分隔符 终止符的条形线较长一些，需要单独处理
        {
            //因为EAN13的数字位数是固定13个 所以所占的条形模块数是固定的，起始符 中间分隔符 终止符位置也是固定的
            if((i>10&&i<14)||(i>55&&i<61)||(i>102&&i<106))
            {
                painter.drawLine(start_x,y,start_x,end_y+TB_MARGIN_EAN13-6);
            }
            else
            {
                painter.drawLine(start_x,y,start_x,end_y);
            }
        }
    }
    //EAN13码绘制对应的数字
    if(this->barCodeType == BarCode::EAN13_TYPE)
    {
        painter.setRenderHint(QPainter::TextAntialiasing);
        //前置符
        QRect rect(x+3*PEN_WIDTH,end_y,7*PEN_WIDTH,TB_MARGIN_EAN13);
        painter.drawText(rect,Qt::AlignCenter,this->barCodeSrc.mid(0,1));
        //左侧6个数据
        for(int i=0;i<6;i++)
        {
            QRect rect(x+(14+i*7)*PEN_WIDTH,end_y,7*PEN_WIDTH,TB_MARGIN_EAN13);
            painter.drawText(rect,Qt::AlignCenter,this->barCodeSrc.mid(i+1,1));
        }
        //右侧6个数据
        for(int i=0;i<6;i++)
        {
            QRect rect(x+(61+i*7)*PEN_WIDTH,end_y,7*PEN_WIDTH,TB_MARGIN_EAN13);
            painter.drawText(rect,Qt::AlignCenter,this->barCodeSrc.mid(i+7,1));
        }

    }
}
