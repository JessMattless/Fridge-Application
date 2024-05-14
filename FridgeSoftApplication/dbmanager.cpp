#include "dbmanager.h"
#include "environment.h"
#include "utils.h"

#include <cstdlib>

// ----Non class specific functions---- //

void logDatabaseError() {

}

// ---- ---- //

DbManager::DbManager(const QString& path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    // Prevents Waiting for locks to release.
    // referenced -> :https://stackoverflow.com/questions/57744538/determine-whether-sqlite-database-is-locked (p-a-o-l-o Sep 4 2019)
    m_db.setConnectOptions("QSQLITE_BUSY_TIMEOUT=0");

    if (m_db.open()){
        qDebug() << "Successfully connected to Database.";
    }else{
        qDebug() << "Database connection failed. view "; logDatabaseError();
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen()) m_db.close();
}

// ---- ---- //

bool DbManager::isOpen() const
{
    return m_db.isOpen();

}

/**
 * @brief Checks if the database is currently locked.
 *
 * If the lock is obtained successfully, it's promptly released, and the function returns false, signaling that the database isn't locked.
 * If the database is, indeed, locked, the function returns true, and an error message is logged to the console.
 *
 * @return It will return true if the database is locked and if the database is not locked it will provide a false return.
 */

bool DbManager::isDatabaseLocked() const
{

    // Trying to aquire lock to test for database lock state.
    bool locked = true;
    QSqlQuery checklockQuery(m_db);

    // Try to acquire lock.
    if(checklockQuery.exec("BEGIN EXCLUSIVE"))
    {
        // Release lock immediately.
        checklockQuery.exec("COMMIT");
        locked = false;
    }

    // If the database is locked, the function will log a message.
    if (locked) {
        qDebug() << "Database is locked. Stop other operations and try again later.";
    }

    return locked;
}

/**
  * @brief Creates the 'Products', 'Index', 'Staff', 'Drivers', 'Suppliers', 'Orders', 'OrderItems' and 'Notifications' tables in the database if they do not exist.
  *
  * @return It will return true if the table is successfully created or already exists and if not it will be false, meaning no tables were created.
  */

bool DbManager::createTables()
{
    bool wasTableCreated = false;

    QSqlQuery query;
    for (auto iterator = _tableStrings.begin(); iterator != _tableStrings.end(); iterator++) {
        query.prepare(iterator->second);
        if (query.exec()) {
            wasTableCreated = true;
        }
    }

    return wasTableCreated;
}

/**
 * @brief Lists all items from a given table.
 * @param table is the name of the table to get data from.
 * @return a map of all of the items, each entry has the name of the column then vector of each item in that column.
 */

std::map<QString, std::vector<QString>> DbManager::getAllItemsFromTable(Table table) const {
    std::map<QString, std::vector<QString>> returnTable;

    // Get name of table the enum entry given in the function argument
    QString tableName;
    switch (table) {
    case Products: tableName = "Products"; break;
    case Index: tableName = "Index"; break;
    case Staff: tableName = "Staff"; break;
    case Drivers: tableName = "Drivers"; break;
    case Suppliers: tableName = "Suppliers"; break;
    case Orders: tableName = "Orders"; break;
    case OrderItems: tableName = "OrderItems"; break;
    case Notifications: tableName = "Notifications"; break;
    }

    QSqlQuery querySearch;
    querySearch.prepare("SELECT * FROM " + tableName);
//    querySearch.bindValue(":tableName", tableName);

    // Fill the map with column entries
    if (querySearch.exec()) {
        for (int i = 0; i < querySearch.record().count(); i++) {
            returnTable[querySearch.record().fieldName(i)] = {};
        }

        while (querySearch.next()) {
            // For each column in the table, add the corresponding cell of data.
            for (auto itr = returnTable.begin(); itr != returnTable.end(); itr++) {
                int fieldNo = querySearch.record().indexOf(itr->first);
                itr->second.push_back(querySearch.value(fieldNo).toString());
            }
        }

    }
    return returnTable;

}

/**
 * @brief Lists all products in the 'Products' table of the database.
 *
 * The function retrieves all records from the 'Products' table and logs
 * the ID and Name of each product to the console.
 */

