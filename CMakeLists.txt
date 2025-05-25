cmake_minimum_required(VERSION 3.16) # 设置 CMake 的最低版本要求

project(PrayerClockApp VERSION 0.1.0 LANGUAGES CXX) # 设置项目名称和版本，并指定使用的语言

set(CMAKE_CXX_STANDARD 17) # 设置 C++ 标准 (根据你的需求选择)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 模块
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets) # 如果你使用的是 Qt 5，将 Qt6 替换为 Qt5

find_package(EXPAT REQUIRED) # 查找 Expat 库
# 如果 EXPAT_FOUND 为真，则添加其包含目录和链接库
if (EXPAT_FOUND)
  include_directories(${EXPAT_INCLUDE_DIRS})
  message(STATUS "Found Expat: ${EXPAT_LIBRARIES}") # 打印出找到的 Expat 库路径，用于调试
else()
  message(FATAL_ERROR "Expat library not found!") # 如果找不到 Expat，则停止构建
endif()

set(CMAKE_AUTOMOC ON)  # 自动运行 MOC
# set(CMAKE_AUTORCC ON)  # 自动处理资源文件
# set(CMAKE_AUTOUIC ON)  # 自动处理 UI 文件（如果用到）

# 设置可执行文件的名称
set(EXECUTABLE_NAME prayer-clock)

# 指定源文件
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp") # 查找 src 目录下所有 .cpp 和 .h 文件
file(GLOB_RECURSE UI_FILES "res/*.ui") # 查找项目根目录下所有 .ui 文件

# 使用 uic 处理 .ui 文件
qt_wrap_ui(UI_HEADERS ${UI_FILES})

# 包含生成的 UI 头文件和你的源文件
set(SOURCE_FILES
    ${SOURCES}
    ${UI_HEADERS}
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# 创建可执行文件
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# 链接 Qt 库
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Qt::Core Qt::Gui Qt::Widgets ${EXPAT_LIBRARIES})

# 设置安装路径 (可选)
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
# install(FILES ${UI_FILES} DESTINATION share/${PROJECT_NAME}) # 安装 .ui 文件 (可选)

# 如果你需要处理资源文件 (.qrc)
# find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Xml) # 如果你使用的是 Qt 5，将 Qt6 替换为 Qt5
# file(GLOB_RECURSE RESOURCES "*.qrc")
# qt_add_resources(APP_RESOURCES ${RESOURCES})
# target_sources(${EXECUTABLE_NAME} PRIVATE ${APP_RESOURCES})

# 设置 C++ 编译选项 (可选)
# target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra)

# 添加子目录 (如果你的项目有更复杂的目录结构)
# add_subdirectory(src)
# add_subdirectory(lib)
