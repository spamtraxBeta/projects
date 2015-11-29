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

#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>

#include "components/dirIterator/diriterator.h"
#include "imageProviders/transformProvider/transformprovider.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("MaEr Inc.");
    app.setOrganizationDomain("maer.inc");
    app.setApplicationName("ImageTransformer");

    qmlRegisterType<DirIterator>("MaEr", 1, 0, "MDirIterator");

    QQmlApplicationEngine engine;
    //engine.addImageProvider(QLatin1String("transform"), new TransformProvider(QUrl("qrc:///res/img/")));
    engine.addImageProvider(QLatin1String("transform"), new TransformProvider(QUrl("")));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