void DbManager::listAllProducts() const
{

    qDebug() << "Products in db: \n";
    qDebug() << "ID:        Name: ";
    QSqlQuery query("SELECT * FROM Products");
    int prodId = query.record().indexOf("id");
    int prodName = query.record().indexOf("productName");
    while (query.next())
    {
        QString currentProdName = query.value(prodName).toString();
        QString currentProdId = query.value(prodId).toString();
        qDebug() << currentProdId << " === " << currentProdName;
    }
    qDebug() << "\n";

}

/**
 * @brief It will check if a product with the given name exists in the 'Products' table.
 *
 * The function prepares and executes an SQL query to select the 'productName' from the 'Products' table where the name matches.
 * If a matching record is found, the function returns true; otherwise, it will return false.
 *
 * @return It will return true if a product with the given name exists and if not, it will be false.
 */


bool DbManager::productExists(const QString& productName) const{

    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT productName FROM Products WHERE productName = (:productName)");
    checkQuery.bindValue(":productName", productName);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }
    else{
        qDebug() << "Product exists failed: " << checkQuery.lastError();
    }

    return exists;
}

/**
 * @brief Add a product with the given name to the 'Products' table.
 *
 * The function prepares and executes an SQL query to insert a new record into
 * the 'Products' table with the supplied product name. It will check for the validation
 * of the product name and then it will log messages if the the data is not inserted.
 *
 * @param productName the name of the product to be added.
 *
 * @return It will return true if the product is successfully added and if not, it will be false.
 */

bool DbManager::addProduct(const QString& productName)
{
    bool success = false;

    if(!productName.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO Products (productName) VALUES (:productName)");
        queryAdd.bindValue(":productName", productName);

        // Check if the product already exists
        if (productExists(productName))
        {
            qDebug() << "add Product failed: product already exists!";
            success = false;
            return success;
        }

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "add Product to table failed: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "add Product failed: product name cannot be empty!";
    }

    return success;
}

/**
 * @brief It will get the ID of a product with the specified name from the 'Products' table.
 *
 * The function prepares and executes an SQL query to select the 'id' from the 'Products' table where the product name matches the given parameter.
 * If a matching record is found, the function returns the ID as a QString; otherwise, it will return as an empty string.
 *
 * @return The ID of the product if found, an empty string otherwise.
 */

QString DbManager::getProductId(const QString& productName) const{

    QString id;



    // Prepare SQL query to select the ID of the specified product
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM Products WHERE productName = (:productName)");
    checkQuery.bindValue(":productName", productName);

    // Execute the query and retrieve the product ID.
    if (checkQuery.exec())
    {
        if(checkQuery.next())
        {
            id = checkQuery.value(0).toString(); // 0 is the index of id in the products table.
        }
    }
    else
    {
        qDebug() << "Failed to find Productid: " << checkQuery.lastError();
    }

    return id;
}

/**
 * @brief Deletes a product from the database.
 *
 * This method removes a product from the 'Products' table in the database
 * based on the provided product name. It will first check if the product exists
 * in the database before trying to delete it.
 *
 * @param productName the name of the product to be deleted.
 *
 * @return It will return if the product is successfully deleted and if not, it will be false which means the product has not been deleted yet.
 *
 */

bool DbManager::deleteProduct(const QString& productName){
    bool success = false;

    // Check if the product exists before attempting deletion.
    if (productExists(productName)){

        // Prepare SQL query for product deletion
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM Products WHERE productName = (:productName)");
        queryDelete.bindValue(":productName", productName);
        success = queryDelete.exec();

        // Log an error message if deletion fails
        if(!success){
            qDebug() << "Remove product Failed!: " << queryDelete.lastError();
        }
    }
    else{
        qDebug() << "Remove product failed. Product doesnt exist...";
    }

    // Return the success status if the product has been deleted.
    return success;
}


bool DbManager::compareUserAuth(QString username, QString password){

    return false;
}

