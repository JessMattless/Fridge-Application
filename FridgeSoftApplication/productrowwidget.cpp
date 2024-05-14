#include "productrowwidget.h"
#include "ui_productrowwidget.h"

ProductRowWidget::ProductRowWidget(QString productName, QString quantity, QWidget *parent) :
    QWidget(parent), _productName(productName), _quantity(quantity),
    ui(new Ui::ProductRowWidget)
{
    ui->setupUi(this);
    ui->productName->setText(_productName);
    ui->quantity->setText(_quantity);

    _startingQuantity = _quantity.toInt();

}

ProductRowWidget::~ProductRowWidget()
{
    delete ui;
}

// Example Initialization.
// QString value1 = "Hello";
// QString value2 = " World!";
// ProductRowWidget productRowWidgetInstance1(value1, value2);
// productRowWidgetInstance.show();
// return a.exec();

void ProductRowWidget::on_addItem_clicked()
{
    //TODO: For some reason I can't get the widget to update
    // its text and I'm not sure why.
    //qDebug("%d", this->updatesEnabled());
    // Get the current displayed quantity from the widget and increment it by 1
//    ui->productName->setText("");//QString::number(ui->quantity->text().toInt() + 1));
    this->update();
    emit requestStockAddition(ui->productName->text());
}


void ProductRowWidget::on_removeItem_clicked()
{
    int productQuantity = ui->quantity->text().toInt();
    if (productQuantity > 0) {
        // Get the current displayed quantity from the widget and decrement it by 1
        ui->quantity->setText(QString::number(productQuantity - 1));
        this->update();
        emit requestStockRemoval(ui->productName->text());
    }
}

void ProductRowWidget::updateStartingQuantity() {
    _startingQuantity = ui->quantity->text().toInt();
}

void ProductRowWidget::resetDisplayQuantity() {
    ui->quantity->setText(QString::number(_startingQuantity));
    this->update();
}
