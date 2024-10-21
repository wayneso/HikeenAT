#include "hikeenat.h"
#include "./ui_hikeenat.h"

HikeenAT::HikeenAT(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HikeenAT)
{
    ui->setupUi(this);
}

HikeenAT::~HikeenAT()
{
    delete ui;
}