bool DbManager::decryptUserAuth(QString username, QString password){

    // Retrieve Hashed Password from database for comparison.

    QString hashedPassword;

    // Prepare SQL query to select the passcode of the specified user
    QSqlQuery query;
    query.prepare("SELECT passcode FROM Staff WHERE name = (:username)");
    query.bindValue(":username", username);

    // Execute the query and retrieve the passcode.
    if (query.exec())
    {
        if(query.next())
        {
            hashedPassword = query.value(0).toString(); // 0 is the index of id in the products table.
        }
    }
    else
    {
        qDebug() << "Failed to find user: " << query.lastError();

        return false;
    }


    // Entered Password
    std::string enteredPassword = password.toStdString(); //https://stackoverflow.com/questions/4214369/how-to-convert-qstring-to-stdstring :chris Nov 18 2010.
    std::string storedHash = hashedPassword.toStdString();

    if (isPasswordCorrect(enteredPassword, storedHash)){
        return true;
    } else {
        return false;
    }

    return false;
}


bool DbManager::addUser(QString username, QString password){

    // Check if user exists before entering a new username.

    // Prepare SQL query to select the username of the specified user
    QSqlQuery checkUserExistsQuery;
    checkUserExistsQuery.prepare("SELECT name FROM Staff WHERE name = (:username)");
    checkUserExistsQuery.bindValue(":username", username);

    // Execute the query and retrieve the passcode.
    if (checkUserExistsQuery.exec())
    {
        if (checkUserExistsQuery.next()){
            qDebug() << "User Already Exists. Please create Another user";
            return false;
        } else {


            std::string hashedPassword = generateHashedPasswordWithSalt(password.toStdString());


            // Code to Insert user here

            QSqlQuery queryAdd;
            queryAdd.prepare("INSERT INTO Staff (name, passcode) VALUES (:username, :passcode)");
            queryAdd.bindValue(":username", username);
            queryAdd.bindValue(":passcode", QString::fromStdString(hashedPassword));

            if(queryAdd.exec()){
                qDebug() << "User Successfully Added";
                return true;
            } else {
                qDebug() << "Failed to add User to table...";
                return false;
            }
        }
    }

    return false;
}


std::vector<QPair<QString, QString>> DbManager::searchProductStock(QString currentSearch){

    std::vector<QString> tempProductList;
    std::vector<QString> tempQuantityList;
    // Return the QPair of Product Name and Quantity items. return empty if nothing.
    std::vector<QPair<QString, QString>> items;

    // Run code to search: SELECT productName FROM Products WHERE productName LIKE '(:ReplaceString)%'

    if(!isOpen()) {
        qDebug("Database is not Open.");
        return items;
    }

    // pull results with query.
    QSqlQuery query(m_db);
    QString queryString = "SELECT productName FROM Products WHERE productName LIKE :searchTerm";
    query.prepare(queryString);
    query.bindValue(":searchTerm", "%" + currentSearch + "%");

    if (!query.exec()) {
        qDebug() << "Error executing search query: " << query.lastError().text();
        return items;
    }

    // Dump result in vector. and if more than 0 loop over each product and fetch relevant stock quantity. Dump result in vector.

    while(query.next()) {
        QString productName = query.value(0).toString();

        // Fetch relevant stock quantity from Index table.
        QSqlQuery indexQueryObj(m_db);
        QString indexQuery = "SELECT COUNT(*) FROM [Index] WHERE productId = :productId";

        indexQueryObj.prepare(indexQuery);
        indexQueryObj.bindValue(":productId", getProductId(productName)); // Who knew this would come in handy again lol.


//        qDebug() << "Error Executing index query: " << indexQueryObj.lastError().text();

        if (indexQueryObj.exec()){
            if (indexQueryObj.next()) {

                // Make result Pair and return.
                QString quantity = indexQueryObj.value(0).toString();

                items.push_back(qMakePair(productName, quantity));
            }
        }
    }

//    qDebug("Response From Db");

    return items;
}


