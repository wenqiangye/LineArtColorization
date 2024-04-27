/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */

#include "colormodel.h"

COLORMODEL::ColorModel::ColorModel()
{
    ptr_api_base = OrtGetApiBase();
    g_ort = ptr_api_base->GetApi(ORT_API_VERSION);
    CheckStatus(g_ort, g_ort->CreateEnv(ORT_LOGGING_LEVEL_WARNING, "test", &this->env));

    CheckStatus(g_ort, g_ort->CreateSessionOptions(&this->session_options));
    CheckStatus(g_ort, g_ort->SetIntraOpNumThreads(this->session_options, 1));
    CheckStatus(g_ort, g_ort->SetSessionGraphOptimizationLevel(this->session_options, ORT_ENABLE_BASIC));

    this->cuda_option.device_id = 0;
    this->cuda_option.arena_extend_strategy = 0;
    this->cuda_option.cudnn_conv_algo_search = OrtCudnnConvAlgoSearchExhaustive;
    this->cuda_option.gpu_mem_limit = SIZE_MAX;
    this->cuda_option.do_copy_in_default_stream = 1;

    //CUDA 加速
    CheckStatus(g_ort, g_ort->SessionOptionsAppendExecutionProvider_CUDA(this->session_options, &this->cuda_option));

    //load  model and creat session
    printf("Using Onnxruntime C++ API\n");

    CheckStatus(g_ort, g_ort->CreateSession(this->env, this->model_path.c_str(), this->session_options, &this->session));
    CheckStatus(g_ort, g_ort->GetAllocatorWithDefaultOptions(&this->allocator));
    CheckStatus(g_ort, g_ort->SessionGetInputCount(this->session, &this->num_input_nodes));


    input_node_names.resize(this->num_input_nodes);
    input_node_dims.resize(this->num_input_nodes);
    input_types.resize(this->num_input_nodes);
    input_tensors.resize(this->num_input_nodes);

    for (size_t i = 0; i < this->num_input_nodes; i++)
    {
        // Get input node names
        char *input_name;
        CheckStatus(g_ort, g_ort->SessionGetInputName(this->session, i, this->allocator, &input_name));
        this->input_node_names[i] = input_name;

        // Get input node types
        OrtTypeInfo *typeinfo;
        CheckStatus(g_ort, g_ort->SessionGetInputTypeInfo(this->session, i, &typeinfo));
        const OrtTensorTypeAndShapeInfo *tensor_info;
        CheckStatus(g_ort, g_ort->CastTypeInfoToTensorInfo(typeinfo, &tensor_info));
        ONNXTensorElementDataType type;
        CheckStatus(g_ort, g_ort->GetTensorElementType(tensor_info, &type));
        input_types[i] = type;

        // Get input shapes/dims
        size_t num_dims;
        CheckStatus(g_ort, g_ort->GetDimensionsCount(tensor_info, &num_dims));
        input_node_dims[i].resize(num_dims);
        CheckStatus(g_ort, g_ort->GetDimensions(tensor_info, input_node_dims[i].data(), num_dims));

        size_t tensor_size;
        CheckStatus(g_ort, g_ort->GetTensorShapeElementCount(tensor_info, &tensor_size));

        if (typeinfo)
        {
            g_ort->ReleaseTypeInfo(typeinfo);
        }
    }
    //---------------------------------------------------//


    CheckStatus(g_ort, g_ort->SessionGetOutputCount(session, &num_output_nodes));
    output_node_names.resize(num_output_nodes);
    output_node_dims.resize(num_output_nodes);
    output_tensors.resize(num_output_nodes);

    for (size_t i = 0; i < num_output_nodes; i++)
    {
        // Get output node names
        char *output_name;
        CheckStatus(g_ort, g_ort->SessionGetOutputName(session, i, allocator, &output_name));
        output_node_names[i] = output_name;

        OrtTypeInfo *typeinfo;
        CheckStatus(g_ort, g_ort->SessionGetOutputTypeInfo(session, i, &typeinfo));
        const OrtTensorTypeAndShapeInfo *tensor_info;
        CheckStatus(g_ort, g_ort->CastTypeInfoToTensorInfo(typeinfo, &tensor_info));

        // Get output shapes/dims
        size_t num_dims;
        CheckStatus(g_ort, g_ort->GetDimensionsCount(tensor_info, &num_dims));
        output_node_dims[i].resize(num_dims);
        CheckStatus(g_ort, g_ort->GetDimensions(tensor_info, (int64_t *)output_node_dims[i].data(), num_dims));

        size_t tensor_size;
        CheckStatus(g_ort, g_ort->GetTensorShapeElementCount(tensor_info, &tensor_size));

        if (typeinfo)
        {
            g_ort->ReleaseTypeInfo(typeinfo);
        }
    }
    //---------------------------------------------------//
    printf("Number of inputs = %zu\n", num_input_nodes);
    printf("Number of output = %zu\n", num_output_nodes);
    std::cout << "input_name:" << input_node_names[0] << std::endl;
    std::cout << "output_name: " << output_node_names[0] << std::endl;

}

COLORMODEL::ColorModel::ColorModel(const std::string &_model_path, const std::string &_model_name)
{

}

COLORMODEL::ColorModel::~ColorModel()
{

}

void COLORMODEL::ColorModel::runmodel(const QImage &sketch_image, const QImage &ref_image)
{
    mat_sketch_image = QImage2Mat(sketch_image);
    mat_ref_image = QImage2Mat(ref_image);
    LOG_INFO(LOG_BASE::LOG_LEVEL_INFO, "cover QImage to Mat!")

    //推理


}

void COLORMODEL::ColorModel::runmodel()
{

}

bool COLORMODEL::ColorModel::CheckStatus(const OrtApi *g_ort, OrtStatus *status)
{
    if(status != nullptr)
    {
        const char *msg = g_ort->GetErrorMessage(status);
        std::cerr << msg << std::endl;
        LOG_ERROR(LOG_BASE::LOG_LEVEL_ERROR, "checkstatus error: " + std::string(msg));
        g_ort->ReleaseStatus(status);
        throw Ort::Exception(msg, OrtErrorCode::ORT_EP_FAIL);
    }
    return true;
}

cv::Mat COLORMODEL::ColorModel::QImage2Mat(const QImage &img)
{
    cv::Mat res_mat;
    switch (img.format())
    {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
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
    return res_mat;
}
