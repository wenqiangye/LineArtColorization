/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */
#ifndef MODELBASE_H
#define MODELBASE_H
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/dnn.hpp>
namespace COLORMODEL
{
    class ModelBase
    {
    public:
        virtual void RunModel(const QImage &sketch_image, const QImage &ref_image, QImage &gen_image) = 0;
        virtual void RunModel(const QImage &sketch_image, const QImage &ref_image, QImage &gen_image,
                              cv::Mat &sketch_image_mat, cv::Mat &current_ref_image_mat) = 0;
        virtual void Initialize() = 0;
        ModelBase() {};
        virtual ~ModelBase() {};
    };
}
#endif // MODELBASE_H
