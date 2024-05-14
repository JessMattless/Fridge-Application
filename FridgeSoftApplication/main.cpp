#include "landingpage.h"
#include "futurefridgepanel.h"

#include <QApplication>
#include <QString>
#include "debug.h"
#include "dbmanager.h"
#include <QDebug>

// Testing imports
#include "productrowwidget.h"

#include "debug.h"

int main(int argc, char *argv[])
{
//Debug stuff.
    DbManager db;

//    db.createTables();

//    addTableTestData();

//    if (db.isOpen())
//    {
//        db.createTables();
//        db.listAllProducts();
//        if(!db.isDatabaseLocked()){
//            bool success = false;
//            success = db.addProduct("Grapes");
//            if (!success){
//                qDebug() << "Failed to add product database is locked.";
//            }
//        }

//        db.listAllProducts();

//        if(!db.isDatabaseLocked())
//        {
//            bool success = false;
//            success = db.addProduct("Banana");
//            if (!success){
//                qDebug() << "Failed to add product database is locked.";
//            }
//        }

//        db.listAllProducts();
//        db.deleteProduct("Banana");

//    if (db.isOpen())
//    {
//        db.createTables();
//        db.listAllProducts();
//        if(!db.isDatabaseLocked()){
//            bool success = false;
//            success = db.addProduct("Grapes");
//            if (!success){
//                qDebug() << "Failed to add product database is locked.";
//            }
//        }

//        db.listAllProducts();

//        db.deleteProduct("Banana");

//        if(!db.isDatabaseLocked())
//        {
//            bool success = false;
//            success = db.addProduct("Banana");
//            if (!success){
//                qDebug() << "Failed to add product database is locked.";
//            }
//        }

//        db.listAllProducts();

//        QString user = "Boss";
//        QString pass = "TestPass";
//        QString fakepass = "BananaIceCreameSundae";
//        if (db.addUser(user, pass)){
//            qDebug() << "Test for user adding.";
//        }

//        if (db.decryptUserAuth(user, pass)){
//            qDebug() << "User validated commence login.";
//        }

//        if (!db.decryptUserAuth(user, fakepass)){
//            qDebug() << "User invalid Works well :>.";
//        }

//        // Test promoting user.
//        db.setUserAdmin(user);

//        const QString testProductIdSearch = "Banana";

//        qDebug() << "The id for product " << testProductIdSearch << " is: " << db.getProductId(testProductIdSearch) << "\n";

//        qDebug() << "End of Functions";
//    }
//    else
//    {
//        qDebug() << "Error: Database Failed to open.";
//    }

    //addTableTestData();


    //qDebug() << getCurrentUnixTimestamp();

    //QString prodIDTest = "1";

//    db.addItem(prodIDTest, 1707057863);

    //db.removeOldestItem(prodIDTest);


//    // Test for Adding a Driver.
//    QString passcode = "000000";
//    QString supplierId = "1";
//    QString orderId = "1";
//    QString deliveryDate = "1234567898764";

//    db.addDriver(passcode, supplierId, orderId, deliveryDate); // Prob change this to bool so can check for conflicting passcode.



//App Window starts here.
    QApplication a(argc, argv);
    FutureFridgePanel l(true);
    landingPage p;

//    MainWindow w;
//    w.show();

    // Takes the signal for successful login on the login window and executes the main window.



    // This code centers screen. https://stackoverflow.com/questions/49608313/change-window-position-on-show-in-qt - Abhishek Agarwal - April 3rd 2018
    int width = 1920;
    int height = 1080;

    int x=(width - p.width()) / 2.0;
    int y=(height - p.height()) / 2.0;

    p.setGeometry(x,y,p.width(),p.height());

//    p.show();
     l.show();
    // b.show();


    return a.exec();
}
