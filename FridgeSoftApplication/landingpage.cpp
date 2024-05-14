#include "landingpage.h"
#include "ui_landingpage.h"


#include "futurefridgepanel.h"
#include "driverpanel.h"
#include "driverloginwidget.h"

// Signals and slots reference: https://doc.qt.io/qt-5/signalsandslots.html
// Some code examples observed for coding style. found here-> https://cpp.hotexamples.com/examples/-/QDialog/-/cpp-qdialog-class-examples.html

landingPage::landingPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::landingPage)
{

    ui->setupUi(this);

    this->setWindowTitle("Future Fridges");
    this->setWindowIcon(QIcon(":/Default/Icons/Res/FFSOFT-removebg-preview.png"));
}

landingPage::~landingPage()
{
    delete ui;
}


void landingPage::on_user_Login_clicked()
{


    //    init instance of login prompt.
    LoginWidget *loginWidget = new LoginWidget(this);

    connect(loginWidget, &LoginWidget::loginSuccessful, this, &landingPage::onLoginSuccessful);

//    this->hide();
    // Hide the titlebar of the widget.
    loginWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog); // Refernece: https://stackoverflow.com/questions/26015880/how-to-hide-titlebar-of-the-qdialog-window User AngryDuck 24th September 2014.
    loginWidget->exec();
}


void landingPage::on_delivery_Login_clicked()
{
    //    init instance of login prompt.
    DriverLoginWidget *driverLoginWidget = new DriverLoginWidget(this);

    connect(driverLoginWidget, &DriverLoginWidget::loginSuccessful, this, &landingPage::onLoginSuccessful);

//    this->hide();
    // Hide the titlebar of the widget.
    driverLoginWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog); // Refernece: https://stackoverflow.com/questions/26015880/how-to-hide-titlebar-of-the-qdialog-window User AngryDuck 24th September 2014.
    driverLoginWidget->show();

}


void landingPage::onLoginSuccessful(){

    qDebug("Login Successful Put logic here.");


//    MainWindow *mainWindow = new MainWindow;
//    FutureFridgePanel *futureFridgePanel = new FutureFridgePanel(true);
//    futureFridgePanel->show();

    this->hide();
}

