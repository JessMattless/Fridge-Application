#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "dbmanager.h"

#include "futurefridgepanel.h"

// Css resourceS: https://www.w3schools.com/css/css3_variables.asp

// Currently looking at https://wiki.qt.io/User_Login_Dialog

LoginWidget::LoginWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    // Obtain DB.
    DbManager db;

    //Check if password is obscured.
    ui->passcodeField->setEchoMode(QLineEdit::Password);

    // Connect confirm button click signal to the slot, repurposed from the loginWindow class.
    connect(ui->confirmButton, &QPushButton::clicked, this, &LoginWidget::onConfirmClicked);
    // Referenced: https://stackoverflow.com/questions/15561608/detecting-enter-on-a-qlineedit-or-qpushbutton - Owl - Jun 2 2021.
    connect(ui->passcodeField, &QLineEdit::returnPressed, this, &LoginWidget::onConfirmClicked);
    connect(ui->usernameField, &QLineEdit::returnPressed, this, &LoginWidget::onConfirmClicked);


    //CLose Button.
    connect(ui->closeButton, &QPushButton::clicked, this, &LoginWidget::onCloseClicked);

    //Report if the database is connected.
    if(db.isOpen()){
        qDebug("Database Connected.");
    }else{
        ui->errorLabel->setText("Error: Connection to db Failed!");
    }

}

LoginWidget::~LoginWidget()
{
    delete ui;
}


void LoginWidget::onConfirmClicked() {
    //Steal login verification from previous implementation.

    qDebug("Clicked");
    ui->errorLabel->setText("Logging in...");



    // Code to do check for username and password.

    bool loginIsSuccessful = false;

    DbManager db;

    QString user = ui->usernameField->text();
    QString pass = ui->passcodeField->text();

    if (db.decryptUserAuth(user,pass)){
        loginIsSuccessful = true;
    }

//    this->hide();

    // Code to jump to next step in ui flow.
    if(loginIsSuccessful){


        this->hide();

        FutureFridgePanel *futureFridgePanel = new FutureFridgePanel(db.isUserAdmin(user));

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

void LoginWidget::onCloseClicked() {

//    this->hide();
    close();
}

void LoginWidget::loginError() {

    qDebug("Login Failed. Change Error label to show and populate with appropriate message.");

    ui->usernameField->clear();
    ui->passcodeField->clear();
    ui->usernameField->setFocus();


    this->show();
    ui->errorLabel->setText("Login Failed. Incorrect User or Passcode");
}




void LoginWidget::on_closeButton_2_clicked()
{
    //Closes login widget.
    this->hide();
}

