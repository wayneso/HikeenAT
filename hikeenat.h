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
    void on_link_pushButton_clicked();

private:
    Ui::HikeenAT *ui;
    DeviceContext Intel_API_Display = {0};
};
#endif // HIKEENAT_H
