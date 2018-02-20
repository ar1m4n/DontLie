#include "allmodels.hxx"

Advertising::Advertising(const QString &url, const QString &description)
    :m_url(url), m_description(description)
{

}

const QString &Advertising::Url() const
{
    return m_url;
}

void Advertising::Url(const QString &url)
{
    m_url = url;
}

const QString & Advertising::Description() const
{
    return m_description;
}

void Advertising::Description(const QString &description)
{
    m_description = description;
}

ImageHashes &Advertising::ImageHashesList()
{
    return m_imageHashes;
}
