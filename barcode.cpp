/*
 *@file:   barcode.cpp
 *@author: 缪庆瑞
 *@date:   2016.9.28
 *@brief:  处理各类条形码编号，返回编码串
 */
#include "barcode.h"
#include <QDebug>
#include <QMessageBox>

BarCode::BarCode()
{
    initCode128B();
    initEAN13();
}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.28
 *@brief:   处理code128码的原始编号，返回bs串
 *@param:   code:用户输入的编号
 *@return:  QString:NULL表示编码失败
 */
QString BarCode::process128BCode(QString code)
{
    int checkId=104;//code128B的校验位  初始化存放开始位的id
    QString barCodes;//用来存放bs串
    barCodes.append("bbsbssbssss");//开始位 StartB
    QString temp;
    int indexId;
    int num = code.size();//获取输入编号的长度
    if(num == 0)
    {
        //qDebug()<<"请输入编号：";
        QMessageBox::information(0,"barcode","请输入编号：");
        return NULL;
    }
    //逐个对字符编码
    for(int i=0;i<num;i++)
    {
        temp=code.mid(i,1);
        indexId = char128BList.indexOf(temp);//获取字符对应的位置
        if(indexId == -1)
        {
            //qDebug()<<"输入的内容含不可编码字符。。。";
            QMessageBox::information(0,"barcode","输入的内容含不可\n编码字符。。。");
            return NULL;
        }
        else
        {
            barCodes.append(code128BValue.at(indexId));//追加与字符对应的编码
            checkId +=(i+1)*indexId;//计算校验位
        }
    }
    /*Code128检验位计算：
     *(开始位对应的ID值(B:104)＋每位数据在整个数据中的位置(从1开始)×每位数据对应的ID值)%103
     */
    checkId %=103;
    barCodes.append(code128BValue.at(checkId));//追加校验位的bs编码
    barCodes.append("bbsssbbbsbsbb");//停止位 StopB
    /*为开头和结尾多留11个长度的空白，它实际上是不属于128B的编码的，这里是模仿EAN13的空白区,
     *使生成的条形码看起来更顺眼些，但从接口功能上讲，不该放在这里。不过为了方便处理，先这样，
     *以后有时间再改。
     */
    barCodes.prepend("sssssssssss");
    barCodes.append("sssssssssss");
    return barCodes;//返回编码好的bs串
}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.29
 *@brief:   处理EAN13码的原始编号，返回01串
 *@param:   code:用户输入的编号
 *@return:  QString:NULL表示编码失败
 */
