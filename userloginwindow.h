#ifndef USERLOGINWINDOW_H
#define USERLOGINWINDOW_H

#include <QWidget>

namespace Ui {
class userLoginWindow;
}

class userLoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit userLoginWindow(QWidget *parent = 0);
    ~userLoginWindow();
signals:
    void loginSuccess();//成功登陆的信号，传递给主窗口
private slots:
    void on_registerButton_clicked();

    void on_return_button_clicked();

    void regUser();

    void sendToDateBase_regist();

    void sendToDateBase_log();


private:
    Ui::userLoginWindow *ui;
};

#endif // USERLOGINWINDOW_H
