#ifndef ADDNEWITEM_H
#define ADDNEWITEM_H

#include <QWidget>
#include<QMainWindow>
#include<QString>
#include<QTextEdit>
#include<QPushButton>
class addNewItem : public QWidget
{
    Q_OBJECT
public:
    explicit addNewItem(QWidget *parent = nullptr);

signals:
    void conveyContent(QString);
public slots:

private:
    QString textContent;    //文本框里的内容
    //QTextEdit *textedit;
    //QPushButton *confirmButton;

};

#endif // ADDNEWITEM_H
