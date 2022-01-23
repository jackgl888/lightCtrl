#include "pcdcfg.h"

pcdCfg::pcdCfg(QWidget *parent)
    : QWidget(parent)
{
    topLayoutInit();
    bottomLayoutInit();

    QVBoxLayout  *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    serialMutex = true;
    cmdBtn = true;  //下发
    this->m_timer = new QTimer(this);
    connect(this->m_timer,SIGNAL(timeout()),SLOT(timeOutMethod()));
    //加载默认配置
    openfile(saveCfgPath);
    this->resize(800,350);
}

pcdCfg::~pcdCfg()
{

}



void pcdCfg::topLayoutInit()
{

    QStringList  list;
    //id
    QLabel * idLab = new QLabel("ID:",this);
    idBox = new QSpinBox(this);
    idBox->setMaximum(99);
    idBox->setMinimum(0);
    QHBoxLayout  *idLayout = new QHBoxLayout();
    idLayout->addWidget(idLab);
    idLayout->addWidget(idBox);

    //PWM
    QLabel *pwmLab = new QLabel("PWM:",this);
    pwmBox = new QComboBox(this);

    list<<"62.5K"<<"125K";
    pwmBox->addItems(list);
    QHBoxLayout *pwmLayout = new QHBoxLayout();
    pwmLayout->addWidget(pwmLab);
    pwmLayout->addWidget(pwmBox);

    //亮度模式
    QLabel  *brightModeLab = new QLabel("亮度模式:",this);
     brightModeBox = new QComboBox(this);
    list.clear();
    list<<"A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"
       <<"K"<<"L"<<"M"<<"N"<<"O"<<"P";
    brightModeBox->addItems(list);
    QHBoxLayout *brightModeLayout = new QHBoxLayout();
    brightModeLayout->addWidget(brightModeLab);
    brightModeLayout->addWidget(brightModeBox);

    //触发设置
    QLabel *triggerLab = new QLabel("触发:",this);
    triggerBox = new QComboBox(this);
    list.clear();
    list<<"CM"<<"SM"<<"FM"<<"RST";
    triggerBox->addItems(list);
    QHBoxLayout *triggerLayout = new QHBoxLayout();
    triggerLayout->addWidget(triggerLab);
    triggerLayout->addWidget(triggerBox);

    //电平设置
    QLabel *levelLab = new QLabel("电平:",this);
    levelBox = new QComboBox(this);
    list.clear();
    list<<"H"<<"L";
    levelBox->addItems(list);
    QHBoxLayout *levelLayout = new QHBoxLayout();
    levelLayout->addWidget(levelLab);
    levelLayout->addWidget(levelBox);

    //串口
    QLabel *comLab  = new QLabel("串口:",this);
    serialName  = new QComboBox(this);
    list.clear();
    QString comname ;
    for(uchar i = 0;i<40;i++)
    {
        comname = QString("%1%2").arg("COM").arg(QString::number(i));
        list<<comname;
    }
   serialName->addItems(list);
    QHBoxLayout  *comLayout = new QHBoxLayout();
    comLayout->addWidget(comLab);
    comLayout->addWidget(serialName);

    //全局参数布局
    QGridLayout  *globCfgLayout  = new QGridLayout();
    globCfgLayout ->addLayout(idLayout,0,0);
    globCfgLayout->addLayout(comLayout,0,1);
    globCfgLayout ->addLayout(pwmLayout,1,0);
    globCfgLayout ->addLayout(brightModeLayout,1,1);
    globCfgLayout ->addLayout(triggerLayout,2,0);
    globCfgLayout->addLayout(levelLayout,2,1);
    globalCfgBox  = new QGroupBox("全局参数",this);
    globalCfgBox->setLayout(globCfgLayout );



    //通道序号
    QLabel *chnLab = new QLabel("通道号",this);
    QLabel  *ftChnLab = new QLabel("通道1:",this);
    QLabel  *sdChnLab = new QLabel("通道2:",this);
    QLabel  *tdChnLab = new QLabel("通道3:",this);
    QLabel  *fhChnLab = new QLabel("通道4",this);


    //亮度值
    QLabel *brightSetLab = new QLabel("亮度值",this);
    ftChnBrightSetBox = new QSpinBox(this);
    ftChnBrightSetBox->setMinimum(0);
    ftChnBrightSetBox->setMaximum(1024);
    sdChnBrightSetBox =new QSpinBox(this);
    sdChnBrightSetBox->setMinimum(0);
    sdChnBrightSetBox->setMaximum(1024);
    tdChnBrightSetBox = new QSpinBox(this);
    tdChnBrightSetBox->setMinimum(0);
    tdChnBrightSetBox->setMaximum(1024);
    fhChnBrightSetBox = new QSpinBox(this);
    fhChnBrightSetBox->setMinimum(0);
    fhChnBrightSetBox->setMaximum(1024);

    //通道开关控制
    QLabel * chnCtrlLab = new QLabel("开关",this);
    list.clear();
    list<<"ON"<<"OFF";
    ftChnCtrlBox = new QComboBox(this);
    ftChnCtrlBox->addItems(list);
    sdChnCtrlBox = new QComboBox(this);
    sdChnCtrlBox->addItems(list);
    tdChnCtrlBox = new QComboBox(this);
    tdChnCtrlBox->addItems(list);
    fhChnCtrlBox = new QComboBox(this);
    fhChnCtrlBox->addItems(list);


    //通道配置布局
    //head& 第一行
    QGridLayout *chnLayout = new QGridLayout();
    chnLayout->addWidget(chnLab,0,0);
    chnLayout->addWidget(brightSetLab,0,1);
    chnLayout->addWidget(chnCtrlLab,0,2);
    chnLayout->addWidget(ftChnLab,1,0);
    chnLayout->addWidget(ftChnBrightSetBox,1,1);
    chnLayout->addWidget( ftChnCtrlBox ,1,2);
    //第二行  第三行
    chnLayout->addWidget(sdChnLab,2,0);
    chnLayout->addWidget( sdChnBrightSetBox,2,1);
    chnLayout->addWidget( sdChnCtrlBox,2,2);

    chnLayout->addWidget(tdChnLab,3,0);
    chnLayout->addWidget(tdChnBrightSetBox,3,1);
    chnLayout->addWidget(  tdChnCtrlBox ,3,2);

    //第四行
    chnLayout->addWidget(fhChnLab,4,0);
    chnLayout->addWidget(fhChnBrightSetBox,4,1);
    chnLayout->addWidget(  fhChnCtrlBox ,4,2);

    chnCfgBox  = new QGroupBox("通道参数",this);
    chnCfgBox->setLayout(chnLayout);


    topLayout = new QHBoxLayout();
    topLayout->addWidget(globalCfgBox);
    topLayout->addWidget(chnCfgBox);


}

