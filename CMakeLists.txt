cmake_minimum_required(VERSION 3.21)

project(LQHJ20 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

# 1. 查找Qt6模块（保持不变）
find_package(Qt6 6.8 REQUIRED COMPONENTS Core Quick Qml Multimedia Gui)

# 2. 批量扫描src下的C++源文件（增加过滤，避免扫到无关文件）
file(GLOB_RECURSE PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
# 关键：过滤掉build目录下的文件（如果有的话）
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "${CMAKE_BINARY_DIR}/*")

# 3. 声明可执行文件（只放入口文件和qrc，C++源文件移到qml模块里）
qt_add_executable(appLQHJ20
    src/main.cpp          # 仅入口文件
    qml.qrc               # 资源文件
)

# 4. 配置QML模块（核心改动：把C++源文件移到SOURCES里）
qt_add_qml_module(appLQHJ20
    URI LQHJ20
    VERSION 1.0
    # QML文件（保持不变）
    QML_FILES
        qml/Main.qml
        qml/view/MainMenuView.qml
        qml/view/StoryView.qml
        qml/view/GameView.qml
        qml/view/SettingsView.qml
        qml/components/ChessPiece.qml
        qml/components/CustomButton.qml
        qml/components/Dialog.qml
    # 所有C++源文件放在这里（MOC会正确处理Q_OBJECT/Q_PROPERTY）
    SOURCES
        ${PROJECT_SOURCES}
        src/data/SaveManager.cpp
            src/data/ResourceManager.cpp
)

# 5. 包含路径（保持不变，目标名一致）
target_include_directories(appLQHJ20
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

# 6. 链接Qt库（保持不变）
target_link_libraries(appLQHJ20
    PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::Qml
        Qt6::Multimedia
        Qt6::Gui
)

# 7. Qt6运行时部署（保持不变）
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(appLQHJ20)
endif()
