//#include <boost/test/unit_test_suite.hpp>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE FridgeSoft_Tests
#include <boost/test/included/unit_test.hpp>
#include "dbmanager.h"
#include "mainwindow.h"
#include <iostream>
#include <cstdio>
#include <string>

// delete any existing database file. force to create new file.
struct Fixture {
    Fixture() {
        char dbfile[] = "./FFridge-test.db";
        remove(dbfile);
        std::cout << "removed previous db test database" << std::endl;
    }
    ~Fixture() {}
};

BOOST_GLOBAL_FIXTURE( Fixture );

BOOST_AUTO_TEST_SUITE( Database_Tests )


BOOST_AUTO_TEST_CASE( DbNotLocked )
{
    DbManager db = DbManager("./FFridge-test.db");
    bool locked = db.isDatabaseLocked();
    BOOST_CHECK_EQUAL(locked, false);
}


BOOST_AUTO_TEST_CASE( DbLocked )
{
   // connect to the database without dbManager class to claim lock first
   QSqlDatabase database_raw = QSqlDatabase::addDatabase("QSQLITE");
   database_raw.setDatabaseName("./FFridge-test.db");
   if (!database_raw.isOpen())
   {
       std::cout << "connected to database raw" << std::endl;
   }
   DbManager db = DbManager("./FFridge-test.db");
   QSqlQuery checklockQuery(database_raw);

   //acquire lock outside of dbManager
   if(checklockQuery.exec("BEGIN EXCLUSIVE"))
   {
       std::cout << "Acquired lock without dbManager" << std::endl;
       // check with dbmanager if db is available
       BOOST_CHECK_EQUAL(db.isDatabaseLocked(),true);

       // release lock
       checklockQuery.exec("COMMIT");
   }
   else
   {
       std::cout << "couldn't acquire lock without dbManager, failing test" << std::endl;
       // in case something goes horribly wrong
       BOOST_CHECK_EQUAL(0,true);
   }
}

// test currently fails. for some reason there is a lock.

BOOST_AUTO_TEST_CASE( CreateTables )
{
    DbManager *db =  new DbManager("./FFridge-test.db");
    // create tables
    db->createTables();
    delete db;

    // access database raw
    QSqlDatabase database_raw = QSqlDatabase::addDatabase("QSQLITE");
    database_raw.setDatabaseName("./FFridge-test.db");
    if (database_raw.isOpen())
    {
        std::cout << "conneected to database raw" << std::endl;
    }

    QSqlQuery checktableQuery(database_raw);
    bool success = false;

    std::cout << "Acquired lock. checking tables" << std::endl;
    QStringList tables;
    tables.append("Products");
    tables.append("Index");
    tables.append("User_Table");
    tables.append("Admin_Table");
    tables.append("Delivery_Driver");
    tables.append("Suppliers");
    for (int i = 0; i < tables.count(); i++)
    {
        checktableQuery.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name=?");
        checktableQuery.addBindValue(tables.at(i));
        if(checktableQuery.exec())
        {
            BOOST_CHECK_EQUAL(checktableQuery.seek(0),1);
        }
    }
}


BOOST_AUTO_TEST_CASE( SingleAddProduct )
{
    DbManager db = DbManager("./FFridge-test.db");
    // ensure tables made
    db.createTables();

    // add product
    const QString product = "Apple";
    BOOST_CHECK_EQUAL(db.addProduct(product),true);
}


BOOST_AUTO_TEST_CASE( SingleProductExists )
{
    DbManager db = DbManager("./FFridge-test.db");
    //check product is in table
    const QString product = "Apple";
    BOOST_CHECK_EQUAL(db.productExists(product), true);
}


BOOST_AUTO_TEST_CASE( AddProductMultipleExists )
{
    DbManager db = DbManager("./FFridge-test.db");
    QStringList products;
    products.append("Banana");
    products.append("Chery");
    products.append("Milk");

    for (int i = 0; i < products.count(); i++) {
       BOOST_CHECK_EQUAL(db.addProduct(products.at(i)),true);
    }
}

BOOST_AUTO_TEST_CASE( DeleteOldestProduct )
{
    DbManager db = DbManager("./FFridge-test.db");

    QString product = "apple";

    std::cout << "Removing apple" << std::endl;
    db.removeOldestItem(product);
    BOOST_CHECK_EQUAL(db.productExists(product),false);
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( Main_Test )

BOOST_AUTO_TEST_CASE( First )
{

}

BOOST_AUTO_TEST_SUITE_END()
