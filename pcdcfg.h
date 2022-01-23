#ifndef PCDCFG_H
#define PCDCFG_H

#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include "communication/serverthread.h"
#include "struct.h"
#include <QTimer>
#include <QTextStream>
#include <QFileDialog>
#include <QFile>

#define   saveCfgPath   "./cfg.txt"
class pcdCfg : public QWidget
{
    Q_OBJECT

public:
    pcdCfg(QWidget *parent = 0);
    ~pcdCfg();

    void topLayoutInit();   //全局参数配置
    void bottomLayoutInit();  //使能按键
    void serialportInit();  //串口
    void setCfgValue(); //设置
    void rstCfgValue();  //复位
    void  workThreadInit();  //工作线程初始化
    void saveBtnClickedSlot();  //保存配置
    void openfile( QString filename);
signals:

    void  stopSerial(const QString &ip );
    void  startSend(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);
    void  sig(QVariant para,uchar device);

private slots:
    void  cfgEnBtnClickedSlot();   //下发按下
    void  cfgRstBtnClickedSlot();
    void  timeOutMethod();    //定时超时方法
private:
    QSpinBox *idBox;
    QComboBox  *pwmBox ;
    QComboBox *brightModeBox;
    QComboBox *triggerBox;
    QComboBox *levelBox;
    QComboBox  *serialName;
    QGroupBox  *globalCfgBox,*chnCfgBox;
    QHBoxLayout  *topLayout,*bottomLayout;
    QSpinBox *ftChnBrightSetBox,*sdChnBrightSetBox,*tdChnBrightSetBox,*fhChnBrightSetBox; //亮度值
    QComboBox  *ftChnCtrlBox,*sdChnCtrlBox,*tdChnCtrlBox,*fhChnCtrlBox;  //通道开关状态
    QPushButton *cfgEnBtn,*cfgRstBtn; //cfg下发  cfg复位
    serverThread  *m_thread;
    bool serialMutex;
    QTimer  *m_timer;
    bool   cmdBtn; //0,下发  1,复位
};

#endif // PCDCFG_H
