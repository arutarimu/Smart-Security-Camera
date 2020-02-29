#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"

/*!
 * \brief Dialog::Dialog Constructor
 */
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scene()->addItem(&pixmap);
}

/*!
 * \brief Dialog::~Dialog Deconstructor
 */
Dialog::~Dialog()
{
    delete ui;
}

/*!
 * \brief loading XML files into Cascade classifier for face detection
 */
void Dialog::loadFiles()
{
    nestedCascade.load("/home/pi/opencv_build/opencv/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml");
    cascade.load("/home/pi/opencv_build/opencv/data/haarcascades/haarcascade_frontalface_default.xml");
}

/*!
 *\brief To capture or stop live video from USB webcam
 */
void Dialog::on_pushButton_clicked()
{
    this->loadFiles();
    if (capture.isOpened())
    {
        ui->pushButton->setText("Start");
        capture.release();
        return;
    }
    capture.open(0);
    if(capture.isOpened())
    {
        ui->pushButton->setText("Stop");
        while(1)
        {

            capture >> frame;
            if (frame.empty())
                break;
            frame1 = frame.clone();
            this->detectAndDraw(frame1, cascade,nestedCascade, scale); //!< Detect face and draw circle around it
            qApp->processEvents();
        }

    } else
        QMessageBox::about(this,"Error","camera not detected");
}

/*!
 * \brief Dialog::detectAndDraw Detect face and draw circle around it
 * \param img current frame
 * \param cascade, For Detection eye if the person wearing eyeglass
 * \param nestedCascade, For human face detection
 * \param scale, scale of image from which face is searched
 */
void Dialog::detectAndDraw(cv::Mat &img, cv::CascadeClassifier &cascade, cv::CascadeClassifier nestedCascade, double scale)
{
        std::vector<cv::Rect> faces, faces2;
        cv::Mat gray, smallImg;

        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY); // convert frame to gray scale
        double fx = 1 / scale;

        cv::resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR);
        cv::equalizeHist(smallImg, smallImg);

        cascade.detectMultiScale(smallImg, faces, 1.1,
            2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30)); // Detect faces in frame

        if (!faces.empty())
        {
            ui->label_2->setText("Face Detected"); //change the status bar into FaceDetection

            if (ui->checkBox->isChecked()) // see the checkBox
                this->sendemail(); // send email notification
        } else {
             ui->label_2->setText("");  // update the status bar
        }

        for (size_t i = 0; i < faces.size(); i++)
        {
            cv::Rect r = faces[i];
            cv::Mat smallImgROI;
            std::vector<cv::Rect> nestedObjects;
            cv::Point center;
            cv::Scalar color = cv::Scalar(255, 0, 0); // Color for Drawing tool
            int radius;

            double aspect_ratio = (double)r.width / r.height;
            if (0.75 < aspect_ratio && aspect_ratio < 1.3)
            {
                center.x = cvRound((r.x + r.width * 0.5) * scale);
                center.y = cvRound((r.y + r.height * 0.5) * scale);
                radius = cvRound((r.width + r.height) * 0.25 * scale);
                circle(img, center, radius, color, 3, 8, 0);
            }
            else
                rectangle(img, CvPoint(cvRound(r.x * scale), cvRound(r.y * scale)),
                    cvPoint(cvRound((r.x + r.width - 1) * scale),
                        cvRound((r.y + r.height - 1) * scale)), color, 3, 8, 0);
            if (nestedCascade.empty())
                continue;
            smallImgROI = smallImg(r);

            // Detection of eyes int the input image
            nestedCascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2,
                0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

            // Draw circles around eyes
            for (size_t j = 0; j < nestedObjects.size(); j++)
            {
                cv::Rect nr = nestedObjects[j];
                center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
                center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
                radius = cvRound((nr.width + nr.height) * 0.25 * scale);
                circle(img, center, radius, color, 3, 8, 0);
            }
        }
        QImage qimg(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        pixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
        ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}


/*!
 * \brief Dialog::on_pushButton_add_clicked Add UserName and Password to Databse
 */
void Dialog::on_pushButton_add_clicked()
{
    QString uu = ui->lineEdit_user->text();
    QString pp = ui->lineEdit_pass->text();

    std::ofstream fin;

    fin.open("DataBase.txt", std::ios::app);
    fin << uu.toStdString() << " " << pp.toStdString() << "\n";
    QMessageBox::about(this, "DataBase", "UserName and Password added to DataBase");
    fin.close();

}

/*!
 * \brief Dialog::on_pushButton_2_clicked Display Database into the GUI
 */
void Dialog::on_pushButton_2_clicked()
{
    QString filename = "DataBase.txt";
    QFile ff(filename);
    if (!ff.exists())
    {
        qDebug() << "File cannot be found" << filename;
    } else {
    {
        qDebug() << filename << "openning ...";
    }
    QString line;
    ui->textEdit->clear();
    if(ff.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&ff);
        while(!stream.atEnd())
        {
            line = stream.readLine();
            ui->textEdit->setText(ui->textEdit->toPlainText() + line + "\n");
            //qDebug() << "line: " << line;
        }
    }
    ff.close();
}

}

/*!
 * \brief Dialog::on_pushButton_3_clicked Hide the DataBase shown in GUI
 */
void Dialog::on_pushButton_3_clicked()
{
        ui->textEdit->clear();
}


static size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp)
{

    struct upload_status {
        int lines_read;
    };

    static const char* payload_text[] = {
      "Date: Mton, 29 Nov 2010 21:54:29 +1100\r\n",
      "To: "   "\r\n",
      "From: "  " \r\n",
      "Cc: "  " (Another example User)\r\n",
      "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
      "rfcpedant.example.org>\r\n",
      "Subject: Notification Smart Security\r\n",
      "\r\n", /* empty line to divide headers from body, see RFC5322 */
      "Person at the door.\r\n",
      "\r\n",
      NULL
    };

    struct upload_status* upload_ctx = (struct upload_status*)userp;
    const char* data;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
        return 0;
    }

    data = payload_text[upload_ctx->lines_read];

    if (data) {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;

        return len;
    }

    return 0;
}

/*!
 * \brief Dialog::sendemail() Responsible for sending email
 */

void Dialog::sendemail()
{

    struct upload_status {
        int lines_read;
    };

    CURL* curl;
    CURLcode res = CURLE_OK;
    struct curl_slist* recipients = NULL;
    struct upload_status upload_ctx;

    upload_ctx.lines_read = 0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, "learncppuwo@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "123654789@abc");
        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
        //curl_easy_setopt(curl, CURLOPT_CAINFO, "C:/repos/Email_Notif/curl-7.66.0-win64-mingw/bin/curl-ca-bundle.crt");
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);
        recipients = curl_slist_append(recipients, TO);
        recipients = curl_slist_append(recipients, CC);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }

}


