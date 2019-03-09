/*
 *@file:   barcode.h
 *@author: 缪庆瑞
 *@date:   2016.9.28
 *@brief:  处理各类条形码编号，返回编码串
 */
#ifndef BARCODE_H
#define BARCODE_H
#include <QString>
#include <QList>

class BarCode
{
public:
    BarCode();
    QString process128BCode(QString code);//处理128 B类型条码码的编号，返回bs字符串
    QString processEAN13Code(QString code);//处理EAN13码的编号，返回01字符串

private:
    void initCode128B();//初始化128 B型码相关数据
    void initEAN13();//初始化EAN13码相关数据

private:
    //code128B码 长度可调，但包括开始位和结束位在内不超过232个字元,停止位前需要加校验位，不然扫不出来
    QList<QString> char128BList;//code128 B类条码能编码的字符集 0-127的ascll码
    QList<QString> code128BValue;//与条码字符集对应的编码值 b：单位黑线1  s:单位空白0

    /*EAN13码 13位数字(0-9)，第一位前置码，最后一位校验码(通过前12位运算自动生成)，缺一不可。
     *数据字符和校验码都是占7个条形模块，整体由 左侧空白区(11)+前置码+起始符(3)+
     *左侧数据符(42(6*7))+中间分隔符(5)+右侧数据符(35(5*7))+校验符(7)+终止符(3)+右侧空白区(7)
     */
    QList<QString> preCodeWayList;//EAN13条码 前置码对应左侧数据符的编码方式AB 前置码为国家代码（1-9，最前面的数字）
    QList<QString> codeAEAN13Value;//左侧数据符(6个) A编码方式下0-9对应编码值 1：单位黑线  0:单位空白
    QList<QString> codeBEAN13Value;//左侧数据符 B编码方式下0-9对应编码值 1：单位黑线  0:单位空白
    QList<QString> codeCEAN13Value;//右侧数据符(5个) 0-9对应编码值 1：单位黑线  0:单位空白

};

#endif // BARCODE_H
