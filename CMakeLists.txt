cmake_minimum_required(VERSION 3.5)

project(MultiGuideLineArtColorization VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES MultiGuideLineArtColorization_zh_CN.ts)

# 添加 include 目录
include_directories(${CMAKE_SOURCE_DIR}/include)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MultiGuideLineArtColorization
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        include/ColorModel.h
        include/ModelBase.h
        include/ScribbleColorModel.h
        include/Loger.h
        src/Loger.cpp
        images/images.qrc
        src/ColorModel.cpp
        include/MatProcess.h
        src/MatProcess.cpp
        include/ui_mainwindow.h
        include/DoubleClickeableLabel.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MultiGuideLineArtColorization APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(MultiGuideLineArtColorization SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MultiGuideLineArtColorization
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(MultiGuideLineArtColorization PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# opencv
find_package(OpenCV REQUIRED)
set(ONNXRUNTIME_ROOT_PATH /home/yesky/onnxruntime-linux-x64-gpu-1.17.3)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${ONNXRUNTIME_ROOT_PATH}/include)
link_libraries(${ONNXRUNTIME_ROOT_PATH}/lib)
# onnxruntime

# set(ONNXRUNTIME_INCLUDE_DIRS ${ONNXRUNTIME_ROOT_PATH}/include/)
set(ONNXRUNTIME_LIB ${ONNXRUNTIME_ROOT_PATH}/lib/libonnxruntime.so)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} "${ONNXRUNTIME_ROOT_PATH}/lib/libonnxruntime.so")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MultiGuideLineArtColorization)
endif()
set_target_properties(MultiGuideLineArtColorization PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MultiGuideLineArtColorization
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MultiGuideLineArtColorization)
endif()
