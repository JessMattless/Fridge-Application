#ifndef FUTUREFRIDGEPANEL_H
#define FUTUREFRIDGEPANEL_H

#include <QMainWindow>

#include "productrowwidget.h"
#include "dbmanager.h"

#include <vector>
#include <QPair>

namespace Ui {
class FutureFridgePanel;
}

class FutureFridgePanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit FutureFridgePanel(bool isAdmin = false, QWidget *parent = nullptr);
    ~FutureFridgePanel();

    //void populateProductRows();
    //void clearProductList();

public slots:
    // functions called by the signals emitted within productrowwidget.h
    void addStockItemToAdditionBuffer(QString productName);
    void addStockItemToRemoveBuffer(QString productName);

private slots:
    void on_productSearchBar_textChanged();
    void on_settingsButton_clicked();
    void on_confirmButton_clicked();

    void on_cancelButton_clicked();

    void on_logoutButton_clicked();

private:
    Ui::FutureFridgePanel *ui;

    DbManager db;
    std::vector<ProductRowWidget*> productWidgets; // Array to store all widgets, my swap for individual instances later.
    std::vector<QString> productAdditionBuffer;
//    std::vector<std::pair<QString, int>> itemAdditionBuffer;
    std::vector<QString> productRemoveBuffer;
    QString currentSearchText;

signals:
    // Signals to productrowwidget.h to update quantity numbers for stock.
    void resetStockQuantity();
    void updateStockQuantity();
};

#endif // FUTUREFRIDGEPANEL_H
