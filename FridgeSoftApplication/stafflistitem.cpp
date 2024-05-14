#include "stafflistitem.h"
#include "ui_stafflistitem.h"

StaffListItem::StaffListItem(QString name, QString admin, QWidget *parent) :
    _name(name), _admin(admin.toInt()), QWidget(parent),
    ui(new Ui::StaffListItem)
{
    ui->setupUi(this);
    ui->staffName->setText(_name);
    ui->adminCheckbox->setChecked(_admin);
}

StaffListItem::~StaffListItem()
{
    delete ui;
}
