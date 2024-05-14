#ifndef DELIVERYORDERLISTITEM_H
#define DELIVERYORDERLISTITEM_H

#include <QWidget>

namespace Ui {
class DeliveryOrderListItem;
}

class DeliveryOrderListItem : public QWidget
{
    Q_OBJECT

public:
    explicit DeliveryOrderListItem(QString orderDate, QString orderNumber, QWidget *parent = nullptr);
    ~DeliveryOrderListItem();

private:
    Ui::DeliveryOrderListItem *ui;
    QString _orderDate, _orderNumber;
};

#endif // DELIVERYORDERLISTITEM_H
