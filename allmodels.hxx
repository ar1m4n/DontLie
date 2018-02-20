#ifndef ALLMODELS_HXX
#define ALLMODELS_HXX

#include <QString>
#include <QList>
#include <odb/qt/lazy-ptr.hxx>
#include <odb/core.hxx>

class ImageHash;
using ImageHashes = QList<QLazySharedPointer<ImageHash>>;

#pragma db object
class Advertising
{
public:
    Advertising(const QString &url = "", const QString &description = "");

    const QString &Url() const;
    void Url(const QString &url);

    const QString &Description() const;
    void Description(const QString &description);

    ImageHashes &ImageHashesList();

private:
    friend class odb::access;

    #pragma db id
    QString m_url;

    QString m_description;

    #pragma db value_not_null unordered
    ImageHashes m_imageHashes;
};

using Advertisings = QList<QLazyWeakPointer<Advertising>>;

#pragma db object
class ImageHash
{
public:
    ImageHash(const QString &hash = "");

    const QString &Hash() const;
    void Hash(const QString &hash);

    Advertisings &AdvertisingsList();

private:
    friend class odb::access;

    #pragma db id
    QString m_hash;

    #pragma db value_not_null unordered
    Advertisings m_Advertisings;
};

#endif // ALLMODELS_HXX
