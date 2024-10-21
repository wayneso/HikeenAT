#include "hikeenat.h"
#include "LoginDialog.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LoginDialog loginDialog;
    if (loginDialog.exec() == QDialog::Accepted) {

        HikeenAT mainWindow;
        mainWindow.show();
        return app.exec();
    }
    return 0;
}
