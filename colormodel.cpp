/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */

#include "colormodel.h"


void COLORMODEL::ColorModel::QImage2Mat(const QImage &img, std::vector<float> &tensor_values, const int &channel_cnt)
{
    // std::cout << img.format() << ' ' << QImage::Format_Grayscale8 << std::endl;
    cv::Mat res_mat;
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

    cv::Mat float_image, gray_image;
    // if img not gray image do cvtColor
    if(channel_cnt == 1 && img.format() != QImage::Format_Grayscale8)
    {
        cv::cvtColor(res_mat, gray_image, cv::COLOR_BGR2GRAY);
        res_mat = gray_image;
    }

    res_mat.convertTo(float_image, CV_32F, 1.0 / 255.0);
    // 减去均值并除以标准差 (假设均值和标准差)
    cv::Mat normalized_image;
    if(channel_cnt == 1)
    {
        normalized_image = (float_image - 0.5) / 0.5;
    }
    else
    {
        std::vector<cv::Mat>channels(channel_cnt);
        cv::split(float_image, channels);
        for(int i = 0; i < channel_cnt; ++i)
        {
            channels[i] = (channels[i] - 0.5) / 0.5;
        }
        cv::merge(channels, normalized_image);
    }

    std::vector<cv::Mat>chw_image;
    cv::split(normalized_image, chw_image);

    for(int c = 0; c < channel_cnt; ++c)
    {
        tensor_values.insert(tensor_values.end(), (float *)chw_image[c].datastart, (float *)chw_image[c].dataend);
    }
}

void COLORMODEL::ColorModel::RunModel(const QImage &sketch_image, const QImage &ref_image, QImage  &gen_image)
{
    if(sketch_image.isNull())
    {
        std::cout << "sketch_image is null" << std::endl;
        return;
    }
    std::vector<float> sketch_tensors;
    QImage2Mat(sketch_image, sketch_tensors, 1);

    std::cout << "sketch_tensors num " << sketch_tensors.size() << std::endl;
    if(ref_image.isNull())
    {
        std::cout << "ref_image is null" << std::endl;
        return;
    }
    std::vector<float> ref_tensors;
    QImage2Mat(ref_image, ref_tensors, 3);
    std::cout << "ref_tensors num " << ref_tensors.size() << std::endl;

    const char *input_node_names[] = {"sketch", "reference"};
    const char *output_node_names[] = {"output"};
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    // Ort::Value input_sketch_tensor = ;
    // Ort::Value input_ref_tensor = ;

    std::vector<Ort::Value> inputs_tensors;
    inputs_tensors.push_back(Ort::Value::CreateTensor<float>(memory_info, sketch_tensors.data(), sketch_tensors.size(), input_sketch_shape.data(), input_sketch_shape.size()));
    memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    inputs_tensors.push_back(Ort::Value::CreateTensor<float>(memory_info, ref_tensors.data(), ref_tensors.size(), input_ref_shape.data(), input_ref_shape.size()));
    auto output_tensors = session_->Run(Ort::RunOptions{nullptr}, input_node_names, inputs_tensors.data(), inputs_tensors.size(), output_node_names, 1);

    std::vector<int64_t>  output_shape = output_tensors[0].GetTensorTypeAndShapeInfo().GetShape();
    int batch_size = output_shape[0];
    int channels = output_shape[1];
    int height = output_shape[2];
    int width = output_shape[3];

    std::cout << batch_size << ' ' << channels << ' ' << height << ' ' << width << std::endl;
    // 获取输出张量的数据
    float *output_data = output_tensors[0].GetTensorMutableData<float>();


    std::vector<cv::Mat> channel_mats(channels);
    for(int i = 0; i < channels; ++i)
    {
        channel_mats[i] = cv::Mat(height, width, CV_32F, output_data + i * height * width);
    }

    cv::Mat output_image;
    cv::merge(channel_mats, output_image);

    // 转换为 8 位无符号整数
    output_image.convertTo(output_image, CV_8UC3, 255.0);
    MatToQImage(output_image, gen_image);
}

void COLORMODEL::ColorModel::MatToQImage(const cv::Mat &mat, QImage &gen_image)
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
                gen_image = QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_BGR888);
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

