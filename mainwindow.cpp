#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMainWindow>
#include<QString>
#include<QLabel>
#include<QSqlDatabase>
#include<QDebug>
#include<QSqlQueryModel>
#include<QSqlQuery>
#include<QPushButton>
#include<QWidget>
#include<QHBoxLayout>
#include<QInputDialog>
#include"itemwidget.h"
#include<QDateTime>
#include<QListWidgetItem>
#include<QListView>
#include<QFile>
#include<QIODevice>
#include<QByteArray>
#include<QBarCategoryAxis>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tagWidgetLayout.setParent(ui->dailyBoardContent);
    connect(&user_login,&userLoginWindow::loginSuccess,this,&MainWindow::user_login_sucess);
    connect(&itemMark,&itemMark::conveyData,this,&MainWindow::addNewItemLabel);
    ui->edit_pushButton->hide();
    ui->delet_pushButton_2->hide();
}

MainWindow::~MainWindow()
{
    QSqlQuery query;//建立一个数据库的query
    QString sql1;
    sql1="insert into user_login(id,useraccount)values(NULL,'') ";
    query.exec(sql1);
    query.exec("select  id,useraccount from user_login where id >= 0");
    while (query.next()) {
        QString id = query.value(0).toString();
        useraccount = query.value(1).toString();
        qDebug()<<"mainwindow:"<<id<<useraccount;
    }
    delete ui;
}

void MainWindow::on_addItemButton_clicked()
{
    //ui->tabWidget->hide();
    itemMark.show();
}

void MainWindow::addNewItemLabel(int money ,QString type,QString content){
    newWidget = new itemWidget(money,type,content,ui->itemListWidget);
    QSqlQuery query1;
    query1.exec("select  id,useraccount from user_login where id >= 0");
    while (query1.next()) {
        QString id = query1.value(0).toString();
        useraccount = query1.value(1).toString();
        qDebug()<<id<<useraccount;
    }

    if(content!=NULL){
        QDateTime dtm;
        QString timestr = dtm.currentDateTime().toString("yyyyMMdd");
        QSqlQuery query;//建立一个数据库的query
        QString sql1;
        sql1=QString("insert into item(id, money, type,content,addtime,useraccount) values(NULL,'%1', '%2', '%3','%4','%5')").arg(money).arg(type).arg(content).arg(timestr).arg(useraccount);
        query.exec(sql1);
        qDebug()<<money<<type<<content<<timestr<<useraccount;
        QListWidgetItem *listwidgetitem = new QListWidgetItem(ui->itemListWidget,0);
        listwidgetitem->setSizeHint(QSize(50,50));
        ui->itemListWidget->setItemWidget(listwidgetitem,newWidget);
        ui->itemListWidget->show();
    }

}
void MainWindow::deletItem(){
    QListWidgetItem *listwidgetitem= ui->itemListWidget->currentItem();
    listwidgetitem->setHidden(true);
}

void MainWindow::on_userAvatarPic_clicked()
{
    user_login.show();
}

