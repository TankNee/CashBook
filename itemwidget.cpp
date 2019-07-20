#include "itemwidget.h"
#include<QPushButton>
#include<QLabel>
#include<QHBoxLayout>
#include<QDebug>
#include<QLineEdit>
#include<QString>
#include<QCheckBox>
itemWidget::itemWidget(int money,QString typeName,QString content,QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *itemLayout = new QHBoxLayout(this);
    //QPushButton *itemEdit = new QPushButton(this);
    //QPushButton *itemDel = new QPushButton(this);
    //QCheckBox *checkBox = new QCheckBox(this);
    //itemEdit->setText("编辑");
    //itemDel->setText("删除");
    QLabel *itemLabel1 = new QLabel(this);
    QLabel *itemLabel2 = new QLabel(this);
    QLabel *itemLabel3 = new QLabel(this);
    QString tempMoney;
    tempMoney=QString("%1").arg(money);
    itemLabel1->setText(tempMoney);
    itemLabel2->setText(typeName);
    itemLabel3->setText(content);
    qDebug()<<"2"<<content;
    //itemLayout->addWidget(checkBox);
    itemLayout->addWidget(itemLabel1);
    itemLayout->addWidget(itemLabel2);
    itemLayout->addWidget(itemLabel3);
    //itemLayout->addWidget(itemEdit);
    //itemLayout->addWidget(itemDel);
    //itemLayout
    //connect(itemDel,&QPushButton::clicked,this,&itemWidget::Del_btn_clicked);
}
void itemWidget::setContent(QString str){

    itemContent=str;
    qDebug()<<"1"<<itemContent;
}
void itemWidget::Edit_btn_clicked(){

}
void itemWidget::Del_btn_clicked(){
    emit DELET();
}