void DbManager::addItem(QString productId, long int unixTime){


    // When adding items in bulk make sure to have the unixTime incremented (basically be unique) otherwise they all will be deleted. (Might have internal logic to fix this later.

    QSqlQuery query(m_db);

//    "INSERT INTO User_Table (name, passcode) VALUES (:username, :passcode)"

    query.prepare("INSERT INTO [Index] (productId, expiryDate) VALUES (:productId, :expiryDate)");
    query.bindValue(":productId", productId);
    query.bindValue(":expiryDate", QVariant::fromValue<long>(unixTime));

    if (!query.exec()){
        qDebug() << query.lastError().text();
    }

}

void DbManager::removeItem(QString productId, long int unixTime){

    QSqlQuery query(m_db);

    // Just.... delete the item.
    query.prepare("DELETE FROM [Index] WHERE productId = (:productId) AND expiryDate = (:expiryDate)");
    query.bindValue(":productId", productId);
    query.bindValue(":expiryDate", QVariant::fromValue<long>(unixTime));
    query.exec();

}

void DbManager::removeOldestItem(QString productId){

    QSqlQuery query(m_db);

    // Select the oldest item (by id?)
    query.prepare("SELECT id FROM [Index] WHERE productId = (:productId) ORDER BY expiryDate ASC LIMIT 1");
    query.bindValue(":productId", productId);

    if (query.exec() && query.next()) {
        // Get the min expiry
        int id = query.value(0).toInt();

        // Remove the oldest item
        query.prepare("DELETE FROM [Index] WHERE id = (:id)");
        query.bindValue(":id", id);
//        query.bindValue(":expiryDateId", minExpiryDateId);
        query.exec();
    } else {
        qDebug() << query.lastError().text();
    }

//    query.prepare("DELETE FROM [Index] WHERE productId = (:productId) AND expiryDate = (SELECT MIN(expiryDate) FROM [Index] WHERE productId = (:productId))");

//    query.bindValue(":productId", productId);

//       if (!query.exec()) {
//           qDebug() << query.lastError().text();
//       }

}


bool DbManager::isUserAdmin(QString userName){

    bool isAdmin = false;
    // Prepare SQL query to select the adminstatus of the specified user
    QSqlQuery query;
    query.prepare("SELECT admin FROM Staff WHERE name = (:username)");
    query.bindValue(":username", userName);

    // Execute the query and retrieve the passcode.
    if (query.exec())
    {
        if (query.next()){
            QString adminStatus = query.value(0).toString();
            if (adminStatus == "1"){
                isAdmin = true;
            }
        }
    }
    return isAdmin;
}

void DbManager::setUserAdmin(QString userName){

    // Prepare SQL query to update the username of the specified user
    QSqlQuery query;
    query.prepare("UPDATE Staff SET admin = 1 WHERE name = (:username)");
    query.bindValue(":username", userName);

    // Execute the query and retrieve the passcode.
    if (!query.exec())
    {
        qDebug("User doesnt Exist");
    }
    return;
}


void DbManager::demoteUserAdmin(QString userName){

    // Prepare SQL query to update the username of the specified user
    QSqlQuery query;
    query.prepare("UPDATE Staff SET admin = 0 WHERE name = (:username)");
    query.bindValue(":username", userName);

    // Execute the query and retrieve the passcode.
    if (!query.exec())
    {
        qDebug("User doesnt Exist");
    }
    return;
}



void DbManager::addDriver(QString passcode, QString supplierId, QString orderId, QString deliveryDate){

    // Adds driver using the provided items.
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO drivers (supplierId, passcode,deliveryDate, orderId) VALUES (:supplierid, :passcode, :deliverydate, :orderid)");
    queryAdd.bindValue(":passcode", passcode);
    queryAdd.bindValue(":supplierid", supplierId);
    queryAdd.bindValue(":orderid", orderId);
    queryAdd.bindValue(":deliverydate", deliveryDate);

    if(queryAdd.exec()){
        qDebug() << "Driver Successfully Added";
        return;
    } else {
        qDebug() << "Failed to add Driver to table...";
        return;
    }
}


