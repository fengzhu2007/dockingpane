cmake_minimum_required(VERSION 3.14)

project(dockingpane)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)


message("dirss :${AUTOGEN_BUILD_DIR}")
set(RESOURCES
    src/res.qrc
)

qt5_add_resources(RESOURCE_ADDED ${RESOURCES})


add_library(dockingpane SHARED
    ${RESOURCE_ADDED}
    src/docking_guide.h
    src/docking_guide_cover.h
    src/docking_guide_diamond.h
    src/docking_pane.h
    src/docking_pane_client.h
    src/docking_pane_container.h
    src/docking_pane_container_nclient.h
    src/docking_pane_container_tabbar.h
    src/docking_pane_fixed_window.h
    src/docking_pane_float_window.h
    src/docking_pane_handle.h
    src/docking_pane_layout.h
    src/docking_pane_layout_item_info.h
    src/docking_pane_manager.h
    src/docking_pane_tabbar.h
    src/docking_pane_tabbar_item.h
    src/docking_pane_window_resizer.h
    src/docking_workbench.h
    src/global.h
    src/docking_guide.cpp
    src/docking_guide_cover.cpp
    src/docking_guide_diamond.cpp
    src/docking_pane.cpp
    src/docking_pane_client.cpp
    src/docking_pane_container.cpp
    src/docking_pane_container_nclient.cpp
    src/docking_pane_container_tabbar.cpp
    src/docking_pane_fixed_window.cpp
    src/docking_pane_float_window.cpp
    src/docking_pane_handle.cpp
    src/docking_pane_layout.cpp
    src/docking_pane_layout_item_info.cpp
    src/docking_pane_manager.cpp
    src/docking_pane_tabbar.cpp
    src/docking_pane_tabbar_item.cpp
    src/docking_pane_window_resizer.cpp
    src/docking_workbench.cpp
    src/docking_guide.ui
    src/docking_pane_container_nclient.ui
    src/theme.h
    src/docking_qss.h
    src/docking_qss.cpp
    #src/docking_pane_client_region.h
    #src/docking_pane_client_region.cpp
    #src/docking_region.h
    #src/docking_region.cpp
    #src/docking_pane_client_region_item_info.h
    #src/docking_pane_client_region_item_info.cpp

)

target_link_libraries(dockingpane PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(dockingpane PRIVATE DOCKINGPANE_LIBRARY)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}.dll        ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}.dll
    COMMENT "Copying file to binary directory"
)

