#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "environment.h"

#include <QString>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

// For ProductSearchReturn.
#include <vector>
#include <QPair>

enum Table {Products, Index, Staff, Drivers, Suppliers, Orders, OrderItems, Notifications};

// For item add and remove.
#include "utils.h"

// Reference Implementation of sqlitemanager
// https://katecpp.wordpress.com/2015/08/28/sqlite-with-qt/

// Reference for various sqlite commands. https://www.sqlitetutorial.net/

/**
 * @brief A Database Classpair
 * @author Frigde Soft LLC
 *
 * This class is about how database will operate and the functions within the database, and it shows the database opening and closing of database connections
 * and it checks for the status of database file.
 */


class DbManager
{
public:

    /**
     * @brief The public member function will have a constructor DbManager that will be the object with the specified database file path.
     *
     * @param path is the path to the database file.
     */

    DbManager(const QString& path = DATABASE_PATH);

    /**
     * @brief This functions will destroy the DbManager object and close any open connections.
     *
     * The destructor will ensure that database connections are properly closed when the DbManager object goes out of scope.
     */

    ~DbManager();

    /**
     * @brief This will check if the database file is open.
     *
     * @return It will return true if the database file is open and if not, it will be false which means file is closed.
     */

    bool isOpen() const;

    /**
     * @brief It will see the status of database if it's currently locked.
     *
     * This function checks whether the SQLite database is locked.
     * Referenced from: https://stackoverflow.com/questions/57744538/determine-whether-sqlite-database-is-locked (p-a-o-l-o Sep 4 2019)
     *
     * @return It will return true if the database is locked and if not, it will be false which means the database is not locked.
     */

    bool isDatabaseLocked() const;

    /**
     * @brief Creates tables used by the Database.
     *
     * @return The function returns true if any tables are created, and false otherwise.
     */

    bool createTables();

    /**
     * @brief Get all items from a database table, with each column having a list of items.
     * @param table is the name of the table to get items from.
     * @return a map of all of the items, each entry has the name of the column then vector of each item in that column.
     */

    std::map<QString, std::vector<QString>> getAllItemsFromTable(Table table) const;

    /**
     * @brief Lists all items in the product table.
     */

    void listAllProducts() const;

    /**
     * @brief Checks if a product with the given name exists.
     *
     * @param productName is the name of the product to check.
     *
     * @return It will return true if the product exists and if not, it will be false which means the product does not exist.
     */

    bool productExists(const QString& productName) const;

    /**
     * @brief Adds a product to the products table.
     *
     * @param productName the name of the product to add.
     *
     * @return It will return true if the product is successfully added and if not, it will be false which means the proudct has not yet been added.
     */

    bool addProduct(const QString& productName);

    /**
     * @brief Searches for a product and returns its ID.
     *
     * @param productName the name of the product to search for.
     *
     * @return It will return the ID of the product if it is found and if not an empty string will be returned.
     */

    QString getProductId(const QString& productName) const;

    /**
     * @brief Finds and deletes a product from the table.
     *
     * @param productName the name of the product to delete.
     *
     * @return It will return true if the product is successfully deleted and if not, it will be false which means the product has not yet been deleted.
     */

    bool deleteProduct(const QString& productName);
    bool deleteProduct(int id);

    // Function to handle all password functions and return true if the user is valud
    bool decryptUserAuth(const QString username, const QString password);

    // Function to add the user to the database.
    bool addUser(const QString username, const QString password);

    // Function to search by productname and return a vector of the products.
    std::vector<QPair<QString, QString>> searchProductStock(QString);


    // Adding Items to database. (Uses time_t)
    void addItem(QString, long int unixTime);

    // Remove Item from database according to timestamp.
    void removeItem(QString productId, long int unixTime);

    // Remove nearest Expiry Item from database.
    void removeOldestItem(QString productId);

    // check if user is an admin
    bool isUserAdmin(QString userName);

    // Set a user as admin
    void setUserAdmin(QString userName);

    // Demote a user for admin
    void demoteUserAdmin(QString userName);

    // Add a driver to the driver table.
    void addDriver(QString passcode, QString supplierId, QString orderId, QString deliveryDate);

    // Check if passcode is valid.
    bool isDriverValid(QString passcode);

    // Adds a supplier to the Supplier table.
    void addSupplier(QString supplierName);

    void addOrder(QString expectedDate, QString supplierId, std::vector<std::pair<QString, QString>> productList);

    QString getOrderId(QString expectedDate, QString supplierId);
    std::pair<QString, QString> getOrderDetails(QString orderId);
    std::vector<std::pair<QString, QString>> getOrderProducts(QString orderId);

    void addNotification(QString date, QString title, QString content);

private:
    QSqlDatabase m_db;

    // A map of strings used for creating tables in the database
    std::map<Table, QString> _tableStrings = {
        {Products, "CREATE TABLE Products(id INTEGER NOT NULL, productName TEXT NOT NULL, PRIMARY KEY(id));"},
        {Index, "CREATE TABLE [Index](id INTEGER NOT NULL, productId INTEGER NOT NULL, expiryDate INTEGER NOT NULL, FOREIGN KEY(productId) REFERENCES Products(id) PRIMARY KEY(id AUTOINCREMENT));"},
        {Staff, "CREATE TABLE Staff (id	INTEGER NOT NULL UNIQUE, name TEXT NOT NULL, passcode TEXT NOT NULL, admin INTEGER NOT NULL DEFAULT 0, PRIMARY KEY(id));"},
        {Drivers, "CREATE TABLE Drivers (id	INTEGER NOT NULL UNIQUE, supplierId	INTEGER NOT NULL, passcode TEXT NOT NULL, deliveryDate TEXT, orderId INTEGER NOT NULL, FOREIGN KEY(supplierId) REFERENCES Suppliers(id), FOREIGN KEY(orderId) REFERENCES Orders(id), PRIMARY KEY(id));"},
        {Suppliers, "CREATE TABLE Suppliers(id INTEGER NOT NULL, name TEXT NOT NULL, PRIMARY KEY(id));"},
        {Orders, "CREATE TABLE Orders (id INTEGER NOT NULL UNIQUE, expectedDate	TEXT NOT NULL, supplierId INTEGER NOT NULL, FOREIGN KEY(supplierId) REFERENCES Suppliers(id), PRIMARY KEY(id));"},
        {OrderItems, "CREATE TABLE OrderItems (id INTEGER NOT NULL UNIQUE, orderId INTEGER NOT NULL, productName TEXT NOT NULL, quantity INTEGER NOT NULL, FOREIGN KEY(orderId) REFERENCES Orders(id), PRIMARY KEY(id));"},
        {Notifications, "CREATE TABLE Notifications (id INTEGER NOT NULL UNIQUE, date TEXT NOT NULL, title TEXT NOT NULL, content TEXT, read INTEGER NOT NULL DEFAULT 0, PRIMARY KEY(id));"}
    };

    bool compareUserAuth(QString username, QString password);



}; // class DbManager

#endif // DBMANAGER_H