QString BarCode::processEAN13Code(QString code)
{
    if(code.size() != 12)
    {
        //qDebug()<<"请输入12位数字编号：";
        QMessageBox::information(0,"barcode","请输入12位数字编号：");
        return NULL;
    }
    //C1 C2 CC checkId变量都是为了计算校验位 算法可百度
    int C1=0;//编号奇数位相加（13位数字是从1开始的，且不加第十三位）
    int C2=0;//编号偶数位相加和乘3
    int CC=0;//C1+C2取个位
    int checkId=0;//校验位 10-CC取个位
    QString barCodes;//用来存放01串
    barCodes.append("00000000000");//11个模块的左侧空白区
    barCodes.append("101");//起始符
    bool ok;//标记将字符转换成数字是否成功
    int preNo=code.mid(0,1).toInt(&ok);//转换得到第一个数字，即前置码
    if(!ok)
    {
        //qDebug()<<"输入的字符不应含有非数字符号。。。";
        QMessageBox::information(0,"barcode","输入的字符不应含有\n非数字符号。。。");
        return NULL;
    }
    if(!preNo)
    {
        //qDebug()<<"前置码不能为0。。。";
        QMessageBox::information(0,"barcode","前置码不能为0。");
        return NULL;
    }
    //qDebug()<<"preNo:"<<preNo;
    C1+=preNo;
    //通过前置码得到左侧数据符的编码方式A或B，preCodeWayList对应1-9
    QString preCodeWay = preCodeWayList.at(preNo-1);
    //左侧数据符 6位
    for(int i=1;i<7;i++)
    {
        int index = code.mid(i,1).toInt(&ok);
        if(!ok)
        {
            //qDebug()<<"输入的字符不应含有非数字符号。。。";
            QMessageBox::information(0,"barcode","输入的字符不应含有\n非数字符号。。。");
            return NULL;
        }
        if(i%2)
        {
            C2+=index;
        }
        else
        {
            C1+=index;
        }
        //将对应编码方式下的字符编码追加到字符串中
        if(preCodeWay.at(i-1)=='A')
        {
            barCodes.append(codeAEAN13Value.at(index));
        }
        else if(preCodeWay.at(i-1)=='B')
        {
            barCodes.append(codeBEAN13Value.at(index));
        }
    }
    barCodes.append("01010");//中间分隔符
    //右侧数据 5位
    for(int j=7;j<12;j++)
    {
        int index = code.mid(j,1).toInt(&ok);
        if(!ok)
        {
            //qDebug()<<"输入的字符不应含有非数字符号。。。";
            QMessageBox::information(0,"barcode","输入的字符不应含有\n非数字符号。。。");
            return NULL;
        }
        if(j%2)
        {
            C2+=index;
        }
        else
        {
            C1+=index;
        }
        barCodes.append(codeCEAN13Value.at(index));
    }
    //qDebug()<<"C2:"<<C2<<"C1:"<<C1;
    CC=(C1+C2*3)%10;
    checkId=(10-CC)%10;
    qDebug()<<"checkId:"<<checkId;
    barCodes.append(codeCEAN13Value.at(checkId));//校验码编码
    barCodes.append("101");//终止符
    barCodes.append("0000000");//7个模块的右侧空白区
    return barCodes;//返回编码好的01串
}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.28
 *@brief:   初始化128 B型码相关数据
 */
