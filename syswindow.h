#ifndef SYSWINDOW_H
#define SYSWINDOW_H

#include <QDialog>

namespace Ui {
class syswindow;
}

class syswindow : public QDialog
{
    Q_OBJECT

public:
    explicit syswindow(QWidget *parent = nullptr);
    ~syswindow();

signals:
    void windowClosed(); // 窗口关闭信号

protected:
    void closeEvent(QCloseEvent *event) override; // 重写closeEvent

private slots:
    void on_pushButton_clicked();

private:
    Ui::syswindow *ui;
};

#endif // SYSWINDOW_H
