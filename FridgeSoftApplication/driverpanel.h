#ifndef DRIVERPANEL_H
#define DRIVERPANEL_H

#include <QMainWindow>

#include "productrowwidget.h"
#include "dbmanager.h"


#include <vector>
#include <QPair>

namespace Ui {
class DriverPanel;
}

class DriverPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit DriverPanel(QWidget *parent = nullptr);
    ~DriverPanel();

public slots:
    // functions called by the signals emitted within productrowwidget.h
    void addStockItemToAdditionBuffer(QString productName);
    void addStockItemToRemoveBuffer(QString productName);


private slots:
    void on_productSearchBar_textChanged();

    void on_confirmButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::DriverPanel *ui;

    std::vector<QString> productAdditionBuffer;
//    std::vector<std::pair<QString, int>> itemAdditionBuffer;
    std::vector<QString> productRemoveBuffer;
    std::vector<ProductRowWidget*> productWidgets; // Array to store all widgets, my swap for individual instances later.
};

#endif // DRIVERPANEL_H
