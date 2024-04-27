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
    class ColorModel: public ModelBase
    {
    public:
        explicit ColorModel();
        ColorModel(const std::string &_model_path, const std::string &_model_name);
        virtual ~ColorModel();
        virtual void runmodel(const QImage &sketch_image, const QImage &ref_image);
        virtual void runmodel() override;
        bool CheckStatus(const OrtApi *g_ort, OrtStatus *status);
        cv::Mat QImage2Mat(const QImage &img);
    private:
        std::string model_name = "reference_based.onnx";
        std::string model_path = "/home/yesky/code/QT_Projects/MultiGuideLineArtColorization/line_art_model/reference_based.onnx";
        const OrtApi *g_ort = NULL;
        const OrtApiBase *ptr_api_base;
        OrtEnv *env;
        OrtSessionOptions *session_options;

        //CUDA option set
        OrtCUDAProviderOptions cuda_option;
        OrtSession *session;
        OrtAllocator *allocator;

        //**********输入信息**********//
        size_t num_input_nodes; //输入节点的数量

        std::vector<const char *> input_node_names; //输入节点的名称
        std::vector<std::vector<int64_t>> input_node_dims; //输入节点的维度
        std::vector<ONNXTensorElementDataType> input_types; //输入节点的类型
        std::vector<OrtValue *> input_tensors; //输入节点的tensor

        //***********输出信息****************//

        size_t num_output_nodes;
        std::vector<const char *> output_node_names;
        std::vector<std::vector<int64_t>> output_node_dims;
        std::vector<OrtValue *> output_tensors;

        cv::Mat mat_sketch_image;
        cv::Mat mat_ref_image;
    };
}
#endif // COLORMODEL_H
