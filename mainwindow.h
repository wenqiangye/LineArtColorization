/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemView>
#include <QStandardItemModel>
#include <QFileDialog>
#include "colormodel.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // void on_pushButton_clicked();

    void on_add_line_art_pushButton_clicked();

    void on_add_ref_pushButton_clicked();

    void on_gen_image_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<QStandardItemModel> pRef_image_model;
    std::vector<QImage> ref_images;
    std::unique_ptr<COLORMODEL::ColorModel> pRef_based_model;
    QImage sketch_image;
    QImage scrible_image;
    QImage current_ref_image;
    QImage gen_image;
};
#endif // MAINWINDOW_H
