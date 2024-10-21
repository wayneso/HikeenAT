#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    void loadSavedCredentials();

    QString getMasteraddr() const;
    QString getUsername() const;
    QString getPassword() const;

private slots:
    void on_loginButton_clicked();  // 槽函数声明

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
