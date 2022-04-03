//
// Created by lxg on 2022/4/3.
//

#ifndef KC_DB9A5BF6_C069_464E_8CD8_7A4EBAEDE329
#define KC_DB9A5BF6_C069_464E_8CD8_7A4EBAEDE329

#include <stdint.h>
#include "stringbuffer.h"
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif    /*__cplusplus 1*/

#pragma region 抛出java异常不要修改
/*****************************************************************************
*   直接抛出java异常
*****************************************************************************/
#define JIN_KC_THROW_NO_CLASSDEF_ERROR(env, message) do{ \
    jclass exClass; \
    const char *className = "java/lang/NoClassDefFoundError"; \
    exClass = (*env)->FindClass(env, className); \
    (*env)->ThrowNew(env, exClass, message); \
     return NULL; \
}while(0)

#define JIN_KC_THROW_NULL_POINTER_ERROR(env, message) do{ \
    jclass exClass; \
    const char *className = "java/lang/NullPointerException"; \
    exClass = (*env)->FindClass(env, className); \
    if (NULL == exClass) {\
        JIN_KC_THROW_NO_CLASSDEF_ERROR(env, message);     \
    } \
    (*env)->ThrowNew(env, exClass, message); \
    return NULL; \
}while(0)


#define JIN_KC_THROW_RUNTIME_ERROR(env, message) do{ \
    jclass exClass; \
    const char *className = "java/lang/RuntimeException"; \
    exClass = (*env)->FindClass(env, className); \
    if (NULL == exClass) {\
        JIN_KC_THROW_NO_CLASSDEF_ERROR(env, message);\
     } \
    (*env)->ThrowNew(env, exClass, message); \
    return NULL; \
}while(0)

#pragma endregion

#pragma region 其它定义

/*****************************************************************************
*   判断jobject是否为NULL
*****************************************************************************/
#define JIN_KC_OBJECT_IS_NULL(env, ptr) (NULL == (ptr) || (*(env))->IsSameObject((env), (ptr), NULL))

/*****************************************************************************
*   验证输入参数是否为NULL指针,如果是直接跳转至异常
*****************************************************************************/
#define JIN_KC_VALIDATE_PARA_PTR(env, ptr, error) do{\
    if (NULL == (ptr) || (*(env))->IsSameObject((env), (ptr), NULL)){\
        resetStringBuffer((error));\
        appendStringBuffer((error),("invalid argument.(%s:%d)"),__FILE__,__LINE__);\
        goto KC_ERROR_CLEAR;\
    }\
}while (0)

/*****************************************************************************
*   安全的创建java UTF-8字符串
*****************************************************************************/

#define JIN_KC_SAFE_NEW_STRING_UTF_V1(env, jresult, utf8, error) do{\
        (jresult) = (*(env))->NewStringUTF((env),(utf8)); \
        KC_CHECK_MALLOC((jresult), (error)); \
}while (0)

#define JIN_KC_SAFE_NEW_STRING_UTF_V2(env, jresult, utf8, error) do{\
    if(NULL == kcUtf8Check((uint8_t *)(utf8)) ){\
        (jresult) = (*(env))->NewStringUTF((env),(utf8)); \
        KC_CHECK_MALLOC((jresult), (error)); \
    }else{ \
        resetStringBuffer((error));\
        appendStringBuffer((error),("invalid UTF-8 String.(%s:%d)"),__FILE__,__LINE__);\
        goto KC_ERROR_CLEAR;\
    }\
}while (0)

/*****************************************************************************
*   安全的创建Bytes
*****************************************************************************/

#define JIN_KC_SAFE_NEW_BYTES(env, jresult, bin, binLen, error) do{\
        (jresult) = (*(env))->NewByteArray((env),(binLen)); \
        KC_CHECK_MALLOC((jresult), (error)); \
        (*env)->SetByteArrayRegion((env), (jresult), 0, (binLen), (jbyte *)(bin)); \
}while (0)

/*****************************************************************************
*   释放jobject对象
*****************************************************************************/
#define JIN_KC_SAFE_OBJECT_DELETELOCAL(env, jobj) do{ \
    if(NULL != (env)){ \
        if (NULL != (jobj)){\
            (*(env))->DeleteLocalRef(env,(jobj)); \
            (jobj) = NULL; \
        } \
    } \
}while (0)

/*****************************************************************************
*   释放GetStringUTFChars获取的资源
*****************************************************************************/
#define JIN_KC_SAFE_STRING_RELEASE_DELETELOCAL(env, jstr, str) do{ \
    if(NULL != (env)){ \
        if (NULL != (jstr)){ \
            if (NULL != (str)){\
                (*(env))->ReleaseStringUTFChars((env),(jstr), (str)); \
            } \
        } \
    } \
}while (0)

/*****************************************************************************
*   释放GetByteArrayElements获取的资源
*****************************************************************************/
#define JIN_KC_SAFE_BYTES_RELEASE_DELETELOCAL(env, jbytes, jbyte) do{ \
    if(NULL != (env)){ \
        if (NULL != (jbytes)){ \
            if (NULL != (jbyte)){\
                (*(env))->ReleaseByteArrayElements((env),(jbytes), (jbyte),(JNI_ABORT)); \
            } \
        } \
    } \
}while (0)

#pragma endregion

#pragma region jin初始化的参数
/*****************************************************************************
*   jin初始化的参数
*****************************************************************************/

struct KC_JIN {
    StringBuffer error;
    StringBuffer str;
};

#define JIN_SAFE_KC_JIN_FREE(ptr) do {\
    if(NULL !=(ptr)  ) {         \
        KC_SAFE_STRINGBUF_FREE((ptr)->str); \
        KC_SAFE_STRINGBUF_FREE((ptr)->error); \
        free((ptr)); (ptr) = NULL; \
    } \
} while (0)


static const char *JIN_NOT_INITIALIZED = "jni is not initialized, please call kcInit initialization first, and call kcUninit to uninstall after using it.";

extern struct KC_JIN *global_kcjin;

JNIEXPORT void JNICALL Java_cn_kuncb_JinExternal_kcInit(JNIEnv *, jclass);
JNIEXPORT void JNICALL Java_cn_kuncb_JinExternal_kcUninit(JNIEnv *, jclass);

#pragma endregion


#pragma region 实现jin函数

JNIEXPORT jstring JNICALL Java_cn_kuncb_JinExternal_kcUuid(JNIEnv * env, jclass clazz);

#pragma endregion

#ifdef __cplusplus
};
#endif  /*__cplusplus 2*/

#endif //KC_DB9A5BF6_C069_464E_8CD8_7A4EBAEDE329
