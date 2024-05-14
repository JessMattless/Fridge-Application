#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "dbmanager.h"
#include "deliveryorderlistitem.h"
#include "notificationlistitem.h"
#include "stafflistitem.h"
#include "stocksettingswidget.h"

#include <QMainWindow>
#include <vector>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private slots:
    void on_stockSearchBar_textChanged(const QString &inputText);

    void on_closeButton_clicked();

    void on_logoutButton_clicked();

private:
    Ui::SettingsWindow *ui;

    DbManager db;
    std::vector<StockSettingsWidget*> stockList;
    std::vector<DeliveryOrderListItem*> orderList;
    std::vector<StaffListItem*> staffList;
    std::vector<NotificationListItem*> notifList;
};

#endif // SETTINGSWINDOW_H
