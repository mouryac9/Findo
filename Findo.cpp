#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton_2_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");

       db.setDatabaseName("D:/Hospdb.db");
       if(db.open()){
           qDebug() << "success";
       }

    json obj;
    obj.json_parser(db);

}

double degtorad(double deg)
{
    return ( deg * pi / 180);
}

double distanceEarth(double lat2d, double lon2d)
{
    double lat1, lon1, lat2, lon2,
           delta_lon, central_ang;

    lat1 = degtorad(lat1d);
    lon1 = degtorad(lon1d);
    lat2 = degtorad(lat2d);
    lon2 = degtorad(lon2d);

    delta_lon = lon2 - lon1;

    // great circle distance formula.
    central_ang = acos ( sin(lat1) *
                  sin(lat2) + cos(lat1) *
                  cos(lat2) * cos(delta_lon) );

    return (earth_radius * central_ang);
}

    void json::distance_calculator(QSqlTableModel* model)
    {
        QString n=name.c_str();
       double dist = distanceEarth(lat2d, lon2d);

        if (dist <= 50.0000)
        {
            QSqlRecord record = model->record();

                record.setValue(0, QVariant(id_as_int) );
                record.setValue(1, QVariant(n));
                record.setValue(2, QVariant(dist));
                /*-1 is set to indicate that it will be added to the last row*/
                if(model->insertRecord(-1, record)){
                    qDebug()<<"successful insertion";
                    model->submitAll();
                }
        }
    }

    void json:: json_parser(QSqlDatabase db)
    {
        QSqlQuery query(db);
        query.prepare("SELECT * from hinfo");
        if(!query.exec()){ qDebug()<<"not got query";}
        QSqlTableModel *model = new QSqlTableModel();
        model->setTable("Available");

            model->select();
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("Distance(in kms)"));
         while (query.next())

         {
               latitude_as_int= query.value(0).toDouble();
               longitude_as_int=query.value(3).toDouble();
               id_as_int=query.value(1).toInt();
               name=query.value(2).toString().toUtf8().constData();

                lat2d = latitude_as_int;
                lon2d = longitude_as_int;
                distance_calculator(model);
         }
         QTableView *view = new QTableView;
            view->setModel(model);
            view->hideColumn(3);
            view->show();


        // closing stream of customer's file.
        db.close();

    }

