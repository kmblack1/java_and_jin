cmake_minimum_required(VERSION 3.11)

#设置项目名
project(kmblack VERSION 1.0.0 LANGUAGES C CXX)

#设置编译器标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (MSVC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  /utf-8")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
ENDIF ()

#查找软件包
IF (MSVC)
    find_path(OSSP_UUID_INCLUDE NAMES uuid.h PATHS ${OSSP_UUID_DIR}/include NO_DEFAULT_PATH)
    find_library(OSSP_UUID_LIB libossp-uuid.lib ${OSSP_UUID_DIR}/lib)
ELSE ()
    find_package(PkgConfig)
    pkg_check_modules(OSSP-UUID REQUIRED ossp-uuid)
ENDIF ()

#导入JIN目录
IF (MSVC)
    SET(JIN_INCLUDE_DIR E:\\jdk\\include
            E:\\jdk\\include\\win32
            )
    INCLUDE_DIRECTORIES(
            ${CMAKE_CURRENT_BINARY_DIR}
            ${JIN_INCLUDE_DIR}
            ${OSSP_UUID_INCLUDE}
    )
ELSE ()
    SET(JIN_INCLUDE_DIR
            /usr/local/jdk/include
            /usr/local/jdk/include/linux
            )
    INCLUDE_DIRECTORIES(
            ${CMAKE_CURRENT_BINARY_DIR}
            ${JIN_INCLUDE_DIR}
            ${OSSP-UUID_INCLUDE_DIRS}
    )
ENDIF ()


#添加所有的源文件
SET(SRC
        JinMain.c
        stringbuffer.c
        )

# 生成动态库文件
add_library(${PROJECT_NAME} SHARED ${SRC})

# 链接库文件
IF (MSVC)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${OSSP_UUID_LIB}
            )
ELSE ()
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ossp-uuid
            )
ENDIF ()