/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Loger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listView->setIconSize(QSize(256, 256));
    ui->lineartwidget->setStyleSheet(QString::fromUtf8("#lineartwidget{border:2px solid white}"));
    if(pRef_based_model.get() == nullptr)
    {
        pRef_based_model = std::make_unique<COLORMODEL::ColorModel>();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// void MainWindow::on_pushButton_clicked()
// {

// }


void MainWindow::on_add_line_art_pushButton_clicked()
{
    LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "add line art image button clicked!")
    QString image_path = QFileDialog::getOpenFileName(this, tr("Open Line Art Image"),
                         QCoreApplication::applicationDirPath(), tr("*.png *.jpg"));
    if(sketch_image.load(image_path))
    {
        LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "open sketch " + image_path.toStdString() + " image");
        ui->sketch_label->setPixmap(QPixmap::fromImage(sketch_image));
    }
}


void MainWindow::on_add_ref_pushButton_clicked()
{
    LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "add ref image button clicked!")
    QString image_path = QFileDialog::getOpenFileName(this, tr("Open Reference Image"),
                         QCoreApplication::applicationDirPath(), tr("*.png *.jpg"));
    if(pRef_image_model.get() == nullptr)
    {
        pRef_image_model = std::make_unique<QStandardItemModel>(this);
    }
    if(current_ref_image.load(image_path))
    {
        LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "open ref " + image_path.toStdString() + " image");
        ref_images.push_back(current_ref_image);
        QPixmap ref_image_pix = QPixmap::fromImage(current_ref_image);
        QStandardItem *tmp_ref_image = new QStandardItem(QIcon(ref_image_pix), QString(""));
        pRef_image_model.get()->appendRow(tmp_ref_image);
        ui->listView->setModel(pRef_image_model.get());
    }
    else
    {
        LOG_ERROR(LOG_BASE::LOG_LEVEL_ERROR, "can not open " + image_path.toStdString() + " image");
    }
}


void MainWindow::on_gen_image_pushButton_clicked()
{
    if(pRef_based_model.get() == nullptr)
    {
        LOG_WARN(LOG_BASE::LOG_LEVEL_WARNING, "not load color model!")
        return;
    }

    pRef_based_model.get()->runmodel(sketch_image, current_ref_image);
}

