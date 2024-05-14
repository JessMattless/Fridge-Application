#ifndef STOCKSETTINGSWIDGET_H
#define STOCKSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class StockSettingsWidget;
}

class StockSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StockSettingsWidget(QString productName = "", QString productQuantity = "", QWidget *parent = nullptr);
    ~StockSettingsWidget();

private:
    QString _productName, _productQuantity;
    Ui::StockSettingsWidget *ui;
};

#endif // STOCKSETTINGSWIDGET_H
