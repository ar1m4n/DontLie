#include "DbManager.h"
#include <QSqlQuery>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QSqlError>

DbManager::DbManager(const QString& path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }

//    QSqlQuery query("DROP TABLE listing");
    QSqlQuery query(m_db);
    query.prepare("CREATE TABLE listing (url text primary key, description text)");
    if(!query.exec())
    {
        qDebug() << "Listing table creation fail: " << query.lastError();
    }
}

void DbManager::AddRentalListing(const QString& url, const QString &description)
{
    // you should check if args are ok first...
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO listing (url, description) VALUES (:url, :descr)");
    query.bindValue(":url", url);
    query.bindValue(":descr", description);
    if(!query.exec())
    {
        qDebug() << "AddRentalListing error: " << query.lastError();
    }
}
