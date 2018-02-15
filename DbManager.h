#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DbManager
{
public:
    DbManager(const QString& path);
    void AddRentalListing(const QString& url, const QString &description);

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
