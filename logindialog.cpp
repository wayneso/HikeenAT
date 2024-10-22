#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QSettings>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    this->setWindowTitle("登录");
    loadSavedCredentials();
    // 设置密码输入框的回显模式为 Password
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

LoginDialog::~LoginDialog() {
    delete ui;
}
QString LoginDialog::getMasteraddr() const {
    return ui->masteraddrlineEdit->text();
}
QString LoginDialog::getUsername() const {
    return ui->usernameLineEdit->text();
}

QString LoginDialog::getPassword() const {
    return ui->passwordLineEdit->text();
}

void LoginDialog::loadSavedCredentials() {
    QSettings settings("username", "password");
    QString saveMasteraddr = settings.value("masteraddr").toString();
    QString savedUsername = settings.value("username").toString();
    QString savedPassword = settings.value("password").toString();

    if (!savedUsername.isEmpty()) {
        ui->masteraddrlineEdit->setText(saveMasteraddr);
        ui->usernameLineEdit->setText(savedUsername);
        ui->passwordLineEdit->setText(savedPassword);
        ui->rememberMeCheckBox->setChecked(true); // 默认勾选
    }
}

void LoginDialog::on_loginButton_clicked() {
    // 获取用户名和密码
    QString masteraddr = getMasteraddr();
    QString username = getUsername();
    QString password = getPassword();

    // 简单的用户名和密码验证
    if (username == "admin" && password == "1234") {
        // 验证成功，检查是否勾选“记住密码”
        if (ui->rememberMeCheckBox->isChecked()) {
            QSettings settings("username", "password");
            settings.setValue("masteraddr", masteraddr);
            settings.setValue("username", username);
            settings.setValue("password", password);
        } else {
            // 如果没有勾选，清除保存的用户名和密码
            QSettings settings("username", "password");
            settings.remove("masteraddr");
            settings.remove("username");
            settings.remove("password");
        }

        // 关闭对话框，表示登录成功
        accept();
    } else {
        // 验证失败，显示警告消息
        QMessageBox::warning(this, "登录失败", "请输入正确的用户名或密码");
    }
}