void pcdCfg::bottomLayoutInit()
{
    cfgEnBtn = new QPushButton("下发",this);
    cfgEnBtn->setFixedSize(100,50);
    cfgRstBtn  = new QPushButton("复位",this);
    cfgRstBtn->setFixedSize(100,50);
    bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(cfgEnBtn);
    bottomLayout->addStretch();
    bottomLayout->addWidget(cfgRstBtn);
    bottomLayout->addStretch();

    //
    connect(cfgEnBtn,SIGNAL(clicked()),this,SLOT(cfgEnBtnClickedSlot()));

}


void pcdCfg::openfile( QString filename)
{
    uchar row=0;
    QString cfgVlue;

       if(!filename.isEmpty())
       {
             QFile  file(filename);
             if(file.open(QFile::ReadOnly|QFile::Text))
             {
                 QTextStream  stream(&file);
                 QString  line;
                 do
                 {
                   line = stream.readLine();
                   if(!line.isEmpty())
                   {
                        QStringList pieces= line.split(",",QString::SkipEmptyParts);
                        switch (row) {
                        case 0:   //id
                                cfgVlue = pieces.value(1);
                              this->idBox->setValue(cfgVlue.toUShort());
                            break;
                        case  1:    //串口
                                cfgVlue = pieces.value(1);
                                serialName->setCurrentText(cfgVlue);
                                break;

                        case 2:   //pwm
                                cfgVlue = pieces.value(1);
                                 this->pwmBox->setCurrentText(cfgVlue); ;
                            break;

                        case 3:   //亮度模式
                                cfgVlue = pieces.value(1);
                                this->brightModeBox->setCurrentText(cfgVlue);
                            break;
                        case 4:    //触发模式
                                cfgVlue = pieces.value(1);
                                this->triggerBox->setCurrentText(cfgVlue);
                            break;
                        case 5:  //触发电平
                                cfgVlue = pieces.value(1);
                                this->levelBox->setCurrentText(cfgVlue);
                            break;
                        case 6:  //通道1配置
                                cfgVlue = pieces.value(1);
                                this->ftChnBrightSetBox->setValue(cfgVlue.toUShort());
                                cfgVlue = pieces.value(2);
                                this->ftChnCtrlBox->setCurrentText(cfgVlue);
                            break;
                        case 7:  //通道2配置
                                cfgVlue = pieces.value(1);
                                this->sdChnBrightSetBox->setValue(cfgVlue.toUShort());
                                cfgVlue = pieces.value(2);
                                this->sdChnCtrlBox->setCurrentText(cfgVlue);
                            break;
                        case 8:  //通道3配置
                                cfgVlue = pieces.value(1);
                                this->tdChnBrightSetBox->setValue(cfgVlue.toUShort());
                                cfgVlue = pieces.value(2);
                                this->tdChnCtrlBox->setCurrentText(cfgVlue);
                            break;
                        case 9:  //通道4配置
                                cfgVlue = pieces.value(1);
                                this->fhChnBrightSetBox->setValue(cfgVlue.toUShort());
                                cfgVlue = pieces.value(2);
                                this->fhChnCtrlBox->setCurrentText(cfgVlue);
                            break;

                        default:
                            break;
                        }

                   }
                   row++;
                 }while(!line.isEmpty());
             }
             file.close();
       }
}



