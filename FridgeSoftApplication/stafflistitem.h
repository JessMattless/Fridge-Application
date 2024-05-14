#ifndef STAFFLISTITEM_H
#define STAFFLISTITEM_H

#include <QWidget>

namespace Ui {
class StaffListItem;
}

class StaffListItem : public QWidget
{
    Q_OBJECT

public:
    explicit StaffListItem(QString name, QString admin, QWidget *parent = nullptr);
    ~StaffListItem();

private:
    Ui::StaffListItem *ui;
    QString _name;
    bool _admin;
};

#endif // STAFFLISTITEM_H
