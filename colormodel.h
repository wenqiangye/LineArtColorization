/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */
#ifndef COLORMODEL_H
#define COLORMODEL_H

#include "modelbase.h"
#include "Loger.h"
#include <onnxruntime_cxx_api.h>
#include <onnxruntime_c_api.h>
#include <cpu_provider_factory.h>
#include <provider_options.h>
#include <onnxruntime_lite_custom_op.h>
#include <onnxruntime_run_options_config_keys.h>
#include <string>
#include <QString>
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/dnn.hpp>

using namespace Ort;
namespace COLORMODEL
{
    class ColorModel
    {
    public:
        explicit ColorModel(const std::string  &model_path) : model_path_(model_path)
        {
            Initialize();
        }
        ColorModel()
        {
            Initialize();
        }
        ~ColorModel()
        {
            delete session_;
        }
        void QImage2Mat(const QImage &img, std::vector<float> &tensor_values, const int &channels_cnt );
        void Initialize()
        {
            env_ = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "ColorModel");
            session_ = new Ort::Session(env_, model_path_.c_str(), session_options_);
            // 获取输入数量和输出数量
            size_t input_count = session_->GetInputCount();
            size_t output_count = session_->GetOutputCount();

            std::cout << "Input Count: " << input_count << std::endl;
            std::cout << "Output Count: " << output_count << std::endl;

            // 遍历输入和输出
            for (size_t i = 0; i < input_count; ++i)
            {
                Ort::TypeInfo type_info = session_->GetInputTypeInfo(i);
                // const Ort::TensorTypeAndShapeInfo tensor_info = type_info.GetTensorTypeAndShapeInfo();
                std::vector<int64_t> input_shape = type_info.GetTensorTypeAndShapeInfo().GetShape();

                std::cout << "Input " << i << " Shape: ";
                for (size_t j = 0; j < input_shape.size(); ++j)
                {
                    std::cout << input_shape[j] << " ";
                    if(i == 0)
                    {
                        input_sketch_shape.push_back(input_shape[j]);
                    }
                    else
                    {
                        input_ref_shape.push_back(input_shape[j]);
                    }
                }
                std::cout << std::endl;
            }

            for (size_t i = 0; i < output_count; ++i)
            {
                Ort::TypeInfo type_info = session_->GetOutputTypeInfo(i);
                std::vector<int64_t> output_shape = type_info.GetTensorTypeAndShapeInfo().GetShape();

                std::cout << "Output " << i << " Shape: ";
                for (size_t j = 0; j < output_shape.size(); ++j)
                {
                    std::cout << output_shape[j] << " ";
                    if(i == 0)
                    {
                        output_ref_shape.push_back(output_shape[j]);
                    }
                    else
                    {
                        output_sketch_shape.push_back(output_shape[j]);
                    }
                }
                std::cout << std::endl;
            }
        }
        void RunModel(const QImage &sketch_image, const QImage &ref_image, QImage &gen_image);
        void MatToQImage(const cv::Mat &mat, QImage &gen_image);
    private:
        std::string model_name = "reference_based.onnx";
        std::string model_path_ = "/home/yesky/PycharmProjects/MultiGuideLineArtColorization/modelonnx/MY_GEN.onnx";
        Ort::SessionOptions session_options_;
        Ort::Env  env_;
        Ort::Session *session_;

        std::vector<int64_t> input_sketch_shape;
        std::vector<int64_t> input_ref_shape;

        std::vector<int64_t> output_sketch_shape;
        std::vector<int64_t> output_ref_shape;
        cv::Mat mat_sketch_image;
        cv::Mat mat_ref_image;
    };
}
#endif // COLORMODEL_H
