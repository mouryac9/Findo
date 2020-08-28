#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHeaderView>
#include <QFile>
#include <QMessageBox>
#include <QTableView>
#include <QSqlTableModel>
#include <QDebug>
#include <bits/stdc++.h>
#include <QtSql/QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDir>

#define pi 3.14159265358979323846
#define earth_radius 6371.0


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace std;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

double fRand(double fMin, double fMax);
double degtorad(double deg);
double distanceEarth(double lat2d, double lon2d);
struct json
{
    //To save hospital's id, latitude, longitude respectively
    double id_as_int, lat2d, lon2d;

    //Hospital's name
    string name;

public:
    void json_parser(QSqlDatabase);

private:
    void distance_calculator(QSqlTableModel*);
};

#endif // MAINWINDOW_H
