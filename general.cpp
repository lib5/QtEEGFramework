#include "general.h"
#include"ui_General.h"
General::General(QWidget *parent)
    : QWidget{parent}, ui(new Ui::General)
{
    ui->setupUi(this);
}

void General::initHdr(const edf_hdr_struct *hdr)
{
    if (hdr) {
        // Convert numeric fields to QString
        QString startDate = QString("%1/%2/%3")
                                .arg(hdr->startdate_day, 2, 10, QLatin1Char('0'))
                                .arg(hdr->startdate_month, 2, 10, QLatin1Char('0'))
                                .arg(hdr->startdate_year);

        QString startTime = QString("%1:%2:%3.%4")
                                .arg(hdr->starttime_hour, 2, 10, QLatin1Char('0'))
                                .arg(hdr->starttime_minute, 2, 10, QLatin1Char('0'))
                                .arg(hdr->starttime_second, 2, 10, QLatin1Char('0'))
                                .arg(hdr->starttime_subsecond, 7, 10, QLatin1Char('0'));

        QString fileDuration = QString::number(hdr->file_duration);
        QString datarecordDuration = QString::number(hdr->datarecord_duration);
        QString datarecordsInFile = QString::number(hdr->datarecords_in_file);

        // Set text for QLineEdit controls
        ui->lineEdit->setText(QString(hdr->patient));          // Patient
        ui->lineEdit_2->setText(QString(hdr->recording));      // Recording
        ui->lineEdit_3->setText(startDate);                    // Start Date
        ui->lineEdit_4->setText(startTime);                    // Start Time
        ui->lineEdit_5->setText(fileDuration);                 // File Duration
        ui->lineEdit_6->setText(datarecordDuration);           // Datarecord Duration
        ui->lineEdit_7->setText(datarecordsInFile);            // Datarecords in File
        ui->lineEdit_8->setText(QString(hdr->patientcode));    // Patient Code
        ui->lineEdit_9->setText(QString(hdr->gender));         // Gender
    }
}
