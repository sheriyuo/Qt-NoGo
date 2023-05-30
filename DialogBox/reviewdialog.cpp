#include "reviewdialog.h"
#include "ui_reviewdialog.h"

ReviewDialog::ReviewDialog(Judge *_judge, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReviewDialog),
    judge(_judge)
{
    messagebox = new MessageBox(this);
    tim = new QTimer();
    tim->setInterval(500);
    connect(tim,&QTimer::timeout,this,&ReviewDialog::display);
    ui->setupUi(this);


    QRegularExpression regx("[0-9]{1,3}$");
    QValidator *validator = new QRegularExpressionValidator(regx, ui->stepnum);
    ui->stepnum->setValidator(validator);
    connect(ui->stepnum, &QLineEdit::returnPressed, this,  &ReviewDialog::on_input_entered);


    //禁用enter默认
    ui->start->setAutoDefault(false);
    ui->pause->setAutoDefault(false);
    ui->next->setAutoDefault(false);
    ui->previous->setAutoDefault(false);
    ui->tryButton->setAutoDefault(false);
    ui->quit_try->setAutoDefault(false);
}

ReviewDialog::~ReviewDialog()
{
    delete ui;
}

void ReviewDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    painter.setBrush(QColor(BG_COLOR));
    painter.drawRect(0, 0, WIDTH, HEIGHT);
}

void ReviewDialog::display()
{
    if(now_step>=sum_steps){
        tim->stop();
    }
    else
    {
        qDebug()<<now_step;
        Item cur=dataVec[now_step];
        qDebug()<<cur.first<<" "<<cur.second;
        judge->PlaceAPiece(cur.first, cur.second);
        //qDebug()<<now_step;
        now_step++;
        qDebug()<<now_step;
    }
}

void ReviewDialog::on_start_clicked()
{
//    judge->init();
//    judge->runMode = 1;
    tim->start(1500);
}


void ReviewDialog::on_pause_clicked()
{
    tim->stop();
}


void ReviewDialog::on_previous_clicked()
{
    on_pause_clicked();
//    qDebug()<<now_step;
    judge->init();
    if(now_step<=0){
        return;
    }
    else{
        now_step--;
        qDebug()<<now_step;
        for(int i=0;i<now_step;i++){
            Item cur=dataVec[i];
            judge->PlaceAPiece(cur.first, cur.second, 1);
        }
    }
    emit judge->modifiedCB();
}


void ReviewDialog::on_next_clicked()
{
    on_pause_clicked();
    if(now_step>=sum_steps){
        tim->stop();
    }
    else
    {
//        qDebug()<<now_step;
        Item cur=dataVec[now_step];
//        qDebug()<<cur.first<<" "<<cur.second;
        judge->PlaceAPiece(cur.first, cur.second);
        now_step++;
//        qDebug()<<now_step;
    }
}

void ReviewDialog::on_input_entered(){
    on_pause_clicked();

    QString step = ui->stepnum->text();
    int stepnum=step.toInt();
    if(stepnum<0||stepnum>sum_steps){
        return ;
    }
    else{
        judge->init();
        now_step=stepnum;
        for(int i=0;i<now_step;i++){
            Item cur=dataVec[i];
            judge->PlaceAPiece(cur.first, cur.second, 1);
        }
    }
    on_pause_clicked();
    emit judge->modifiedCB();
}
void ReviewDialog::on_tryButton_clicked()
{
    on_pause_clicked();
    emit trybutton();
    ui->next->setEnabled(0);
    ui->previous->setEnabled(0);
    ui->start->setEnabled(0);
    ui->pause->setEnabled(0);
    ui->stepnum->setReadOnly(1);
}


void ReviewDialog::on_quit_try_clicked()
{
    emit quit_try();
    ui->next->setEnabled(1);
    ui->previous->setEnabled(1);
    ui->start->setEnabled(1);
    ui->pause->setEnabled(1);
    ui->stepnum->setReadOnly(0);
    on_pause_clicked();
    int stepnum=now_step;
    if(stepnum<0||stepnum>sum_steps){
        return ;
    }
    else{
        judge->init();
        now_step=stepnum;
        for(int i=0;i<now_step;i++){
            Item cur=dataVec[i];
            judge->PlaceAPiece(cur.first, cur.second, 1);
        }
    }
    on_pause_clicked();
    emit judge->modifiedCB();
}

void ReviewDialog::set_review_data(char state,char data[],ItemVector v){
    strState=state;
    std::strcpy(dataStr,data);
    dataVec=v;
    sum_steps=dataVec.size();
}

void ReviewDialog::init()
{
    judge->log(Level::Debug, "review dialog initialized");
    tim->stop();
    now_step = 0;
}
