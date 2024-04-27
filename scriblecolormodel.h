/*
 * @Author: yesky a316606581@gmail.com
 * @LastEditors: yesky a316606581@gmail.com
 * Copyright (c) 2024 by yesky email: a316606581@gmail.com, All Rights Reserved.
 */
#ifndef SCRIBLECOLORMODEL_H
#define SCRIBLECOLORMODEL_H

#include "modelbase.h"
#include <string>


namespace COLORMODEL {
class ScribleColorModel:public ModelBase{
public:
    ScribleColorModel();
    virtual ~ScribleColorModel();
    virtual void runmodel(const std::string& model_path, const std::string& model_name);
private:
    std::string model_name;
    std::string model_path;
};
}

#endif // SCRIBLECOLORMODEL_H