void BarCode::initCode128B()
{
    /*******************************code128B码*******************************/
    //多字符的一般为控制类字符，用不到
    char128BList<<"SP"<<"!"<<"\""<<"#"<<"$"<<"%"<<"&"<<"'"<<"("<<")"
            <<"*"<<"+"<<","<<"-"<<"."<<"/"<<"0"<<"1"<<"2"<<"3"
            <<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<":"<<";"<<"<"<<"="
            <<">"<<"?"<<"@"<<"A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"
            <<"H"<<"I"<<"J"<<"K"<<"L"<<"M"<<"N"<<"O"<<"P"<<"Q"
            <<"R"<<"S"<<"T"<<"U"<<"V"<<"W"<<"X"<<"Y"<<"Z"<<"["
            <<"\\"<<"]"<<"^"<<"_"<<"`"<<"a"<<"b"<<"c"<<"d"<<"e"
            <<"f"<<"g"<<"h"<<"i"<<"j"<<"k"<<"l"<<"m"<<"n"<<"o"
            <<"p"<<"q"<<"r"<<"s"<<"t"<<"u"<<"v"<<"w"<<"x"<<"y"
            <<"z"<<"{"<<"|"<<"}"<<"~"<<"DEL"<<"FNC3"<<"FNC2"<<"SHIFT"<<"CODEC"
            <<"FNC4"<<"CODEA"<<"FNC1"<<"StartA"<<"StartB"<<"StartC"<<"Stop";
    //与字符一一对应的bs编码
    code128BValue<<"bbsbbssbbss"<<"bbssbbsbbss"<<"bbssbbssbbs"<<"bssbssbbsss"<<"bssbsssbbss"<<"bsssbssbbss"<<"bssbbssbsss"<<"bssbbsssbss"<<"bsssbbssbss"<<"bbssbssbsss"
            <<"bbssbsssbss"<<"bbsssbssbss"<<"bsbbssbbbss"<<"bssbbsbbbss"<<"bssbbssbbbs"<<"bsbbbssbbss"<<"bssbbbsbbss"<<"bssbbbssbbs"<<"bbssbbbssbs"<<"bbssbsbbbss"
            <<"bbssbssbbbs"<<"bbsbbbssbss"<<"bbssbbbsbss"<<"bbbsbbsbbbs"<<"bbbsbssbbss"<<"bbbssbsbbss"<<"bbbssbssbbs"<<"bbbsbbssbss"<<"bbbssbbsbss"<<"bbbssbbssbs"
            <<"bbsbbsbbsss"<<"bbsbbsssbbs"<<"bbsssbbsbbs"<<"bsbsssbbsss"<<"bsssbsbbsss"<<"bsssbsssbbs"<<"bsbbsssbsss"<<"bsssbbsbsss"<<"bsssbbsssbs"<<"bbsbsssbsss"
            <<"bbsssbsbsss"<<"bbsssbsssbs"<<"bsbbsbbbsss"<<"bsbbsssbbbs"<<"bsssbbsbbbs"<<"bsbbbsbbsss"<<"bsbbbsssbbs"<<"bsssbbbsbbs"<<"bbbsbbbsbbs"<<"bbsbsssbbbs"
            <<"bbsssbsbbbs"<<"bbsbbbsbsss"<<"bbsbbbsssbs"<<"bbsbbbsbbbs"<<"bbbsbsbbsss"<<"bbbsbsssbbs"<<"bbbsssbsbbs"<<"bbbsbbsbsss"<<"bbbsbbsssbs"<<"bbbsssbbsbs"
            <<"bbbsbbbbsbs"<<"bbssbssssbs"<<"bbbbsssbsbs"<<"bsbssbbssss"<<"bsbssssbbss"<<"bssbsbbssss"<<"bssbssssbbs"<<"bssssbsbbss"<<"bssssbssbbs"<<"bsbbssbssss"
            <<"bsbbssssbss"<<"bssbbsbssss"<<"bssbbssssbs"<<"bssssbbsbss"<<"bssssbbssbs"<<"bbssssbssbs"<<"bbssbsbssss"<<"bbbbsbbbsbs"<<"bbssssbsbss"<<"bsssbbbbsbs"
            <<"bsbssbbbbss"<<"bssbsbbbbss"<<"bssbssbbbbs"<<"bsbbbbssbss"<<"bssbbbbsbss"<<"bssbbbbssbs"<<"bbbbsbssbss"<<"bbbbssbsbss"<<"bbbbssbssbs"<<"bbsbbsbbbbs"
            <<"bbsbbbbsbbs"<<"bbbbsbbsbbs"<<"bsbsbbbbsss"<<"bsbsssbbbbs"<<"bsssbsbbbbs"<<"bsbbbbsbsss"<<"bsbbbbsssbs"<<"bbbbsbsbsss"<<"bbbbsbsssbs"<<"bsbbbsbbbbs"
            <<"bsbbbbsbbbs"<<"bbbsbsbbbbs"<<"bbbbsbsbbbs"<<"bbsbssssbss"<<"bbsbssbssss"<<"bbsbssbbbss"<<"bbsssbbbsbsbb";

}
/*
 *@author:  缪庆瑞
 *@date:    2016.9.28
 *@brief:   初始化EAN13码相关数据
 */
void BarCode::initEAN13()
{
    /***********************************EAN13码******************************/
    //EAN13条码 前置码对应左侧数据符的编码方式AB 前置码为国家代码（1-9，最前面的数字）
    preCodeWayList<<"AAAAAA"<<"AABABB"<<"AABBAB"<<"ABAABB"<<"ABBAAB"<<"ABBBAA"<<"ABABAB"<<"ABABBA"<<"ABBABA";
    //左侧数据符(6个) A编码方式下0-9对应编码值 1：单位黑线  0:单位空白
    codeAEAN13Value<<"0001101"<<"0011001"<<"0010011"<<"0111101"<<"0100011"
                   <<"0110001"<<"0101111"<<"0111011"<<"0110111"<<"0001011";
    //左侧数据符 B编码方式下0-9对应编码值 1：单位黑线  0:单位空白
    codeBEAN13Value<<"0100111"<<"0110011"<<"0011011"<<"0100001"<<"0011101"
                   <<"0111001"<<"0000101"<<"0010001"<<"0001001"<<"0010111";
    //右侧数据符(5个) 0-9对应编码值 1：单位黑线  0:单位空白
    codeCEAN13Value<<"1110010"<<"1100110"<<"1101100"<<"1000010"<<"1011100"
                   <<"1001110"<<"1010000"<<"1000100"<<"1001000"<<"1110100";
}
