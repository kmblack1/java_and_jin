#include <stdlib.h>
#include <stdarg.h>
#include <uuid.h>
#include "KcUtility.h"
#include "JinMain.h"

struct KC_JIN *global_kcjin = NULL;


void Java_cn_kuncb_JinExternal_kcInit(JNIEnv *env, jclass clazz) {
    struct KC_JIN *kcJin = NULL;
    kcJin = (struct KC_JIN *) calloc(1, sizeof(struct KC_JIN));
    if (NULL == kcJin)
        goto KC_ERROR_CLEAR;
    kcJin->error = createStringBuffer();
    kcJin->str = createStringBuffer();
    global_kcjin = kcJin;
    return;
KC_ERROR_CLEAR:
    JIN_SAFE_KC_JIN_FREE(kcJin);

}

void Java_cn_kuncb_JinExternal_kcUninit(JNIEnv *env, jclass clazz) {
    JIN_SAFE_KC_JIN_FREE(global_kcjin);
}

#define KC_UUID_LEN (16)

int32_t kcUuidOut(const struct uuid_st *uuid, char flag, StringBuffer strUuid, StringBuffer error) {
    static const char HEX_CHARS[] = "0123456789ABCDEF";
    uint8_t hi, lo;
    const uint8_t *src;

    KC_VALIDATE_PARA_PTR(uuid, error);
    src = (const uint8_t *) uuid;

    resetStringBuffer(strUuid);
    for (uint8_t i = 0; i < KC_UUID_LEN; ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10)
            appendStringBufferChar(strUuid, flag);
        hi = (src[i] >> 4) & 0x0F;
        lo = src[i] & 0x0F;
        appendStringBufferChar(strUuid, HEX_CHARS[hi]);
        appendStringBufferChar(strUuid, HEX_CHARS[lo]);
    }
    return KC_OK;
KC_ERROR_CLEAR:
    return KC_FAIL;
}

jstring Java_cn_kuncb_JinExternal_kcUuid(JNIEnv *env, jclass clazz) {
    int32_t rc;
    StringBuffer error, str;

    jstring jresult = NULL;

    uuid_rc_t urc;
    struct uuid_st *uuid = NULL;

    if (NULL == global_kcjin)
        JIN_KC_THROW_NULL_POINTER_ERROR(env, JIN_NOT_INITIALIZED);
    error = global_kcjin->error;
    str = global_kcjin->str;


    urc = uuid_create(&uuid);
    KC_CHECK_UUID_RC(urc, error);
    urc = uuid_make(uuid, UUID_MAKE_V4); //从随机数产生
    KC_CHECK_UUID_RC(urc, error);

    KC_CHECK_RCV2(kcUuidOut(uuid, '-', str, error));

    JIN_KC_SAFE_NEW_STRING_UTF_V1(env, jresult, str->data, error);
    KC_SAFE_UUID(uuid);
    return jresult;
KC_ERROR_CLEAR:
    KC_SAFE_UUID(uuid);
    JIN_KC_SAFE_OBJECT_DELETELOCAL(env, jresult);
    JIN_KC_THROW_RUNTIME_ERROR(env, error->data);
}