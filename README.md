# 一、项目介绍

基于QT6的线稿图自动上色系统，主要功能即，添加线稿图、参考图片以及随机涂鸦图片，然后通过算法模型生成一张具有颜色的保留线稿图结果特征并且具有参考图片颜色特征以及随机涂鸦图片颜色增强的生成图片。算法模型通过训练出来之后，模型部署通过OnnxRuntime进行部署推理。图像处理库采用OpenCV。

简单日志类；深度学习模型类；etc

界面截图：

初始界面：

![image-20240706105059015](https://cdn.jsdelivr.net/gh/wenqiangye/yesky_image/img/image-20240706105059015.png)

运行：

![image-20240706105212773](https://cdn.jsdelivr.net/gh/wenqiangye/yesky_image/img/image-20240706105212773.png)