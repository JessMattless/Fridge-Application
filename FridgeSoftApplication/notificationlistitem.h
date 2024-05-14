#ifndef NOTIFICATIONLISTITEM_H
#define NOTIFICATIONLISTITEM_H

#include <QWidget>

namespace Ui {
class NotificationListItem;
}

class NotificationListItem : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationListItem(QString date, QString title, QString subline, QString read, QWidget *parent = nullptr);
    ~NotificationListItem();

private:
    Ui::NotificationListItem *ui;
    QString _date, _title, _subline;
    bool _read;
};

#endif // NOTIFICATIONLISTITEM_H
