#ifndef HIKEENAT_H
#define HIKEENAT_H

#include <QMainWindow>

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

private:
    Ui::HikeenAT *ui;
};
#endif // HIKEENAT_H
