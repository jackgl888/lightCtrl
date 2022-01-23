#ifndef STRUCT_H
#define STRUCT_H

#include<QMetaType>

struct pcdPara
{
    uchar id;  //id
    uchar pwm; //频率
    uchar level;//触发电平
    uchar  triggerMode;//触发模式
    uchar  brightMode;  //亮度模式
    ushort brightVal[4]; //亮度值
    uchar  pcdCtrl[4];  //开关

};



Q_DECLARE_METATYPE(pcdPara)
#endif // STRUCT_H
