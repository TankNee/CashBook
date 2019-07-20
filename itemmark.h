#ifndef ITEMMARK_H
#define ITEMMARK_H

#include <QWidget>
#include<QString>
namespace Ui {
class itemMark;
}

class itemMark : public QWidget
{
    Q_OBJECT

public:
    explicit itemMark(QWidget *parent = 0);
    ~itemMark();
signals:
    void conveyData(int money,QString type,QString content);
private:
    Ui::itemMark *ui;
};

#endif // ITEMMARK_H
