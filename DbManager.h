#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

struct ListingMatch
{
    ListingMatch(const QString & = "", const QString & = "", double = 0);
    QString firstUrl, secondUrl;
    double matchPercent = 0;
};

class DbManager
{
public:
    DbManager(const QString& path);
    void AddRentalListing(const QString& url, const QString &description);
    QVector<ListingMatch> GetLevenshtineDistance(const QString &url, const QString &descr);

private:
    double CalculateLevenshteinDistance(const QString & s1, const QString &s2);
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
