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
    if(pRefBasedModel.get() == nullptr)
    {
        pRefBasedModel = std::make_unique<COLORMODEL::ColorModel>();
    }

    connect(ui->actionAdd_LineArt, SIGNAL(triggered()), this, SLOT(onAddLineArtPushButtonClicked()));
    connect(ui->actionAdd_reference, SIGNAL(triggered()), this, SLOT(onAddRefPushButtonClicked()));
    connect(ui->actionAdd_Scribble, SIGNAL(triggered()), this, SLOT(onAddScriblePushButtonClicked()));
    connect(ui->actionSave_gen_image, SIGNAL(triggered()), this, SLOT(onSaveGenImageClicked()));


    connect(ui->add_line_art_pushButton, SIGNAL(clicked()), this, SLOT(onAddLineArtPushButtonClicked()));
    connect(ui->add_ref_pushButton, SIGNAL(clicked()), this, SLOT(onAddRefPushButtonClicked()));
    connect(ui->add_scrible_pushButton, SIGNAL(clicked()), this, SLOT(onAddScriblePushButtonClicked()));
    connect(ui->gen_image_pushButton, SIGNAL(clicked()), this, SLOT(onGenImagePushButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAddLineArtPushButtonClicked()
{
    LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "add line art image button clicked!")
    QString image_path = QFileDialog::getOpenFileName(this, tr("Open Line Art Image"),
                         QCoreApplication::applicationDirPath(), tr("*.png *.jpg"));
    if(sketchImage.load(image_path))
    {
        LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "open sketch " + image_path.toStdString() + " image");
        ui->sketch_label->setPixmap(QPixmap::fromImage(sketchImage));
        sketchImageMat = cv::imread(image_path.toStdString(), cv::IMREAD_COLOR);
    }
}


void MainWindow::onAddRefPushButtonClicked()
{
    LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "add ref image button clicked!")
    QString image_path = QFileDialog::getOpenFileName(this, tr("Open Reference Image"),
                         QCoreApplication::applicationDirPath(), tr("*.png *.jpg"));
    if(pRefImageModel.get() == nullptr)
    {
        pRefImageModel = std::make_unique<QStandardItemModel>(this);
    }
    if(currentRefImage.load(image_path))
    {
        LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "open ref " + image_path.toStdString() + " image");
        refImages.push_back(currentRefImage);
        QPixmap ref_image_pix = QPixmap::fromImage(currentRefImage);
        QStandardItem *tmp_ref_image = new QStandardItem(QIcon(ref_image_pix), QString(""));
        pRefImageModel.get()->appendRow(tmp_ref_image);
        ui->listView->setModel(pRefImageModel.get());
        currentRefImageMat = cv::imread(image_path.toStdString(), cv::IMREAD_COLOR);
    }
    else
    {
        LOG_ERROR(LOG_BASE::LOG_LEVEL_ERROR, "can not open " + image_path.toStdString() + " image");
    }
}


void MainWindow::onGenImagePushButtonClicked()
{
    if(pRefBasedModel.get() == nullptr)
    {
        LOG_WARN(LOG_BASE::LOG_LEVEL_WARNING, "not load color model!")
        return;
    }

    pRefBasedModel.get()->RunModel(sketchImage, currentRefImage, genImage, sketchImageMat, currentRefImageMat);
    if(!genImage.isNull())
    {
        ui->gen_label->setPixmap(QPixmap::fromImage(genImage));
    }
}


void MainWindow::onAddScriblePushButtonClicked()
{
    LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "add scrible image button clicked!")
    QString image_path = QFileDialog::getOpenFileName(this, tr("Open Scrible Image"),
                         QCoreApplication::applicationDirPath(), tr("*.png *.jpg"));
    if(scribleImage.load(image_path))
    {
        LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "open scrible " + image_path.toStdString() + " image");
        ui->scrible_label->setPixmap(QPixmap::fromImage(scribleImage));
        scribleImageMat = cv::imread(image_path.toStdString(), cv::IMREAD_COLOR);
    }
}

void MainWindow::onSaveGenImageClicked()
{
    if(genImage.isNull())
    {
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save gen Image"), QCoreApplication::applicationDirPath(), tr("PNG Files (*.png);;JPG Files (*.jpg)"));
    if(!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);
        QString suffix = fileInfo.suffix().toLower();
        if(suffix.isEmpty())
        {
            fileName.append(".png");
        }
        else if(suffix != "png" && suffix != "jpg")
        {
            fileName.append(".png");
        }
        if(!genImage.save(fileName))
        {
            qWarning("Failed to save gen image.");
        }
    }
}


