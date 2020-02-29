/*!
 * \authors Mohammad Karami, Deok Kim, Junyoung Kim, Malachi Kitsa, Harish Nagallapati (Group 43)
 * \version 1.0
 * \date 2019-11-28
 * \copyright GNU Public License
 * \mainpage Smart Security
 * \section intro_sec Introduction
 * Smart security camera sends email once a face is detected. One of its application is home safety.
 * \subsection Installation
 * Needs the Following Packages:
 * - OpenCV--> For computer vision purpose,
 * -Libcurl--> For email notification
 * \subsection Hardware Requirement
 * Raspberry Pi & USB Webcam
 */
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
