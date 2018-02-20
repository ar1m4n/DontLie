#include "allmodels.hxx"

ImageHash::ImageHash(const QString &hash)
    :m_hash(hash)
{

}

const QString &ImageHash::Hash() const
{
    return m_hash;
}

void ImageHash::Hash(const QString &hash)
{
    m_hash = hash;
}

Advertisings &ImageHash::AdvertisingsList()
{
    return m_Advertisings;
}
