#include "notificationlistitem.h"
#include "ui_notificationlistitem.h"

NotificationListItem::NotificationListItem(QString date, QString title, QString subline, QString read, QWidget *parent) :
    _date(date), _title(title), _subline(subline), _read(read.toInt()), QWidget(parent),
    ui(new Ui::NotificationListItem)
{
    ui->setupUi(this);
    ui->notifDate->setText(_date);
    ui->notifTitle->setText(_title);
    ui->notifSubline->setText(_subline);

    if (!_read) {
        ui->horizontalLayoutWidget->setStyleSheet("background: #DDE3EA");
    }
}

NotificationListItem::~NotificationListItem()
{
    delete ui;
}
