#include "itemmark.h"
#include "ui_itemmark.h"
#include<QString>
itemMark::itemMark(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::itemMark)
{
    ui->setupUi(this);
    connect(ui->cancel_btn,&QPushButton::clicked,this,&itemMark::close);
    connect(ui->confirm_btn,&QPushButton::clicked,
            [=](){
        int moneyNumber = ui->moneyLine->text().toInt();
        QString typeName = ui->comboBox->currentText();
        QString allOfContent = ui->content->toPlainText();
        emit conveyData(moneyNumber,typeName,allOfContent);
        this->hide();
        ui->moneyLine->clear();
        //ui->comboBox->clear();
        ui->content->clear();
    }
            );
}

itemMark::~itemMark()
{
    delete ui;
}