void MainWindow::on_dailyBoardButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_chartAnalysisButton_clicked()
{
    int i = ui->monthSelector->currentIndex();
    chartsMaking(i+1);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_bugetSettingButton_clicked()
{
    QSqlQuery query_user;
    query_user.exec("select  id,useraccount from user_login where id >= 0");
    while(query_user.next()){
        useraccount = query_user.value(1).toString();
    //    qDebug()<<useraccount;
    }

    ui->stackedWidget->setCurrentIndex(2);

    QSqlQuery query;
    QDateTime dtm;
    QString timeNow=dtm.currentDateTime().toString("yyyyMMdd");
    int year = timeNow.toInt();
 //   int day = year % 100;
    year/=100;
 //   int month = year % 100;
    year/=100;
    int thisMonthCost(0),thisDayCost(0),thisYearCost(0);
    QString monthNow = timeNow.remove(6,7);
    timeNow=dtm.currentDateTime().toString("yyyyMMdd");
    QString sql =QString("select  money,addtime,useraccount from item where addtime between '%1%2%3'and '%4%5%6'").arg(year).arg("01").arg("01").arg(year+1).arg("01").arg("00");
    query.exec(sql);
    while(query.next()){
        if (query.value(2).toString() == useraccount) {
        thisYearCost+=query.value(0).toInt();
        if (query.value(1).toString().contains(monthNow,Qt::CaseInsensitive))  thisMonthCost+=query.value(0).toInt();
        if (query.value(1).toString() == timeNow ) thisDayCost+=query.value(0).toInt();
        }
    }
    //qDebug()<<thisDayCost;
    //qDebug()<<thisMonthCost;
    //qDebug()<<thisYearCost;

    sql = QString("select dayBudget,monthBudget,yearBudget from user_budget_setting where useraccount = '%1'").arg(useraccount);
    query.exec(sql);
    int dayBudget(0),monthBudget(0),yearBudget(0);
    while(query.next()){
         qDebug()<<query.value(0).toInt();
         qDebug()<<query.value(1).toInt();
         qDebug()<<query.value(2).toInt();
         dayBudget=query.value(0).toInt();
         monthBudget=query.value(1).toInt();
         yearBudget=query.value(2).toInt();
    }
    
    if (thisDayCost>dayBudget)//TODO 增加弹窗提示
    {
        QMessageBox message(QMessageBox::NoIcon, "whoops！", "今日预算超支");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_what the --.png"));
        message.exec();
        ui->dayprogressBar->setMaximum(thisDayCost);
    }
    else ui->dayprogressBar->setMaximum(dayBudget);

    if (thisMonthCost>monthBudget)//TODO 增加弹窗提示
    {
        QMessageBox message(QMessageBox::NoIcon, "whoops！", "本月预算超支");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_what the --.png"));
        message.exec();
        ui->monthprogressBar->setMaximum(thisMonthCost);
    }
    else ui->monthprogressBar->setMaximum(monthBudget);

    if (thisYearCost>yearBudget)//TODO 增加弹窗提示
    {
        QMessageBox message(QMessageBox::NoIcon, "whoops！", "今年预算超支");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_what the --.png"));
        message.exec();
        ui->yearprogressBar->setMaximum(thisYearCost);
    }
    else ui->yearprogressBar->setMaximum(yearBudget);
    
    ui->dayprogressBar->setValue(thisDayCost);
    ui->monthprogressBar->setValue(thisMonthCost);
    ui->yearprogressBar->setValue(thisYearCost);


    QString daylabel = QString("每日预算：%1/%2").arg(thisDayCost).arg(dayBudget);
    QString monthlabel = QString("每月预算：%1/%2").arg(thisMonthCost).arg(monthBudget);
    QString yearlabel = QString("每年预算：%1/%2").arg(thisYearCost).arg(yearBudget);
    ui->dayBudgetSetlabel->setText(daylabel);
    ui->monthBudgetSetlabel->setText(monthlabel);
    ui->yearBudgetSetlabel->setText(yearlabel);
}

void MainWindow::on_dayBudgetSetButton_clicked()
{
   //接收新的预算
   int dayBudget;
   dayBudget = QInputDialog::getInt(this,"设置","每日预算：",0);
    QSqlQuery query;
    QSqlQueryModel *model_2 = new QSqlQueryModel;
    QString sql = QString("select count (*) from user_budget_setting where useraccount = '%1'").arg(useraccount);
    model_2->setQuery(sql);
    QModelIndex index1 = model_2->index(0,0);
    if (index1.data()==0){//插入一条新记录
        sql = QString("insert into user_budget_setting(id,dayBudget,monthBudget,yearBudget,useraccount) values(NULL,'%1','%2','%3','%4')").arg(dayBudget).arg(0).arg(0).arg(useraccount);
        qDebug()<<useraccount;
        query.exec(sql);
    }
    else { //更新记录
        //Debug
        qDebug()<<"更新成功";
        sql = QString("update user_budget_setting set dayBudget = '%1' where useraccount = '%2'").arg(dayBudget).arg(useraccount);
        query.exec(sql);
    }
   //更新预算进度标签
   QString daylabel = QString("每日预算：%1/%2").arg(ui->dayprogressBar->value()).arg(dayBudget);
   ui->dayBudgetSetlabel->setText(daylabel);
   if (ui->dayprogressBar->value()>dayBudget)//TODO 增加弹窗提示
   {

       ui->dayprogressBar->setMaximum(ui->dayprogressBar->value());
   }
   else ui->dayprogressBar->setMaximum(dayBudget);
   ui->dayBudgetSetlabel->show();

}

void MainWindow::on_monthBudgetSetButton_clicked()
{
    //接收新的预算
    int monthBudget;
    monthBudget = QInputDialog::getInt(this,"设置","每月预算：",0);
    //TODO添加数据库操作
    QSqlQuery query;
    QSqlQueryModel *model_2 = new QSqlQueryModel;
    QString sql = QString("select count (*) from user_budget_setting where useraccount = '%1'").arg(useraccount);
    model_2->setQuery(sql);
    QModelIndex index1 = model_2->index(0,0);
    if (index1.data()==0){//插入一条新记录
        sql = QString("insert into user_budget_setting(id,dayBudget,monthBudget,yearBudget,useraccount) values(NULL,'%1','%2','%3','%4')").arg(0).arg(monthBudget).arg(0).arg(useraccount);
        qDebug()<<useraccount;
        query.exec(sql);
    }
    else { //更新记录
        //Debug
        qDebug()<<"更新成功";
        sql = QString("update user_budget_setting set monthBudget = '%1' where useraccount = '%2'").arg(monthBudget).arg(useraccount);
        query.exec(sql);
    }

    //更新预算进度标签
    QString monthlabel = QString("每月预算：%1/%2").arg(ui->monthprogressBar->value()).arg(monthBudget);
    ui->monthBudgetSetlabel->setText(monthlabel);
    if (ui->monthprogressBar->value()>monthBudget)//TODO 增加弹窗提示
    {
        ui->monthprogressBar->setMaximum(ui->monthprogressBar->value());
    }
    else ui->monthprogressBar->setMaximum(monthBudget);

    ui->monthBudgetSetlabel->show();
}

void MainWindow::on_yearBudgetSetButton_clicked()
{
    //接收新的预算
    int yearBudget;
    yearBudget = QInputDialog::getInt(this,"设置","每年预算：",0);
    //TODO添加数据库操作
    QSqlQuery query;
    QSqlQueryModel *model_2 = new QSqlQueryModel;
    QString sql = QString("select count (*) from user_budget_setting where useraccount = '%1'").arg(useraccount);
    model_2->setQuery(sql);
    QModelIndex index1 = model_2->index(0,0);
    if (index1.data()==0){//插入一条新记录
        sql = QString("insert into user_budget_setting(id,dayBudget,monthBudget,yearBudget,useraccount) values(NULL,'%1','%2','%3','%4')").arg(0).arg(0).arg(yearBudget).arg(useraccount);
        qDebug()<<useraccount;
        query.exec(sql);
    }
    else { //更新记录
        //Debug
        qDebug()<<"更新成功";
        sql = QString("update user_budget_setting set yearBudget = '%1' where useraccount = '%2'").arg(yearBudget).arg(useraccount);
        query.exec(sql);
    }
    //更新预算进度标签
    QString yearlabel = QString("每年预算：%1/%2").arg(ui->yearprogressBar->value()).arg(yearBudget);
    ui->yearBudgetSetlabel->setText(yearlabel);
    if (ui->yearprogressBar->value()>yearBudget)//TODO 增加弹窗提示
    {
        ui->yearprogressBar->setMaximum(ui->yearprogressBar->value());
    }
    else ui->yearprogressBar->setMaximum(yearBudget);
    ui->yearBudgetSetlabel->show();
}


void MainWindow::on_monthSelector_activated(int index)
{
    int x = index;
    x++;
    chartsMaking(x);
}

//饼状图生成函数
void MainWindow::chartsMaking(int month)
{
    QSqlQuery query1;
    query1.exec("select  id,useraccount from user_login where id >= 0");
    while (query1.next()) {
        QString id = query1.value(0).toString();
        useraccount = query1.value(1).toString();
        qDebug()<<"BarMaking:"<<id<<useraccount;
    }//获取当前用户账号
    QSqlQuery query;
    QString sql =QString("select  money, type from item where useraccount = '%1'and addtime between '20190%2%3'and '20190%4%5'").arg(useraccount).arg(month).arg("01").arg(month+1).arg("00") ;
    query.exec(sql);
    int eatMoney,entertainmentMoney,eduMoney;
    eatMoney=0;
    entertainmentMoney = 0;
    eduMoney=0;
    //int eatMoney_per,entertainmentMoney_per,eduMoney_per;
    while (query.next()) {
        if(query.value(1).toString()=="饮食"){
             eatMoney+=query.value(0).toInt();
        }else if(query.value(1).toString()=="娱乐"){
             entertainmentMoney+=query.value(0).toInt();
        }else if(query.value(1).toString()=="教育"){
             eduMoney+=query.value(0).toInt();
        }
    }
    qDebug()<<eatMoney;
    qDebug()<<entertainmentMoney;
    qDebug()<<eduMoney;
    qDebug()<<month;
    QPieSeries *series = new QPieSeries();
    series->append("饮食",eatMoney);
    series->append("娱乐",entertainmentMoney);
    series->append("教育",eduMoney);
    series->setLabelsVisible();

    /*QPieSlice *slice_red = series->slices().at(0);
    QPieSlice *slice_green = series->slices().at(1);
    QPieSlice *slice_blue = series->slices().at(2);
    slice_red->setColor(QColor(255,0,0,255));
    slice_green->setColor(QColor(0,255,0,255));
    slice_blue->setColor(QColor(0,0,255,255));*/

    QChart *chart = new QChart();
    chart->addSeries(series);
    QString title = QString("您的%1月的账单统计图").arg(month);
    chart->setTitle(title);
    chart->legend()->hide();
    ui->chartViewer->setChart(chart);
    //QChartView *chartview = new QChartView(chart);
    //chartview->setRenderHint(QPainter::Antialiasing);

    //chartview->resize(400,400);
    //chartview->show();

    //ui->graphicsView->setChart(chart);
    //ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

//柱状图生成函数
void MainWindow::barChartsMaking(int month){
    QSqlQuery query1;
    query1.exec("select  id,useraccount from user_login where id >= 0");
    while (query1.next()) {
        QString id = query1.value(0).toString();
        useraccount = query1.value(1).toString();
        qDebug()<<"BarMaking:"<<id<<useraccount;
    }//获取当前用户账号
    QStringList category;

    QSqlQuery query;
    QString sql =QString("select  money, type from item where useraccount = '%1'and addtime between '20190%2%3'and '20190%4%5'").arg(useraccount).arg(month).arg("01").arg(month+1).arg("00") ;
    query.exec(sql);
    QBarSet *set = new QBarSet("总览");
    QBarSet *setEntertainment = new QBarSet("娱乐");
    QBarSet *setEdu = new QBarSet("教育");
    int eatMoney,entertainmentMoney,eduMoney;
    eatMoney=0;
    entertainmentMoney = 0;
    eduMoney=0;
    //int eatMoney_per,entertainmentMoney_per,eduMoney_per;
    while (query.next()) {
        if(query.value(1).toString()=="饮食"){
             eatMoney+=query.value(0).toInt();
        }else if(query.value(1).toString()=="娱乐"){
             entertainmentMoney+=query.value(0).toInt();
        }else if(query.value(1).toString()=="教育"){
             eduMoney+=query.value(0).toInt();
        }
    }
    *set<<eatMoney<<entertainmentMoney<<eduMoney;
    *setEntertainment<<entertainmentMoney;
    *setEdu<<eduMoney;
    /*while(query.next()){
        int i=1;
        if(query.value(1).toString()=="饮食"){
            *setEat<<query.value(0).toInt();
            //*setEntertainment<<0;
            //*setEdu<<0;
            category<<"饮食";
        }else if(query.value(1).toString()=="娱乐"){
            //*setEat<<0;
            *setEntertainment<<query.value(0).toInt();
            //*setEdu<<0;
            category<<"娱乐";
        }else if(query.value(1).toString()=="教育"){
            //*setEat<<0;
            //*setEntertainment<<0;
            *setEdu<<query.value(0).toInt();
            category<<"教育";
        }

    }*/
    category<<"饮食"<<"娱乐"<<"教育";
    QBarSeries *series = new QBarSeries();
    series->append(set);
    //series->append(setEntertainment);
    //series->append(setEdu);
    QChart *chart = new QChart();
    QString title = QString("您的%1月的账单统计图").arg(month);
    chart->setTitle(title);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(category);
    chart->createDefaultAxes();

    chart->addSeries(series);
    chart->setAxisX(axis,series);
    ui->chartViewer->setChart(chart);
}

void MainWindow::on_delet_pushButton_2_clicked()
{
    QListWidgetItem *listwidgetitem= ui->itemListWidget->currentItem();
    listwidgetitem->setHidden(true);
    QSqlQuery query;
    int curRow = ui->itemListWidget->currentIndex().row();     //鼠标选择删除第几行
        QModelIndex index = ui->itemListWidget->currentIndex();
        int id=index.sibling(curRow,0).data().toInt();
        QString str =QString("delete from item where id = '%1'").arg(curRow) ;
        query.exec(str);
}
//编辑按钮
void MainWindow::on_edit_pushButton_clicked()
{
    QSqlQuery query;
    query.exec("select id,money,type,content,addtime from item where id >= 1");
    QString str;
    QString str1;
    QString str2;
    QString str3;
    QString str4;
    QString str5;
    QDateTime dtm;
    int dtm0;
    while(query.next())
    {
        //query.value(0)是id的值，将其转换为int型

        //QString str0 = dtm.toString("yyyy-mm-dd hh:mm:ss");
        str1 = query.value(0).toString();
        str2 = query.value(1).toString();
        str3 = query.value(2).toString();
        str4 = query.value(3).toString();
        str5 = query.value(4).toString();
        //输出两个值
        qDebug() << str1<< str2<< str3<< str4<<str5 ;
    }
}

void MainWindow::on_clearButton_clicked()
{
    QSqlQuery query;
    query.exec("drop table item");
    query.exec("drop table user");
    query.exec("drop table user_login");
    query.exec("drop table user_buget_setting");
    query.exec("create table item(id INTEGER PRIMARY KEY AUTOINCREMENT,money int,type varchar,content varchar,addtime time,useraccount varchar)");
    query.exec("create table user(id INTEGER PRIMARY KEY AUTOINCREMENT,useraccount varchar,userpassword varchar,useremail varchar)");
    query.exec("create table user_login(id INTEGER PRIMARY KEY AUTOINCREMENT ,useraccount varchar)");
    query.exec("create table user_budget_setting(id INTEGER PRIMARY KEY AUTOINCREMENT ,useraccount varchar,dayBudget int,monthBudget int,yearBudget int)");
    ui->itemListWidget->clear();
}
void MainWindow::user_login_sucess(){

    QSqlQuery query1;
    query1.exec("select  id,useraccount from user_login where id >= 0");
    while (query1.next()) {
        QString id = query1.value(0).toString();
        useraccount = query1.value(1).toString();
        qDebug()<<"mainwindow:"<<id<<useraccount;
    }
    qDebug()<<useraccount;
    QString sql = QString("select id,money,type,content,addtime,useraccount from item where useraccount = '%1'").arg(useraccount);
    query1.exec(sql);
    ui->itemListWidget->clear();
    while (query1.next()) {
        QString id = query1.value(0).toString();
        int money = query1.value(1).toInt();
        QString type = query1.value(2).toString();
        QString content = query1.value(3).toString();
        useraccount = query1.value(5).toString();
        newWidget = new itemWidget(money,type,content,ui->itemListWidget);
        QListWidgetItem *listwidgetitem = new QListWidgetItem(ui->itemListWidget,0);
        listwidgetitem->setSizeHint(QSize(50,50));
        ui->itemListWidget->setItemWidget(listwidgetitem,newWidget);
        ui->itemListWidget->show();
        qDebug()<<money<<type<<content<<useraccount;
    }
}


void MainWindow::on_selectPieChart_clicked()
{
    int index;
    index = ui->monthSelector->currentIndex();
    chartsMaking(index+1);
}

void MainWindow::on_selectLinerChart_clicked()
{
    int index;
    index = ui->monthSelector->currentIndex();
    barChartsMaking(index+1);
}

void MainWindow::on_selectLinerChart_pressed()
{
    ui->selectLinerChart->setStyleSheet("QPushButton {"
                                     "  background: rgb(169, 169, 169);"
                                     "   border: 1px solid #8f8f91;"
                                     "   border-radius: 6px;"
                                    "}");
    ui->selectPieChart->setStyleSheet("QPushButton {"
                                     "   border: 1px solid #8f8f91;"
                                     "   border-radius: 6px;"
                                    "}");
}


void MainWindow::on_selectPieChart_pressed()
{
    ui->selectPieChart->setStyleSheet("QPushButton {"
                                     "  background: rgb(169, 169, 169);"
                                     "   border: 1px solid #8f8f91;"
                                     "   border-radius: 6px;"
                                    "}");
    ui->selectLinerChart->setStyleSheet("QPushButton {"
                                     "   border: 1px solid #8f8f91;"
                                     "   border-radius: 6px;"
                                    "}");
}
