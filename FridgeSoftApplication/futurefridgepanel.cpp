#include "futurefridgepanel.h"
#include "ui_futurefridgepanel.h"
#include "settingswindow.h"
#include "landingpage.h"

#include <utils.h>

FutureFridgePanel::FutureFridgePanel(bool isAdmin, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FutureFridgePanel)
{
    ui->setupUi(this);


    if (!isAdmin) { // if the user is not an admin (from the input parameter) then hide the settings button. logic is still there but inaccessible
        ui->settingsButton->hide();
    }

    this->setWindowTitle("Future Fridges");
    this->setWindowIcon(QIcon(":/Default/Icons/Res/FFSOFT-removebg-preview.png"));

    QWidget* productWidget = new QWidget(this);
    productWidget->setLayout(ui->productLayout);
    ui->stockScroll->setWidget(productWidget);
}

FutureFridgePanel::~FutureFridgePanel()
{
    delete ui;
}




void FutureFridgePanel::on_productSearchBar_textChanged()
{

    DbManager db;
    // Update the current search text to be used in functions that update the product list
    // E.g. adding/removing items from the list.
    currentSearchText = ui->productSearchBar->text();

    QString inputText = currentSearchText;

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
        connect(widget, &ProductRowWidget::requestStockAddition, this, &FutureFridgePanel::addStockItemToAdditionBuffer);
        connect(widget, &ProductRowWidget::requestStockRemoval, this, &FutureFridgePanel::addStockItemToRemoveBuffer);
        connect(this, &FutureFridgePanel::updateStockQuantity, widget, &ProductRowWidget::updateStartingQuantity);
        connect(this, &FutureFridgePanel::resetStockQuantity, widget, &ProductRowWidget::resetDisplayQuantity);
    }
}

// Settings button slot
void FutureFridgePanel::on_settingsButton_clicked()
{
    // create new settingsWindow class and opening. This logic should only be accessible to admins.
    SettingsWindow *settingsPage = new SettingsWindow;

    int width = 1920;
    int height = 1080;

    int x=(width - settingsPage->width()) / 2.0;
    int y=(height - settingsPage->height()) / 2.0;

    settingsPage->setGeometry(x,y,settingsPage->width(),settingsPage->height());

    settingsPage->showNormal();
    this->hide();
}


void FutureFridgePanel::addStockItemToAdditionBuffer(QString productName) {
// Add Item.
    productAdditionBuffer.push_back(productName);
//    // Refresh the page
    on_productSearchBar_textChanged();
}

void FutureFridgePanel::addStockItemToRemoveBuffer(QString productName) {
    // remove Item.
    productRemoveBuffer.push_back(productName);
    // Refresh the page
    on_productSearchBar_textChanged();
}

void FutureFridgePanel::on_confirmButton_clicked()
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


void FutureFridgePanel::on_cancelButton_clicked()
{
    productAdditionBuffer = {};
    productRemoveBuffer = {};

    emit resetStockQuantity();
    on_productSearchBar_textChanged();
}


void FutureFridgePanel::on_logoutButton_clicked()
{
    landingPage *LandingPage = new landingPage;


    int width = 1920;
    int height = 1080;

    int x=(width - LandingPage->width()) / 2.0;
    int y=(height - LandingPage->height()) / 2.0;

    LandingPage->setGeometry(x,y,LandingPage->width(),LandingPage->height());

    LandingPage->showNormal();
    this->hide();
}

