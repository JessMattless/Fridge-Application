#include "deliveryorderlistitem.h"
#include "ui_deliveryorderlistitem.h"

DeliveryOrderListItem::DeliveryOrderListItem(QString orderDate, QString orderNumber, QWidget *parent) :
    _orderDate(orderDate), _orderNumber(orderNumber), QWidget(parent),
    ui(new Ui::DeliveryOrderListItem)
{
    ui->setupUi(this);
    ui->orderDate->setText(_orderDate);
    ui->orderNumber->setText("Order Number: " + _orderNumber);
}

DeliveryOrderListItem::~DeliveryOrderListItem()
{
    delete ui;
}
