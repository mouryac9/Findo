#include "mainwindow.h"
#include "ui_mainwindow.h"

QString path=QDir :: currentPath();


//To get user's name
QString user;

//User's location variables
double lat1d=0, lon1d=0;

//Assign sqlite database to variable
QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    //Close Connection
    db.close();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //Takes users name
    user=ui->lineEdit->text();
qDebug()<<path;
    //Takes random location
    lat1d=fRand(11.0000000,14.0000000);
    lon1d=fRand(76.0000000,78.0000000);
    ui->latitude->setNum(lat1d);
    ui->longitude->setNum(lon1d);
}

void MainWindow::on_pushButton_2_clicked()
{
    //Checks for location
    if(lat1d==0&&lon1d==0){
        QMessageBox msg;
        msg.setText("Get your Location first!");
        msg.exec();
    }
    else{
        db.setDatabaseName(path+"/Hospdb.db");

        //Checks if connection is open
        if(!db.open()){
            QMessageBox msg;
            msg.setText("Could not connect to databse.  ");
            msg.exec();
        }
        else{
            //Creates an object for struct json and calls function
            json obj;
            obj.json_parser(db);
        }
    }
}

//To get a random no. of type double between 2 given numbers
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / (double)RAND_MAX;
    return fMin + f * (fMax - fMin);
}

//Changes degree to radian
double degtorad(double deg)
{
    return ( deg * pi / 180);
}

//Gives distance between user and hospitals
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

//inserts records of hospitals within 50km radius
void json::distance_calculator(QSqlTableModel* model)
{
   double dist = distanceEarth(lat2d, lon2d);

   if (dist <= 50.0000)
   {
       QSqlRecord record = model->record();

       record.setValue(0, QVariant(id_as_int) );
       record.setValue(1, QVariant(name.c_str()));
       record.setValue(2, QVariant(dist));

       //-1 is set to indicate that it will be added to the last row
       if(model->insertRecord(-1, record)){
           model->submitAll();
       }
       else{
           QMessageBox msg;
           msg.setText("Could not insert record. ");
           msg.exec();
       }
   }
}

//Parses through all the records and outputs the required records
void json:: json_parser(QSqlDatabase db){

    QSqlQuery query(db);

    //deletes any prev. info in the output table
    query.prepare("DELETE from Available");
    query.exec();

    //Gets all records from input table
    query.prepare("SELECT * from hinfo");

    //Checks if query executed successfuly
    if(!query.exec()){
        QMessageBox msg;
        msg.setText("Could not execute query.");
        msg.exec();
     }
     else{

         //QSqlModel to set the table format
         QSqlTableModel *model = new QSqlTableModel();
         model->setTable("Available");
         model->select();
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
         model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
         model->setHeaderData(2, Qt::Horizontal, QObject::tr("Distance(in kms)"));

         while (query.next()){

             //Latitude of hospital
             lat2d= query.value(0).toDouble();

             //Longitude of hospital
             lon2d=query.value(3).toDouble();

             id_as_int=query.value(1).toInt();
             name=query.value(2).toString().toUtf8().constData();
             distance_calculator(model);
         }

         //QTableview variable inputs 'model' and sets the table view
         QTableView *view = new QTableView;
         view->setModel(model);

         //Checks if table is empty
         if(model->rowCount()==0){
             QMessageBox msg;
             msg.setText("Sorry "+user+"! No hospitals found near you.");
             msg.exec();
         }
         else{
             view->setWindowTitle("Near " + user);
             view->resizeColumnsToContents();

             //To set size of QTableView correctly
             int w=view->verticalHeader()->width()+4;
             for(int i=0;i<4;i++){
                 w+= view->columnWidth(i);
             }
             int h=view->horizontalHeader()->height()+4;
             for(int i=0;i<model->rowCount();i++){
                 h+= view->rowHeight(i);
             }

             view->resize(w,h);
             view->show();
         }
     }
}


