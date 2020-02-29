#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <vector>
#include <QString>
#include <QStatusBar>
#include <QProcess>
#include <QLabel>
#include <QDebug>
#include <fstream>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/types_c.h>
#include "opencv2/imgproc.hpp"
#include <string.h>
#include <curl/curl.h>
#define FROM  "learncppuwo@gmail.com"
#define TO    "mohammad_km2006@yahoo.com"
#define CC    "mohammad_km2006@yahoo.com"

/*!
 * \brief User Interface Class
 */
namespace Ui {
class Dialog;
}

/*!
 * \brief Panel of camera access & DataBase
 */
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    void detectAndDraw(cv::Mat& img, cv::CascadeClassifier& cascade, cv::CascadeClassifier nestedCascade, double scale);
    void loadFiles();
    void sendemail();
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_add_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::Dialog *ui;
    QGraphicsView *view;
    QProcess process1;
    QGraphicsScene *scene;
    QGraphicsPixmapItem pixmap;
    cv::VideoCapture capture;
    cv::Mat frame,frame1, image;
    cv::CascadeClassifier cascade, nestedCascade;
    double scale = 1;
};

#endif // DIALOG_H
