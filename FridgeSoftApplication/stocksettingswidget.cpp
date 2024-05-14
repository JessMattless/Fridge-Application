#include "stocksettingswidget.h"
#include "ui_stocksettingswidget.h"

StockSettingsWidget::StockSettingsWidget(QString productName, QString productQuantity, QWidget *parent) :
    _productName(productName), _productQuantity(productQuantity), QWidget(parent),
    ui(new Ui::StockSettingsWidget)
{
    ui->setupUi(this);
    ui->productName->setText(_productName);
    ui->quantity->setText(_productQuantity);
}

StockSettingsWidget::~StockSettingsWidget()
{
    delete ui;
}
