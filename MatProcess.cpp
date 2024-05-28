#include "MatProcess.h"

void OpenCVMat_Process::QImage2Mat(const QImage &img, cv::Mat &res_mat)
{
    switch (img.format())
    {
        case QImage::Format_ARGB32:
        case QImage::Format_Grayscale8:
            res_mat =  cv::Mat(img.height(), img.width(), CV_8UC1, (void *)img.constBits(), img.bytesPerLine());
            break;
        case QImage::Format_RGB32:
            res_mat = cv::Mat(img.height(), img.width(), CV_8UC4, (void *)img.constBits(), img.bytesPerLine());
            break;
        case QImage::Format_ARGB32_Premultiplied:
            res_mat = cv::Mat(img.height(), img.width(), CV_8UC4, (void *)img.constBits(), img.bytesPerLine());
            break;
        case QImage::Format_RGB888:
            res_mat = cv::Mat(img.height(), img.width(), CV_8UC3, (void *)img.constBits(), img.bytesPerLine());
            cv::cvtColor(res_mat, res_mat, CV_BGR2RGB);
            break;
        case QImage::Format_Indexed8:
            res_mat = cv::Mat(img.height(), img.width(), CV_8UC1, (void *)img.constBits(), img.bytesPerLine());
            break;
    }
}

void OpenCVMat_Process::MatToQImage(const cv::Mat &mat, QImage &gen_image)
{
    // 检查图像类型
    switch (mat.type())
    {
        case CV_8UC1:   // 灰度图像
            {
                // std::cout << "gray" << std::endl;
                gen_image = QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8);
                return;
            }
        case CV_8UC3:   // 彩色图像
            {
                // std::cout << "color" << std::endl;
                gen_image = QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);
                return;
            }
        case CV_8UC4:   // 带有 alpha 通道的彩色图像
            {
                // std::cout << "alpha color" << std::endl;
                gen_image = QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_ARGB32);
                return;
            }
        default:
            throw std::runtime_error("Unsupported cv::Mat format for QImage conversion");
    }
}

void OpenCVMat_Process::MatNormalize(const cv::Mat &mat, cv::Mat &normalized_image, const int channels_cnt)
{
    cv::Mat float_image, gray_image, res_mat;
    res_mat = mat.clone();
    // if img not gray image do cvtColor
    if(mat.channels() != 1 && channels_cnt == 1)
    {
        cv::cvtColor(mat, gray_image, cv::COLOR_BGR2GRAY);
        res_mat = gray_image;
    }
    res_mat.convertTo(float_image, CV_32F, 1.0 / 255.0);

    if(channels_cnt == 1)
    {
        normalized_image = (float_image - 0.5) / 0.5;
    }
    else
    {
        std::vector<cv::Mat>channels(channels_cnt);
        cv::split(float_image, channels);
        for(int i = 0; i < channels_cnt; ++i)
        {
            channels[i] = (channels[i] - 0.5) / 0.5;
        }
        //bgr-=>rgb
        cv::Mat tmp = channels[0].clone();
        channels[0] = channels[2].clone();
        channels[2] = tmp.clone();
        cv::merge(channels, normalized_image);
    }
}
