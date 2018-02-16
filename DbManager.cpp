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
    while(query.isActive())
    {

    }
    query.clear();
    query.prepare("CREATE TABLE levenshtein (firstUrl text, secondUrl text, matchPersent REAL, PRIMARY KEY (firstUrl, secondUrl))");
    if(!query.exec())
    {
        qDebug() << "Levenshtein table creation fail: " << query.lastError();
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

QVector<ListingMatch> DbManager::GetLevenshtineDistance(const QString &url, const QString &descr)
{
    QVector<ListingMatch> listings;
    QSqlQuery query(m_db);
    query.prepare("SELECT url, description FROM listing");
    if(query.exec())
    {
        while (query.next()) {
            QString testUrl = query.value("url").toString();
            QString testDescr = query.value("description").toString();
            if(testUrl != url)
            {
                auto distance = CalculateLevenshteinDistance(descr, testDescr);
                if(distance >= 50.0)
                {
                    listings.push_back(ListingMatch(url, testUrl, distance));

                    QSqlQuery insertQuery(m_db);
                    insertQuery.prepare("INSERT INTO levenshtein VALUES (:fUrl, :sUrl, :distance)");
                    insertQuery.bindValue(":fUrl", url);
                    insertQuery.bindValue(":sUrl", testUrl);
                    insertQuery.bindValue(":distance", distance);
                    if(!insertQuery.exec())
                    {
                        qDebug() << "INSERT Levenshtein error: " << insertQuery.lastError();
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "SELECT listing error: " << query.lastError();
    }

    return listings;
}

double DbManager::CalculateLevenshteinDistance(const QString &s1, const QString &s2)
{
    // To change the type this function manipulates and returns, change
    // the return type and the types of the two variables below.
    int s1len = s1.size();
    int s2len = s2.size();

    auto column_start = (decltype(s1len))1;

    auto column = new decltype(s1len)[s1len + 1];
    std::iota(column + column_start - 1, column + s1len + 1, column_start - 1);

    for (auto x = column_start; x <= s2len; x++) {
        column[0] = x;
        auto last_diagonal = x - column_start;
        for (auto y = column_start; y <= s1len; y++) {
            auto old_diagonal = column[y];
            auto possibilities = {
                column[y] + 1,
                column[y - 1] + 1,
                last_diagonal + (s1[y - 1] == s2[x - 1]? 0 : 1)
            };
            column[y] = std::min(possibilities);
            last_diagonal = old_diagonal;
        }
    }
    auto result = column[s1len];
    delete[] column;
    double max = std::max(s1len, s2len);
    result = ((max - result) / max) * 100;
    return result;
}

ListingMatch::ListingMatch(const QString &furl, const QString &surl, double d)
    :firstUrl(furl), secondUrl(surl), matchPercent(d)
{

}
