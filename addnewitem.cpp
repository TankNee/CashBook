#include "addnewitem.h"
#include<QTextEdit>
#include<QMainWindow>
#include<QPushButton>
#include<QString>
#include<QDebug>
#include<QHBoxLayout>
#include<QLineEdit>
addNewItem::addNewItem(QWidget *parent) : QWidget(parent)
{  
    this->setWindowTitle("添加新帐单");
    resize(480,320);
    //金额输入框
    QLineEdit *lineEdit = new QLineEdit(this);
    //编辑框
    QTextEdit *textedit = new QTextEdit(this);
    //确认按钮
    QPushButton *confirmButton = new QPushButton(this);
    //confirmButton->move(0,280);
    confirmButton->setText("确认");
    //布局实例
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(lineEdit);
    layout->addWidget(textedit);
    layout->addWidget(confirmButton);
    //传递信号，将文本内容传入父窗口
    connect(confirmButton,&QPushButton::clicked,
            [=](){
        textContent = textedit->toPlainText();
        qDebug()<<textContent;
        this->hide();
        emit conveyContent(textContent);
        textedit->clear();
    }

            );

}
