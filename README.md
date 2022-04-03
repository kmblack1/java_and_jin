
[教学视频](https://www.bilibili.com/video/BV1Xi4y1Q7Sp/)
# 1 声明jin
- 注意包名,后面会使用到
```java
package cn.kuncb;

public class JinExternal {

    static {
        //名称要和dll或so文件名称一至
        System.loadLibrary("kmblack");
    }

    public native static String kcUuid();
}
```

# 2  jin接口编译
```bash
cd D:\MyWork\2022\jin\01\java\src\main\java\cn\kuncb
del /Q /F JinExternal.class
javac -encoding UTF-8 JinExternal.java
```

# 3  查看java方法对应的签名
```bash
javap -classpath D:\MyWork\2022\jin\01\java\src\main\java -s cn.kuncb.JinExternal
```
本例输出
```text
Compiled from "JinExternal.java"
public class cn.kuncb.JinExternal {
  public cn.kuncb.JinExternal();
    descriptor: ()V

  public static native java.lang.String kcUuid();
    descriptor: ()Ljava/lang/String;

  static {};
    descriptor: ()V
}
```

# 4  生成头文件
```bash
#先到源码目录D:\MyWork\2022\jin\01\java\src\main\java生成.h文件
cd D:\MyWork\2022\jin\01\java\src\main\java
del /Q /F cn_kuncb_JinExternal.h
#需要加上包名,否则无法调用
javah -encoding UTF-8 cn.kuncb.JinExternal
#移动到Clion创建的项目D:\MyWork\2022\jin\01\c
move /Y D:\MyWork\2022\jin\01\java\src\main\java\cn_kuncb_JinExternal.h D:\MyWork\2022\jin\01\c
```
# 5 下载并编译OSSP-UUID
[自己编译请参看我的博客](https://blog.csdn.net/kmblack1/article/details/121766014)

如果你不想自己编译, [点这里下载](https://www.kuncb.cn/ossp-uuid.zip) ,这是VC16 x64-86和linux x64-86编译后的二进制文件包.

Ubuntu请使用下列命令安装和查看软件包
```bash
sudo apt-cache search ossp-uuid
sudo apt-get install -y libossp-uuid-dev
dpkg -L libossp-uuid-dev
#C语言查看依赖项
ldd /usr/lib/x86_64-linux-gnu/libossp-uuid.so
#C++查看依赖项
ldd /usr/lib/x86_64-linux-gnu/libossp-uuid++.so
#ossp-uuid在linux上没有依赖其它包,因此可直接运行
```
我们这里使用C语言版本,因为Windows编译的是静态库,它将包含到kmblack库,因此Window上无需操作. 同时将libossp-uuid.so复制到java项目根目录,打包后java会根据平台自动加载对应的ossp-uuid库.

# 6 创建CMakeLists.txt
**注意事项**
- 项目名称必须和在cn\kuncb\JinExternal.java文件中System.loadLibrary("kmblack")指定的名称保持一至;
- 项目类型为动态库

```bash
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
```

# 7 编译我们自己写的jin库
# 7.1 windows
```bash
cd D:\MyWork\2022\jin\01\c
rd /S /Q D:\MyWork\2022\jin\01\c\build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DOSSP_UUID_DIR=E:\tools\library\ossp-uuid
cmake --build build --config Release
```
# 7.2 linux
```bash
cd ~/code/java/01/c
rm -rf build && \
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```




