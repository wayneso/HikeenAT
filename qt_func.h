#ifndef qt_func_H
#define qt_func_H

#include <QString>
#include <QTextEdit>
#include <QVector> // 确保包含 QVector 的头文件

enum
{
    Standard_DDCCI_INDEX,
};

void appendLog(QTextEdit *textEdit, const QString &message, const QVector<int> &dataArray = QVector<int>());

void Sent_Data_Handle(QString sent_data_str, QVector<int> &sent_data_array);

void Sent_Data_Handle_EDID(QString sent_data_str, QVector<int> &sent_data_array);

void QStrToQVec(QString str, QVector<int> &data); // 修改为引用

#endif
