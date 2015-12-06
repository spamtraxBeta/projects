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

#include "diriterator.h"
#include <QDir>

DirIterator::DirIterator(QQuickItem *parent) : QQuickItem(parent)
{

}
QStringList DirIterator::fileTypes() const
{
    return m_fileTypes;
}

void DirIterator::componentComplete()
{
    QQuickItem::componentComplete();

    updateIterator(this->dir(), this->fileTypes());
}

QString DirIterator::currentFile() const
{
    return m_dir + "/" + m_currentFile;
}

void DirIterator::setCurrentFile(const QString &currentFile)
{
    if (m_currentFile == currentFile)
    {
        return;
    }
    m_currentFile = currentFile;
    emit currentFileChanged();
}

void DirIterator::setFileTypes(QStringList fileTypes)
{
    if (m_fileTypes == fileTypes)
        return;

    m_fileTypes = fileTypes;
    emit fileTypesChanged(fileTypes);
}


QString DirIterator::dir() const
{
    return m_dir;
}

void DirIterator::setDir(const QString &dir)
{
//
//    if (m_dir == dir)
//    {return;}
    m_dir = dir;

    updateIterator(m_dir, this->fileTypes());

    emit dirChanged();
    this->next();
}

void DirIterator::next()
{
    QString result;

    if (m_files.empty())
    {
        result = "";
    }
    else if (m_iterator == m_files.end())
    {
        m_iterator = m_files.begin();
        result = *m_iterator;
    }
    else
    {
        if (m_iterator == m_files.end())
        {
            m_iterator = m_files.begin();
        }
        result = *m_iterator;
        ++m_iterator;

    }
    this->setCurrentFile(result);
}

void DirIterator::previous()
{
    QString result;

    if (m_files.empty())
    {
        result = "";
    }
    else if (m_iterator == m_files.end())
    {
        m_iterator = m_files.begin();
        result = *m_iterator;
    }
    else
    {
        if (m_iterator == m_files.begin())
        {
            result = *m_iterator;
            m_iterator = m_files.end();
            --m_iterator;
        }
        else
        {
            result = *m_iterator;
            --m_iterator;
        }
    }
    this->setCurrentFile(result);
}

void DirIterator::updateIterator(QString dir, QStringList fileTypes)
{
    QUrl url(dir);

    QDir qd(url.toLocalFile());

    if (this->isComponentComplete() == true)
    {
        m_files = qd.entryList(fileTypes, QDir::Files, QDir::Name);
        m_iterator = m_files.begin();
    }
    else
    {
        // component not complete yet, nothing to do, this method will
        // be caled again from DirIterator::componentComplete()
    }
}
