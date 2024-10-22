cmake_minimum_required(VERSION 3.16)

project(HikeenAT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    hikeenat.cpp
    hikeenat.h
    hikeenat.ui
    logo.rc
    Graphics_api.h
    qt_func.h
    qt_func.cpp
    user.h
    user.cpp
    Intel_include/cApiWrapper.cpp
    Intel_api.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HikeenAT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        logindialog.h logindialog.cpp logindialog.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET HikeenAT APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(HikeenAT SHARED
            ${PROJECT_SOURCES}
        )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
else()
    add_executable(HikeenAT
        ${PROJECT_SOURCES}
    )
endif()
endif()


# 添加头文件目录
target_include_directories(HikeenAT PRIVATE
    ${CMAKE_SOURCE_DIR}/Nvidia_include
    ${CMAKE_SOURCE_DIR}/Intel_include
    ${CMAKE_SOURCE_DIR}/AMD_include
    ${CMAKE_SOURCE_DIR}/Windows_api
)
target_link_libraries(HikeenAT PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# 链接 Windows 库（DXVA2 和 User32）
target_link_libraries(HikeenAT PRIVATE dxva2 user32)
# 添加库文件目录
target_link_directories(HikeenAT PRIVATE
    ${CMAKE_SOURCE_DIR}/Nvidia_include
)
target_link_libraries(HikeenAT PRIVATE nvapi64)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.HikeenAT)
endif()
set_target_properties(HikeenAT PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS HikeenAT
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HikeenAT)
endif()
