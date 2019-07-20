#include "mainwindow.h"
#include <QApplication>
#include<QSqlDatabase>
#include<QDebug>
#include<QSqlQueryModel>
#include<QSqlQuery>
bool OpenDateBase();
int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    OpenDateBase();
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("BillBoard");
    w.show();

    return a.exec();
}
bool OpenDateBase(){
    QSqlDatabase mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("cashitem.db");
    if(mydb.open())
    {
        qDebug()<<"open success";
        QSqlQuery query;
            //新建item表，id设置为主键，一个金额，一个类型，一个内容,一个时间
            query.exec("create table item(id INTEGER PRIMARY KEY AUTOINCREMENT,money int,type varchar,content varchar,addtime time,useraccount varchar)");
            query.exec("create table user(id INTEGER PRIMARY KEY AUTOINCREMENT,useraccount varchar,userpassword varchar,useremail varchar)");
            query.exec("create table user_budget_setting(id INTEGER PRIMARY KEY AUTOINCREMENT,useraccount varchar,dayBudget int,monthBudget int,yearBudget int)");
         return true;
    }
    else
    {
        qDebug()<<"open failed";
        return false;
    }
}
