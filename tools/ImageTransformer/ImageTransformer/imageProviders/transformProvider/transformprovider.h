#ifndef TRANSFORMPROVIDER_H
#define TRANSFORMPROVIDER_H

#include <QQuickImageProvider>
#include <QMap>
#include <QImage>
#include <QUrl>

class TransformProvider : public QQuickImageProvider
{
public:
    TransformProvider(QUrl baseUrl);

    //virtual QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize, bool requestedAutoTransform);
    virtual QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize);
private:
    QUrl m_baseUrl;
    QMap<QString, QImage> m_imageCache;
    QImage m_image;
    QString m_resolvedId;
};

#endif // TRANSFORMPROVIDER_H
