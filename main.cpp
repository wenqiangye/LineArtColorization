/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "Loger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "MultiGuideLineArtColorization_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.setWindowTitle("LineArtColorization");
    // LOG_BASE::LOG *Log = LOG_BASE::LOG::getInstance();
    LOG_DEBUG(LOG_BASE::LOGLEVEL::LOG_LEVEL_DEBUG, "Start Project!");
    w.show();
    return a.exec();
}
