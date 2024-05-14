#include "futurefridgepanel.h"
#include "landingpage.h"
#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Future Fridges");
    this->setWindowIcon(QIcon(":/Default/Icons/Res/FFSOFT-removebg-preview.png"));

    QWidget* stockWidget = new QWidget(this);
    stockWidget->setLayout(ui->stockLayout);
    ui->stockScroll->setWidget(stockWidget);

    QWidget* orderWidget = new QWidget(this);
    orderWidget->setLayout(ui->orderLayout);
    ui->orderScroll->setWidget(orderWidget);

    QWidget* staffWidget = new QWidget(this);
    staffWidget->setLayout(ui->staffLayout);
    ui->staffScroll->setWidget(staffWidget);

    QWidget* notifWidget = new QWidget(this);
    notifWidget->setLayout(ui->notifLayout);
    ui->notifScroll->setWidget(notifWidget);

    std::map<QString, std::vector<QString>> foundOrders = db.getAllItemsFromTable(Orders);
    std::map<QString, std::vector<QString>> foundStaff = db.getAllItemsFromTable(Staff);
    std::map<QString, std::vector<QString>> foundNotifs = db.getAllItemsFromTable(Notifications);

    for (unsigned long i = 0; i < foundOrders["id"].size(); i++) {
        orderList.push_back(new DeliveryOrderListItem(foundOrders["expectedDate"][i], foundOrders["id"][i], this));
    }
    for (unsigned long j = 0; j < foundStaff["id"].size(); j++) {
        staffList.push_back(new StaffListItem(foundStaff["name"][j], foundStaff["admin"][j], this));
    }
    for (unsigned long k = 0; k < foundNotifs["id"].size(); k++) {
        notifList.push_back(new NotificationListItem(foundNotifs["date"][k], foundNotifs["title"][k], foundNotifs["content"][k], foundNotifs["read"][k], this));
    }

    for (DeliveryOrderListItem* widget : orderList) {
        ui->orderLayout->addWidget(widget);
    }
    for (StaffListItem* widget : staffList) {
        ui->staffLayout->addWidget(widget);
    }
    for (NotificationListItem* widget : notifList) {
        ui->notifLayout->addWidget(widget);
    }
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_stockSearchBar_textChanged(const QString &inputText)
{
    if (ui->stockLayout->layout() != NULL) {
        QLayoutItem* item;
        while ((item = ui->stockLayout->layout()->takeAt(0)) != NULL) {
            delete item->widget();
            delete item;
        }
    }

    stockList = {};

    if(inputText.length() > 0) {
        std::vector<QPair<QString, QString>> foundStock = db.searchProductStock(inputText);
        for (QPair<QString, QString> stock : foundStock) {
            stockList.push_back(new StockSettingsWidget(stock.first, stock.second, this));
        }
    }

    for (StockSettingsWidget* widget : stockList) {
        ui->stockLayout->addWidget(widget);
    }
}


void SettingsWindow::on_closeButton_clicked()
{
    FutureFridgePanel *ffPanel = new FutureFridgePanel(true);

    int width = 1920;
    int height = 1080;

    int x=(width - ffPanel->width()) / 2.0;
    int y=(height - ffPanel->height()) / 2.0;

    ffPanel->setGeometry(x,y,ffPanel->width(),ffPanel->height());
    ffPanel->showNormal();
    this->hide();
}


void SettingsWindow::on_logoutButton_clicked()
{
    landingPage *LandingPage = new landingPage();

    int width = 1920;
    int height = 1080;

    int x=(width - LandingPage->width()) / 2.0;
    int y=(height - LandingPage->height()) / 2.0;

    LandingPage->setGeometry(x,y,LandingPage->width(),LandingPage->height());
    LandingPage->showNormal();
    this->hide();
}

