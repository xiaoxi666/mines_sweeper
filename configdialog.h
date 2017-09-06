#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class configDialog;
}

class configDialog : public QDialog
{
    Q_OBJECT

public:
    explicit configDialog(int cw,int ch,int cm,QWidget *parent = 0);
    ~configDialog();

    int _width;
    int _height;
    int _mines;

public slots:
    virtual void accept() override;

private:
    Ui::configDialog *ui;
};

#endif // CONFIGDIALOG_H