void pcdCfg::setCfgValue()
{
    QVariant   variant;
    pcdPara  para;
    QByteArray str;
   //  QString str;
    para.id= idBox->value();
    if(pwmBox->currentText()=="62.5K")
    para.pwm = 0 ;
    else
    para.pwm = 1;
    if(levelBox->currentText()=="L")
    para.level =0;
    else
    para.level = 1;
     str= brightModeBox->currentText().toLocal8Bit();
    para.brightMode =  str[0];

    if(triggerBox->currentText()=="CM")
    para.triggerMode = 0;
    else if(triggerBox->currentText() =="SM")
    para.triggerMode = 1;
    else if(triggerBox->currentText() =="FM")
    para.triggerMode = 2;
    else if(triggerBox->currentText() =="RST")
    para.triggerMode = 4;
    para.brightVal[0] =  ftChnBrightSetBox->value();
    para.brightVal[1]=  sdChnBrightSetBox->value();
     para.brightVal[2] =  tdChnBrightSetBox->value();
     para.brightVal[3] =  fhChnBrightSetBox->value();
    if(ftChnCtrlBox->currentText() == "ON")
    para.pcdCtrl[0] =1;
    else
      para.pcdCtrl[0] = 0;
    if(sdChnCtrlBox->currentText() == "ON")
     para.pcdCtrl[1]  = 1;
    else
    para.pcdCtrl[1]  =0;
    if(tdChnCtrlBox->currentText() == "ON")
    para.pcdCtrl[2]  = 1;
    else
      para.pcdCtrl[2] =0;
    if(fhChnCtrlBox->currentText() == "ON")
     para.pcdCtrl[3] = 1;
    else
        para.pcdCtrl[3] =0;
    variant.setValue(para);
    sig(variant,0);

}



//复位
void pcdCfg::rstCfgValue()
{
    startSend(serialName->currentText(),0,0,0,0);
}


