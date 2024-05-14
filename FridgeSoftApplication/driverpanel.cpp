#include "driverpanel.h"
#include "ui_driverpanel.h"

#include <utils.h>

DriverPanel::DriverPanel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DriverPanel)
{
    ui->setupUi(this);

    this->setWindowTitle("Delivery Panel");
    this->setWindowIcon(QIcon(":/Default/Icons/Res/FFSOFT-removebg-preview.png"));

    QWidget* productWidget = new QWidget(this);
    productWidget->setLayout(ui->productLayout);
    ui->stockScroll->setWidget(productWidget);
}

DriverPanel::~DriverPanel()
{
    delete ui;
}



void DriverPanel::on_productSearchBar_textChanged()
{
    DbManager db;
    // Update the current search text to be used in functions that update the product list
    // E.g. adding/removing items from the list.


    QString inputText = ui->productSearchBar->text();

    if (ui->productLayout->layout() != NULL) {
        QLayoutItem* item;
        while ((item = ui->productLayout->layout()->takeAt(0)) != NULL) {
            delete item->widget();
            delete item;
        }
    }

    productWidgets = {};

    if (inputText.length() > 0) {
        std::vector<QPair<QString, QString>> foundProducts = db.searchProductStock(inputText);
        for (QPair<QString, QString> product : foundProducts) {
            productWidgets.push_back(new ProductRowWidget(product.first, product.second, this));

            if (productAdditionBuffer.size() > 0 || productRemoveBuffer.size() > 0){

                // Do logic for pushback items here. If valid pop off the vector. Then after the math for how many to count create a new widget that has updated values.

                int tempQuantityCount = product.second.toInt(); // Run temporary Count to check how much quantity needs to be updated.

                // Additional Stock
                for (const auto& buffer : productAdditionBuffer){
                    if(buffer == product.first){
                        tempQuantityCount +=1;
                    }
                }

                // Removal of Stock

                for (const auto& buffer : productRemoveBuffer){
                    if(buffer == product.first){
                        // Removes items from the combined buffer counts. If it exceeds the values on hand simply removes one of the removal values to equalize values.
                        if(tempQuantityCount >= 1){
                            tempQuantityCount -=1;
                        } else {
                            auto iterator = std::find(productRemoveBuffer.begin(), productRemoveBuffer.end(), product.first);
                            productRemoveBuffer.erase(iterator);
                            // Break after removing one to balance quantities.
                            break;
                        }
                    }
                }

                // Removes the just made widget
                productWidgets.pop_back();
                productWidgets.push_back(new ProductRowWidget(product.first, QString::number(tempQuantityCount), this));
            }
        }
    }

    for (ProductRowWidget* widget : productWidgets) {
        ui->productLayout->addWidget(widget);
        connect(widget, &ProductRowWidget::requestStockAddition, this, &DriverPanel::addStockItemToAdditionBuffer);
        connect(widget, &ProductRowWidget::requestStockRemoval, this, &DriverPanel::addStockItemToRemoveBuffer);
    }
}

void DriverPanel::addStockItemToAdditionBuffer(QString productName) {
// Add Item.
    productAdditionBuffer.push_back(productName);
//    // Refresh the page
    on_productSearchBar_textChanged();
}

void DriverPanel::addStockItemToRemoveBuffer(QString productName) {
    // remove Item.
        auto iterator = std::find(productAdditionBuffer.begin(), productAdditionBuffer.end(), productName);
        if(iterator != productAdditionBuffer.end()){
            productAdditionBuffer.erase(iterator);
        }
    // Refresh the page
    on_productSearchBar_textChanged();
}

void DriverPanel::on_confirmButton_clicked()
{
    DbManager db;

    // If stock addition buffer is not empty, add the items in it to the database.
    if (!productAdditionBuffer.empty()) {
        for (QString productName : productAdditionBuffer) {
            // Grab a unix timestamp and date it 7 days into the future.
            // (+604800 seconds)

            time_t expiryDateInTime = getCurrentUnixTimestamp();
            long int expiryDate = static_cast<long int>(expiryDateInTime);
            expiryDate += 604800;

            db.addItem(db.getProductId(productName), expiryDate);
        }
    }

    if (!productRemoveBuffer.empty()) {
        for (QString productName : productRemoveBuffer) {
            QString productId = db.getProductId(productName);

            db.removeOldestItem(productId);
        }
    }

    // Once all changes have been made to the database
    // update the defauilt quantities for each item in the search list
    // and update the search list itself.
//    emit updateStockQuantity();

    // Clear the old buffer.
    productAdditionBuffer.clear();
    productRemoveBuffer.clear();

    on_productSearchBar_textChanged();
}


void DriverPanel::on_cancelButton_clicked()
{
    productAdditionBuffer = {};
    productRemoveBuffer = {};

    on_productSearchBar_textChanged();
}

