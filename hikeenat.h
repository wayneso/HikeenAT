#ifndef HIKEENAT_H
#define HIKEENAT_H

#include <QMainWindow>

#include "Graphics_api.h"

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

    void on_start_pushButton_clicked(bool checked);

    void on_link_pushButton_clicked(bool checked);

private:
    Ui::HikeenAT *ui;
};
#endif // HIKEENAT_H
