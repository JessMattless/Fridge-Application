#include "debug.h"

#include "dbmanager.h"

void addTableTestData() {
    DbManager db;

    db.addProduct("Apple");
    db.addProduct("Grape");
    db.addProduct("Banana");

    db.addUser("Test", "password");
    db.addUser("Admin", "password");
    db.setUserAdmin("Admin");

    db.addSupplier("Tesco");
    db.addSupplier("Asda");

    //    time_t expiryDateInTime = getCurrentUnixTimestamp();
    //    long int expiryDate = static_cast<long int>(expiryDateInTime);
    //    expiryDate += 604800;

    std::vector<std::pair<QString, QString>> testOrder;
    testOrder.push_back({"Apple", "2"});
    testOrder.push_back({"Grape", "1"});
    db.addOrder("1704067200", "1", testOrder);

    db.addDriver("password", "1", "1", "");

    db.addNotification("1704067200", "Test Notification", "Test content for notification");

    qDebug() << "Added Test data to tables";
}
