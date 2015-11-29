#ifndef DIRITERATOR_H
#define DIRITERATOR_H

#include <QQuickItem>

class DirIterator : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString dir READ dir WRITE setDir NOTIFY dirChanged)
    Q_PROPERTY(QStringList fileTypes READ fileTypes WRITE setFileTypes NOTIFY fileTypesChanged)
    Q_PROPERTY(QString currentFile READ currentFile NOTIFY currentFileChanged)

public:
    explicit DirIterator(QQuickItem *parent = 0);

    QStringList fileTypes() const;

    QString dir() const;
    void setDir(const QString &dir);

    Q_INVOKABLE void next();


    QString currentFile() const;
    void setCurrentFile(const QString &currentFile);

signals:

    void fileTypesChanged(QStringList fileTypes);
    void dirChanged();
    void currentFileChanged();

public slots:

    void setFileTypes(QStringList fileTypes);

protected:
     void componentComplete() Q_DECL_OVERRIDE;

private:
    QStringList m_fileTypes;
    QString m_dir;
    QString m_currentFile;

    QStringList m_files;
    QStringList::iterator m_iterator;

    void updateIterator(QString dir, QStringList fileTypes);
};

#endif // DIRITERATOR_H
