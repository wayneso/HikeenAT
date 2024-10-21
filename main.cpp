#include "hikeenat.h"
#include "LoginDialog.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 显示登录对话框
    LoginDialog loginDialog;
    if (loginDialog.exec() == QDialog::Accepted) {
        // 登录成功，显示主窗口
        HikeenAT mainWindow;
        mainWindow.show();
        return app.exec();
    }

    // 登录失败或取消
    return 0;
}
