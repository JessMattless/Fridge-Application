#include "driverloginwidget.h"
#include "ui_driverloginwidget.h"

#include "dbmanager.h"
#include "driverpanel.h"

DriverLoginWidget::DriverLoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriverLoginWidget)
{
    ui->setupUi(this);
    // Obtain DB.
    DbManager db;

    //Check if password is obscured.
    ui->passcodeField->setEchoMode(QLineEdit::Password);

    // Connect confirm button click signal to the slot, repurposed from the loginWindow class.
    connect(ui->confirmButton, &QPushButton::clicked, this, &DriverLoginWidget::on_confirmButton_clicked);
    connect(ui->passcodeField, &QLineEdit::returnPressed, this, &DriverLoginWidget::on_confirmButton_clicked);

}

DriverLoginWidget::~DriverLoginWidget()
{
    delete ui;
}


//isDriverValid

void DriverLoginWidget::on_confirmButton_clicked()
{
    qDebug("Clicked");
    ui->errorLabel->setText("Logging in...");

    DbManager db;

     bool loginIsSuccessful = false;

     QString pass = ui->passcodeField->text();

     if (db.isDriverValid(pass)){
         loginIsSuccessful = true;
     }

     // Code to jump to next step in ui flow.
     if(loginIsSuccessful){


         this->hide();

         DriverPanel *futureFridgePanel = new DriverPanel();

         int width = 1920;
         int height = 1080;

         int x=(width - futureFridgePanel->width()) / 2.0;
         int y=(height - futureFridgePanel->height()) / 2.0;

         futureFridgePanel->setGeometry(x,y,futureFridgePanel->width(),futureFridgePanel->height());

         futureFridgePanel->show();

         emit loginSuccessful(); // On success close landingpage;


         qDebug("Login is successful.");
         // /// // / / /// CODE TO JUMP TO ITEM PANEL GOES HERE!!!! points to main window for now.
         close();
     }else {
         loginError();
     }

}

void DriverLoginWidget::loginError() {

    qDebug("Login Failed. Change Error label to show and populate with appropriate message.");

    ui->passcodeField->clear();
    ui->passcodeField->setFocus();


    this->show();
    ui->errorLabel->setText("Login Failed. Incorrect Passcode");
}

void DriverLoginWidget::on_closeButton_clicked()
{
    //Closes login widget.
    this->hide();
}


void DriverLoginWidget::on_passcodeField_returnPressed()
{

}