bool DbManager::isDriverValid(QString passcode){


    // Checks if the driver is valid and expected.
    QSqlQuery queryValid;
    queryValid.prepare("SELECT * FROM drivers WHERE passcode = :passcode LIMIT 1");
    queryValid.bindValue(":passcode", passcode);

    if(queryValid.exec() && queryValid.next()){
        qDebug() << "Driver Valid. Login confirmed :>";
        return true;
    } else {
        qDebug() << "No match to passcode";
        return false;
    }
    return false;
}

void DbManager::addSupplier(QString supplierName)
{
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO Suppliers (name) VALUES (:supplierName)");
    queryAdd.bindValue(":supplierName", supplierName);

    if(queryAdd.exec()){
        qDebug() << "Supplier Successfully Added";
        return;
    } else {
        qDebug() << "Failed to add Supplier to table...";
        return;
    }
}

void DbManager::addOrder(QString expectedDate, QString supplierId, std::vector<std::pair<QString, QString>> productList)
{
    QSqlQuery queryAdd;

    srand(getCurrentUnixTimestamp());
    QString orderId = QString::number(rand());

    queryAdd.prepare("INSERT INTO Orders (id, expectedDate, supplierId) VALUES (:id, :expectedDate, :supplierId)");
    queryAdd.bindValue(":id", orderId);
    queryAdd.bindValue(":expectedDate", expectedDate);
    queryAdd.bindValue(":supplierId", supplierId);

    if(queryAdd.exec()){
        qDebug() << "Order Successfully Added";
    } else {
        qDebug() << "Failed to add Order to table...";
    }

    for (std::pair<QString, QString> product : productList) {
        queryAdd.prepare("INSERT INTO OrderItems (orderId, productName, quantity) VALUES (:orderId, :productName, :quantity)");
        queryAdd.bindValue(":orderId", orderId);
        queryAdd.bindValue(":productName", product.first);
        queryAdd.bindValue(":quantity", product.second);

        if(queryAdd.exec()){
            qDebug() << "Item Successfully Added To Order";
        } else {
            qDebug() << "Failed to add Item to order...";
        }
    }
}

QString DbManager::getOrderId(QString expectedDate, QString supplierId)
{
    QString orderId = "";

    QSqlQuery querySearch;
    querySearch.prepare("SELECT * FROM Orders WHERE expectedDate = (:expectedDate) AND supplierId = (:supplierId);");
    querySearch.bindValue(":expectedDate", expectedDate);
    querySearch.bindValue(":supplierId", supplierId);

    if (querySearch.first()) {
        orderId = querySearch.value(querySearch.record().indexOf("id")).toString();
    }

    return orderId;
}

std::pair<QString, QString> DbManager::getOrderDetails(QString orderId)
{
    QSqlQuery querySearch;
    querySearch.prepare("SELECT * FROM Orders WHERE id = (:orderId);");
    querySearch.bindValue(":id", orderId);

    if(querySearch.first()) {
        std::pair<QString, QString> returnOrder;
        returnOrder.first = querySearch.value(querySearch.record().indexOf("expectedDate")).toString();
        returnOrder.second = querySearch.value(querySearch.record().indexOf("supplierId")).toString();
        return returnOrder;
    }

    return {"", ""};
}

std::vector<std::pair<QString, QString>> DbManager::getOrderProducts(QString orderId)
{
    std::vector<std::pair<QString, QString>> productList;

    QSqlQuery querySearch;
    querySearch.prepare("SELECT * FROM OrderItems WHERE orderId = (:orderId);");
    querySearch.bindValue(":orderId", orderId);

    while (querySearch.next()) {
        QString productName = querySearch.value(querySearch.record().indexOf("productName")).toString();
        QString quantity = querySearch.value(querySearch.record().indexOf("quantity")).toString();
        productList.push_back({productName, quantity});
    }

    return productList;
}

void DbManager::addNotification(QString date, QString title, QString content)
{
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO Notifications (date, title, content, read) VALUES (:date, :title, :content, :read)");
    queryAdd.bindValue(":date", date);
    queryAdd.bindValue(":title", title);
    queryAdd.bindValue(":content", content);
    queryAdd.bindValue(":read", "0");

    if(queryAdd.exec()){
        qDebug() << "Notification Successfully Added";
    } else {
        qDebug() << "Failed to add Notification to table...";
    }
}

