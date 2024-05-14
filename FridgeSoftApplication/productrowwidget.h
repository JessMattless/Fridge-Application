#ifndef PRODUCTROWWIDGET_H
#define PRODUCTROWWIDGET_H

#include <QWidget>

namespace Ui {
class ProductRowWidget;
}

class ProductRowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProductRowWidget(QString productName, QString quantity, QWidget *parent = nullptr);
    ~ProductRowWidget();

// Recieves signals from futurefridgepanel.h to update quantity numbers.
public slots:
    void updateStartingQuantity();
    void resetDisplayQuantity();

private slots:
    void on_addItem_clicked();
    void on_removeItem_clicked();

private:
    Ui::ProductRowWidget *ui;
    QString _productName;
    QString _quantity;
    int _startingQuantity;

    // Send back values.
signals:
    //Emit signals to futurefridgepanel.h
    void requestStockAddition(QString productName);
    void requestStockRemoval(QString productName);

};

#endif // PRODUCTROWWIDGET_H
