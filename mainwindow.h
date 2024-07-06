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
#include "ColorModel.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/dnn.hpp>

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

    void onAddLineArtPushButtonClicked();

    void onAddRefPushButtonClicked();

    void onGenImagePushButtonClicked();

    void onAddScriblePushButtonClicked();

    void onSaveGenImageClicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<QStandardItemModel> pRefImageModel;
    std::vector<QImage> refImages;
    std::unique_ptr<COLORMODEL::ColorModel> pRefBasedModel;
    QImage sketchImage;
    QImage scribleImage;
    QImage currentRefImage;
    QImage genImage;
    cv::Mat sketchImageMat;
    cv::Mat scribleImageMat;
    cv::Mat currentRefImageMat;
    cv::Mat genImageMat;
};
#endif // MAINWINDOW_H
