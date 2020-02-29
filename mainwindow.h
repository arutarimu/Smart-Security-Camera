#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <vector>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/types_c.h>
#include "opencv2/imgproc.hpp"
#include <QString>
#include "dialog.h"
#include <algorithm>
#include <QFile>
#include <QPixmap>


namespace Ui {
class MainWindow;
}

/*!
 * \brief Sign in panel
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    /*!
     * \brief List of user names in database
     */
    std::vector<QString> userlist;
    /*!
     * \brief List of passwords in database
     */
    std::vector<QString> passlist;
    void InitDB();
    void ReadDB();
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Dialog *page;
};

#endif // MAINWINDOW_H
