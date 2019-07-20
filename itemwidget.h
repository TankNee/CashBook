#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QWidget>
#include<QPushButton>
#include<QLabel>
#include<QString>
class itemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit itemWidget(int money,QString typeName,QString content,QWidget *parent = nullptr);

    void setContent(QString);
    int itemMoney;
    QString itemTypeName;
    QString itemContent;
signals:
    void DELET();
public slots:
private:
    void Edit_btn_clicked();
    void Del_btn_clicked();
};

#endif // ITEMWIDGET_H
