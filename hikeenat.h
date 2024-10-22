#ifndef HIKEENAT_H
#define HIKEENAT_H

#include <QMainWindow>

#include "Graphics_api.h"
#include "syswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class HikeenAT;
}
QT_END_NAMESPACE

class HikeenAT : public QMainWindow
{
    Q_OBJECT

public:
    HikeenAT(QWidget *parent = nullptr);
    ~HikeenAT();

private slots:
    void Sent_Data_Timer_CallBack();

    void Open_Sys_Window();

    void Close_Sys_Window();

    void on_start_pushButton_clicked(bool checked);

    void on_link_pushButton_clicked(bool checked);

    void on_api_comboBox_currentIndexChanged(int index);

    void on_clear_pushButton_clicked();

    void on_copy_pushButton_clicked();



private:
    Ui::HikeenAT *ui;
    syswindow *sysWindow =  nullptr;
};
#endif // HIKEENAT_H
