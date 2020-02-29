#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \brief MainWindow::MainWindow Constructor, Create and Initialize Database text file
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->InitDB();
    this->ReadDB();

    int w = ui->label_pic->width(); // Get the width & height of text lable for displaying image
    int h = ui->label_pic->height();
    QPixmap pix(":/Res/Img.png"); // Read the Image file to display on the login page
    ui->label_pic->setPixmap(pix.scaled(w,h)); // Display the image on the login page
}

/*!
 * \brief MainWindow::~MainWindow Decosntructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::InitDB Creating & Initializing Databse text file
 */
void MainWindow::InitDB()
{
    QString filename="DataBase.txt";
    QFile file(filename);
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << "Admin " << "Admin"<< endl; //Write to DataBase text file
    }
    file.close();
    userlist.push_back("Admin");
    passlist.push_back("Admin");
}
/*!
 * \brief Get the usename and passowrd from user and check if it exists in Databse text file
 */
void MainWindow::on_pushButton_clicked()
{
    QString Username = ui->lineEdit_UserName->text(); // Get the username and password from user
    QString Password = ui->lineEdit_Password->text();

    if (
       (std::find(userlist.begin(), userlist.end(), Username) != userlist.end()) &&
       (std::find(passlist.begin(), passlist.end(), Password) != passlist.end()) )

    {
        QMessageBox::information(this,"Login", "UserName and Password is correct");
        hide(); // hide the login page
        page = new Dialog(this);
        page->show(); // show the main window
    }
    else {
        QMessageBox::information(this,"Error","Incorrect Username or Password");
    }

}
/*!
 * \brief MainWindow::ReadDB Read the Databse text file
 */
void MainWindow::ReadDB()
{
    QString filename = "DataBase.txt";
    QFile ff(filename);
    QString line;

    if(ff.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&ff);
        while(!stream.atEnd())
        {
            QStringList array;
            line = stream.readLine();
            array << line.split(" ");
            userlist.push_back(array[0]);
            passlist.push_back(array[1]);
            //qDebug() << array[0] << array[1];
        }
    }

}
