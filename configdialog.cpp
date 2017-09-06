#include "configdialog.h"
#include "ui_configdialog.h"
#include <QIntValidator>
//#include <qDebug>

configDialog::configDialog(int cw, int ch, int cm, QWidget *parent) :

    QDialog(parent),

    _width(cw),
    _height(ch),
    _mines(cm),

    ui(new Ui::configDialog)
{
    ui->setupUi(this);

    QIntValidator * vaildW = new QIntValidator(1, 30, this);//宽度限制
    QIntValidator * vaildH = new QIntValidator(1, 19, this);//高度限制
    QIntValidator * vaildM = new QIntValidator(0, 570, this);//雷数初步限制(后面还有个数检查)

    ui->lineEditWidth->setValidator(vaildW);
    ui->lineEditHeight->setValidator(vaildH);
    ui->lineEditMines->setValidator(vaildM);

    ui->lineEditWidth->setText(QString::number(_width));
    ui->lineEditHeight->setText(QString::number(_height));
    ui->lineEditMines->setText(QString::number(_mines));

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
}

//自定义accept槽（本来要调用父类的槽，现在调用自己的，因此稍后需要显示调用父类槽）
void configDialog::accept(){

    _width=ui->lineEditWidth->text().toInt();
    _height=ui->lineEditHeight->text().toInt();
    _mines=ui->lineEditMines->text().toInt();

    QDialog::accept();//自己的实现以后，再显示调用父类的accept槽
}

configDialog::~configDialog() {
    delete ui;
}
