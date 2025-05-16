cmake_minimum_required(VERSION 3.16)

project(Myosense VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/qcustomplot-source
    ${CMAKE_CURRENT_SOURCE_DIR}/front

)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/qcustomplot-source
                    ${CMAKE_CURRENT_SOURCE_DIR}/libs/myosdk/include
                    #${CMAKE_CURRENT_SOURCE_DIR}/libs/KNN_Data
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/myosdk/lib)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        front/mainwindow.ui
        libs/qcustomplot-source/qcustomplot.cpp
        libs/qcustomplot-source/qcustomplot.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Myosense
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/myoread.hpp src/myoread.cpp
        src/classifier.h
        src/classifier.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Myosense APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
add_definitions(-DDATAPATH=\"${CMAKE_SOURCE_DIR}/libs/KNN_Data/\")

else()
    if(ANDROID)
        add_library(Myosense SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Myosense
            ${PROJECT_SOURCES}
        )
      add_definitions(-DDATAPATH=\"${CMAKE_SOURCE_DIR}/libs/KNN_Data/\")
    endif()
endif()

target_link_libraries(Myosense
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::PrintSupport
            myo64
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Myosense)
endif()
set_target_properties(Myosense PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Myosense
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Myosense)
endif()
