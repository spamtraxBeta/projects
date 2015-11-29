/******************************************************************************
* This file is part of ImageTransformer.                                      *
*                                                                             *
* RelaisCard is free software : you can redistribute it and / or modify       *
* it under the terms of the GNU General Public License as published by        *
* the Free Software Foundation, either version 3 of the License, or           *
* (at your option) any later version.                                         *
*                                                                             *
* RelaisCard is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the                 *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License           *
* along with RelaisCard.If not, see <http://www.gnu.org/licenses/>.           *
*******************************************************************************/

#include "transformprovider.h"
#include <QDebug>

TransformProvider::TransformProvider(QUrl baseUrl):
    QQuickImageProvider(QQuickImageProvider::Image)
  , m_baseUrl(baseUrl)
{

}

QImage TransformProvider::requestImage(const QString &id, QSize *size, const QSize& requestedSize)
{
    Q_UNUSED(requestedSize)

    QStringList idParts = id.split("::");

    QString image = idParts[0];
    QString actions = idParts.length() > 1 ? idParts[1] : "";

    QUrl resolvedUrl = m_baseUrl.resolved(image);

    QString resolvedId = resolvedUrl.path();

    if (resolvedUrl.scheme() == "qrc")
    {
        resolvedId = QString(":%1").arg(resolvedId);
    }
    else if (resolvedUrl.isLocalFile())
    {
        resolvedId = resolvedUrl.toLocalFile();
    }

    QImage result;
#if USE_CACHE != 0
    if (m_imageCache.contains(resolvedId) == false)
    {
        if (result.load(resolvedId))
        {
            //m_imageCache[resolvedId] = result;
        }
        else
        {
            qDebug()<<"failed to load: "<<resolvedId;
        }
    }
    else
    {
        result = m_imageCache[resolvedId];
    }
#else
    if (resolvedId == m_resolvedId)
    {
        result = m_image;
    }
    else if (m_image.load(resolvedId))
    {
        m_resolvedId = resolvedId;
        result = m_image;
    }
    else
    {
        qDebug()<<"failed to load: "<<resolvedId;
        m_resolvedId = "";
    }
#endif
    QStringList actionList = actions.split(",");

    bool modified = false;
    for (QStringList::iterator iter = actionList.begin(); iter != actionList.end(); ++iter)
    {
        if (*iter == "flipH")
        {
            result = result.mirrored(true, false);
            modified = true;
        }
        else if (*iter == "flipV")
        {
            result = result.mirrored(false, true);
            modified = true;
        }
        else if (*iter == "rot90cw")
        {
            QTransform trans;
            trans.rotate(90);
            result = result.transformed(trans, Qt::FastTransformation);
            modified = true;
        }
        else if (*iter == "rot180cw")
        {
            QTransform trans;
            trans.rotate(180);
            result = result.transformed(trans, Qt::FastTransformation);
            modified = true;
        }
        else if (*iter == "rot270cw")
        {
            QTransform trans;
            trans.rotate(270);
            result = result.transformed(trans, Qt::FastTransformation);
            modified = true;
        }
        else if (*iter == "save")
        {
            if (modified == false)
            {
                qDebug()<<"not modified, not saving: "<<resolvedId;
            }
            else if (result.isNull() == false)
            {
                if (result.save(resolvedId))
                {
                    qDebug()<<"saved: "<<resolvedId;
                }
                else
                {
                    qDebug()<<"failed to save: "<<resolvedId;
                }
            }
            else
            {
                qDebug()<<"Cannot save invalid image: "<<resolvedId;
            }
        }
        else if (*iter == "")
        {
            // ignore empty action
        }
        else
        {
            qDebug()<<"Ignored unknown action: "<<*iter<<"on: "<<id;
        }
    }


    size->setWidth(result.width());
    size->setHeight(result.height());
    return result;


}

