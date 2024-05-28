#ifndef MATPROCESS_H
    #define MATPROCESS_H

#endif // MATPROCESS_H
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/dnn.hpp>

namespace OpenCVMat_Process
{
    void QImage2Mat(const QImage &img, cv::Mat &res_mat);
    void MatToQImage(const cv::Mat &mat, QImage &gen_image);

    void MatNormalize(const cv::Mat &mat, cv::Mat &normalize_mat, const int channels_cnt);
}


