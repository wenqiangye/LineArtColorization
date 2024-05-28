/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */

#include "colormodel.h"
#include "MatProcess.h"

void COLORMODEL::ColorModel::getImgTensors(const QImage &img, std::vector<float> &tensor_values, const int &channel_cnt)
{
    // std::cout << img.format() << ' ' << QImage::Format_Grayscale8 << std::endl;
    cv::Mat res_mat;
    OpenCVMat_Process::QImage2Mat(img, res_mat);

    cv::Mat normalized_image;

    OpenCVMat_Process::MatNormalize(res_mat, normalized_image, channel_cnt);
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
    getImgTensors(sketch_image, sketch_tensors, 1);

    std::cout << "sketch_tensors num " << sketch_tensors.size() << std::endl;
    if(ref_image.isNull())
    {
        std::cout << "ref_image is null" << std::endl;
        return;
    }
    std::vector<float> ref_tensors;
    getImgTensors(ref_image, ref_tensors, 3);
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
    OpenCVMat_Process::MatToQImage(output_image, gen_image);
}

void COLORMODEL::ColorModel::RunModel(const QImage &sketch_image, const QImage &ref_image, QImage &gen_image, cv::Mat &sketch_image_mat, cv::Mat &current_ref_image_mat)
{
    std::cout << "run mat model" << std::endl;
    if(sketch_image.isNull() || sketch_image_mat.empty())
    {
        std::cout << "sketch_image is null" << std::endl;
        return;
    }
    std::vector<float> sketch_tensors;
    GetMatTensor(sketch_image_mat, sketch_tensors, 1);

    std::cout << "sketch_tensors num " << sketch_tensors.size() << std::endl;
    if(ref_image.isNull() || current_ref_image_mat.empty())
    {
        std::cout << "ref_image is null" << std::endl;
        return;
    }
    std::vector<float> ref_tensors;
    GetMatTensor(current_ref_image_mat, ref_tensors, 3);
    std::cout << "ref_tensors num " << ref_tensors.size() << std::endl;

    const char *input_node_names[] = {"sketch", "reference"};
    const char *output_node_names[] = {"output"};
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

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

    //rgb
    channel_mats[0] = cv::Mat(height, width, CV_32F, output_data + 0 * height * width);
    channel_mats[1] = cv::Mat(height, width, CV_32F, output_data + 1 * height * width);
    channel_mats[2] = cv::Mat(height, width, CV_32F, output_data + 2 * height * width);

    cv::Mat output_image;
    cv::merge(channel_mats, output_image);

    // 转换为 8 位无符号整数
    output_image.convertTo(output_image, CV_8UC3, 255.0);
    OpenCVMat_Process::MatToQImage(output_image, gen_image);
}

void COLORMODEL::ColorModel::GetMatTensor(const cv::Mat &img, std::vector<float> &tensor_values, const int &channel_cnt)
{
    cv::Mat float_image, gray_image;
    cv::Mat res_mat = img.clone();

    cv::Mat normalized_image;
    OpenCVMat_Process::MatNormalize(res_mat, normalized_image, channel_cnt);

    std::vector<cv::Mat>chw_image;
    cv::split(normalized_image, chw_image);

    for(int c = 0; c < channel_cnt; ++c)
    {
        tensor_values.insert(tensor_values.end(), (float *)chw_image[c].datastart, (float *)chw_image[c].dataend);
    }
}