//工作线程初始化
void pcdCfg::workThreadInit()
{
    if(serialMutex  == true)
    {
        serialMutex = false;

        m_thread = new serverThread(0,serialName->currentText(),this);  //

        //串口发送
        connect(this, SIGNAL(startSend(QString,int,uchar,ushort,ushort)),
                m_thread, SLOT( sendDataSlot(QString,int,uchar,ushort,ushort)),Qt::UniqueConnection);
        //停止串口
        connect(this,SIGNAL(stopSerial(const QString &)),m_thread,SLOT(stopSerialSlot(const QString  &)));
        //线程退出
        connect(m_thread,SIGNAL(finished()),m_thread,SLOT(deleteLater()),Qt::UniqueConnection);

        //更新串口状态
       // connect(m_thread,SIGNAL(updateUiSerial(const QString &)),this,
            //    SLOT(disconnectSerial(const QString &)),Qt::UniqueConnection);

//        connect(m_thread,SIGNAL( bedUpdate(const uchar,const uchar,const QString &)),this,
//               SLOT(slotUpdateCmdData(const uchar,const uchar,const QString &)));  //界面更新来自串口的数据

        //UI传递总的数据到工作线程
        connect(this,SIGNAL(sig(QVariant,uchar)),m_thread,SLOT(slotGetData(QVariant,uchar)));

        //接收工作线程传过来的数据
      //  connect(m_thread,SIGNAL(sigtu(QVariant)),this,SLOT(slotUiUpdate(QVariant)));

        this->m_thread->start();
    }
}



void pcdCfg::cfgEnBtnClickedSlot()
{
     workThreadInit();
     saveBtnClickedSlot();
     cmdBtn =true;
    this->m_timer->start(500);
}



void pcdCfg::cfgRstBtnClickedSlot()
{
    workThreadInit();
     cmdBtn =false;
    this->m_timer->start(500);
}

void pcdCfg::timeOutMethod()
{
   m_timer->stop();
   if(cmdBtn == true)
   setCfgValue();  //下发
   else
   rstCfgValue();  //复位
}


//另存配置文件
void pcdCfg::saveBtnClickedSlot()
{
    QString str1,str2,text;

    //保存cgf文件
      QFile   file( saveCfgPath );

     if(file.open(QFile::ReadWrite|QFile::Text))
    {

            QTextStream   out(&file);
            //设备id
            str1 = QString::number(idBox->value());
            text = QString("%1%2%3").arg("id").arg(",").arg(str1);
            out<<text<<endl;

            //串口号
            str1 = serialName->currentText();

            text = QString("%1%2%3").arg("串口号").arg(",").arg(str1);
            out<<text<<endl;

            //pwm
            str1 = pwmBox->currentText();
            text = QString("%1%2%3").arg("pwm").arg(",").arg(str1);
            out<<text<<endl;

            //亮度模式
            str1 = brightModeBox->currentText();
            text = QString("%1%2%3").arg("亮度模式").arg(",").arg(str1);
            out<<text<<endl;

            //触发模式
            str1 = triggerBox->currentText();
            text = QString("%1%2%3").arg("触发模式").arg(",").arg(str1);
            out<<text<<endl;

            //触发电平
            str1 = levelBox->currentText();
            text = QString("%1%2%3").arg("触发电平").arg(",").arg(str1);
            out<<text<<endl;

            //通道
            str1 = QString::number(ftChnBrightSetBox->value());    //亮度值
            str2 = ftChnCtrlBox->currentText(); //开关状态
            text = QString("%1%2%3%4%5").arg("通道1").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;
            str1 = QString::number(sdChnBrightSetBox->value());    //亮度值
            str2 = sdChnCtrlBox->currentText(); //开关状态
            text = QString("%1%2%3%4%5").arg("通道2").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;
            str1 = QString::number(tdChnBrightSetBox->value());    //亮度值
            str2 = tdChnCtrlBox->currentText(); //开关状态
            text = QString("%1%2%3%4%5").arg("通道3").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;
            str1 = QString::number(fhChnBrightSetBox->value());    //亮度值
            str2 = fhChnCtrlBox->currentText(); //开关状态
            text = QString("%1%2%3%4%5").arg("通道4").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;


    }
     file.close();

}
