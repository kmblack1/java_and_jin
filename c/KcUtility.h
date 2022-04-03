/*****************************************************************************
*	昆明畅博科技公司 Copyright (c) 2014-2022
*	Copyright (c) 2014-2022, Kunming Changbo Technology Co., Ltd.
*	www.kuncb.cn
*	作者：黑哥
*
*	编码标准宏定义
*****************************************************************************/

#ifndef KC_043055AA_F295_42FC_BE3A_D4B24B4AAE54
#define KC_043055AA_F295_42FC_BE3A_D4B24B4AAE54

#include <stdio.h>
#ifdef _MSC_VER
#	include <io.h>
#  include <winsock2.h>
#	include <process.h>
#else
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <arpa/inet.h>
#endif
#include <pthread.h>
#include "KcCore.h"
#include "stringbuffer.h"


/*****************************************************************************
*	自定义函数返回值定义
*****************************************************************************/
#ifndef KC_OK
#define KC_OK		((int32_t)0)
#endif

#ifndef KC_FAIL
#define KC_FAIL		((int32_t)1)
#endif

/*****************************************************************************
*	比较字符串是否为NULL或空
*****************************************************************************/
#define KC_STR_ISNULLOREMPTY(str) (NULL== str || '\0'==str[0] )

/*****************************************************************************
*	循环读写时的缓冲区大小
*	Internet上的标准MTU值为576字节,UDP的数据缓冲区大小控控制在548字节(576-8-20),20字节的IP首部IP协议格式)和8字节的UDP(UDP协议格式)首部.
*****************************************************************************/
#define KC_UDP_BUFFER_SIZE  (548)

/*****************************************************************************
*	文件或目录最大长度
*****************************************************************************/
#define KC_MAX_PATH (1024)

/*****************************************************************************
*	最大允许分配的内存大小（1GB-1）
*****************************************************************************/
#define MaxAllocSize	((size_t) 0x3fffffff) /* 1 gigabyte - 1 */

/*****************************************************************************
*	UTF8 BOM头
*****************************************************************************/
static const char UTF8BOM[4] = { (char)0xEF ,(char)0xBB ,(char)0xBF,(char)0x00 };

/*****************************************************************************
*	将x转换为字符串
*****************************************************************************/
#define KC_2STR(x) #x

/*****************************************************************************
*	C多国语言函数
*****************************************************************************/
#define _TR(x) gettext(x)

/*****************************************************************************
*	浮点与整型转换		注意只能用指针转换
*****************************************************************************/
#define KC_float2int(x) (*((int32_t*)(&x)))
#define KC_int2float(x) (*((float *)((int32_t *)(&x))))
#define KC_float2uint(x) (*((uint32_t*)(&x)))
#define KC_uint2float(x) (*((float *)((uint32_t *)(&x))))
#define KC_double2int64(x) (*((int64_t*)(&x)))
#define KC_int642double(x) (*((double *)((int64_t *)(&x))))
#define KC_double2uint64(x) (*((uint64_t*)(&x)))
#define KC_uint642double(x) (*((double *)((uint64_t *)(&x))))

/*****************************************************************************
*	struct timeval转换为整数毫秒
*****************************************************************************/
#define TIMEVAL2MSEC(x) (NULL ==(x) ? 0 : (((int64_t)((x)->tv_sec)) * 1000) + (((int64_t)((x)->tv_usec)) / 1000))

/*****************************************************************************
*	struct timeval转换为struct timespec结构体
*  millisecond要在struct timespec结构体增加的毫秒数，默认为0L
*****************************************************************************/
#define KC_TIMEVAL2TIMESPEC(tv,abs,millisecond) do{ \
	(tv).tv_usec += ((millisecond) * 1000L); \
	(tv).tv_sec += ((tv).tv_usec / 1000000L); \
	(tv).tv_usec %= 1000000L; \
	(abs).tv_sec = (tv).tv_sec; \
	(abs).tv_nsec = (tv).tv_usec * 1000L; \
}while (0)

/*****************************************************************************
*	SOCKET定义
*****************************************************************************/
#ifdef _MSC_VER
#	define KC_SOCKET_GETLASTERROR WSAGetLastError()

#	define KC_SOCKET_CLOSE(sock) do {\
	if(INVALID_SOCKET!=(sock)) { \
		closesocket((sock)); \
		(sock)=INVALID_SOCKET; \
	} \
} while (0)
#else
typedef int SOCKET;
#	define INVALID_SOCKET  (SOCKET)(~0)
#	define SOCKET_ERROR            (-1)
#	define KC_SOCKET_GETLASTERROR errno
#	define KC_SOCKET_CLOSE(sock) do {\
	if(INVALID_SOCKET!=(sock)) { \
		close((sock)); \
		(sock)=INVALID_SOCKET; \
	} \
} while (0)
#endif
/*****************************************************************************
*	安全释放分配的内容，只能用于本模块内的资源释放
*****************************************************************************/
#define KC_SAFE_FREE(ptr) do {\
	if(NULL !=(ptr)  ) { \
		free((ptr)); (ptr) = NULL; \
	} \
} while (0)

#define KC_SAFE_DELETE(ptr) do {\
	if (NULL !=  (ptr)) {\
		delete (ptr);  (ptr) = NULL;\
	}\
} while (0)

#define KC_FILE_CLOSE(stream) do {\
	if(NULL !=(stream)  ) { \
		fclose((stream)); (stream) = NULL; \
	} \
} while (0)

#define KC_SAFE_UUID(ptr) do {\
	if(NULL !=(ptr)  ) { \
		uuid_destroy((ptr)); (ptr) = NULL; \
	} \
} while (0)
/*****************************************************************************
*   检查分配内存是否成功
*****************************************************************************/
#define KC_CHECK_MALLOC(ptr,error) do{\
	if (NULL == (ptr) ){\
		resetStringBuffer((error));\
		appendStringBuffer((error),("out of memory.(%s:%d)"),__FILE__,__LINE__);\
		goto KC_ERROR_CLEAR;\
	}\
}while (0)

/*****************************************************************************
*   内存操作是检查是否越界
*****************************************************************************/
#define KC_CHEKC_MEM_ERANGE(rc,error) do{\
	if (rc ){\
		resetStringBuffer((error));\
		appendStringBuffer((error),("memory overflow.(%s:%d)"),__FILE__,__LINE__);\
		goto KC_ERROR_CLEAR;\
	}\
}while (0)

/*****************************************************************************
*   检查是否为NULL指针,如果是直接跳转至异常
*****************************************************************************/
#define KC_CHECK_NULL_PTR(ptr) do{\
	if (NULL ==(ptr))\
		goto KC_ERROR_CLEAR;\
}while (0)

/*****************************************************************************
*   验证输入参数是否为NULL指针,如果是直接跳转至异常
*****************************************************************************/
#define KC_VALIDATE_PARA_PTR(ptr,error) do{\
	if (NULL ==(ptr)){\
			resetStringBuffer((error));\
			appendStringBuffer((error),("invalid argument.(%s:%d)"),__FILE__,__LINE__);\
		goto KC_ERROR_CLEAR;\
	}\
}while (0)

/*****************************************************************************
*   验证输入参数是否为0,如果是直接跳转至异常
*****************************************************************************/
#define KC_PARA_LESS_EQUAL_ZERO(para,error) do{\
		if ((para)<=0){\
			resetStringBuffer((error));\
			appendStringBuffer((error),("invalid argument[val<=0].(%s:%d)"),__FILE__,__LINE__);\
			goto KC_ERROR_CLEAR;\
		}\
	}while (0)

/*****************************************************************************
*	向客户端发送一个数组索引超出数据范围异常消息`
*****************************************************************************/
#define KC_CHECK_ARRAY_INDEX(array_index,array_size,error) do{\
	if ((array_index)<0 || (array_index)>=(array_size)){\
		resetStringBuffer((error));\
		appendStringBuffer((error),("array subscript exceeds the range of arrays.(%s:%d)"),__FILE__,__LINE__);\
		goto KC_ERROR_CLEAR;\
	}\
}while (0)

/*****************************************************************************
*   检查标准函数返回代码是否正确,如果不正确直接跳转至异常
*****************************************************************************/
#define KC_CHECK_RCV1(rc,funname,error) do{\
	if ((rc)){\
		resetStringBuffer((error));\
		appendStringBuffer((error),("call function \"%s\" fail.(%s:%d)"),(funname),__FILE__,__LINE__);\
		goto KC_ERROR_CLEAR;\
	}\
}while (0)
/*****************************************************************************
*   检查自定义函数返回代码是否正确,如果不正确直接跳转至异常
*****************************************************************************/
#define KC_CHECK_RCV2(rc) do{\
	if ( KC_OK!=(rc))\
		goto KC_ERROR_CLEAR;\
}while (0)

/*****************************************************************************
*   检查函数返回1表示成功，否则表示失败
*****************************************************************************/
#define KC_CHECK_ONE_RC(rc,funname,error) do{\
	if (!(rc)){\
		resetStringBuffer((error));\
		appendStringBuffer((error),("call function \"%s\" fail.(%s:%d)"),(funname),__FILE__,__LINE__);\
		goto KC_ERROR_CLEAR;\
	}\
}while (0)

/*****************************************************************************
*   直接跳转换至异常
*****************************************************************************/
#define KC_GOTOERR(error,format, ...) do{\
		resetStringBuffer((error));\
		appendStringBuffer((error),(format), ##__VA_ARGS__);\
		goto KC_ERROR_CLEAR;\
}while (0)
/*****************************************************************************
*   获取时间
*****************************************************************************/
#define KC_GETTIMEOFDAY(tv,error) do{\
	if ( 0 != gettimeofday(tv,NULL)){\
		resetStringBuffer((error)); \
		appendStringBuffer((error),("call function \"%s\" fail.(%s:%d)"),"gettimeofday",__FILE__,__LINE__); \
		goto KC_ERROR_CLEAR;\
	} \
}while (0)

/*****************************************************************************
*   雪花算法
*****************************************************************************/
#define KC_SNOWFLAKE_ID_10_12(registerId,snowflakeId,error) do{\
	struct timeval tv; \
	KC_GETTIMEOFDAY(&(tv), (error)); \
	(snowflakeId) = tv.tv_sec; \
	(snowflakeId) <<= 22; \
	(snowflakeId) |= (((registerId) << 12) | 1); \
}while (0)

#define KC_SNOWFLAKE_ID_29_2(registerId,snowflakeId,error) do{\
	struct timeval tv; \
	KC_GETTIMEOFDAY(&(tv), (error)); \
	(snowflakeId) = tv.tv_sec; \
	(snowflakeId) <<= 32; \
	(snowflakeId) |= (((registerId) << 2) | 1); \
}while (0)

/*****************************************************************************
*   设置的内容超过缓冲区大小
*****************************************************************************/
#define KC_SET_BUFFER_EXCEED(pBufBegin,pBufEnd,bufSize,error) do{\
		if(((pBufEnd)-(pBufBegin)) > bufSize) { \
			resetStringBuffer((error));\
			appendStringBuffer((error), ("the contents of the setting exceed the buffer size.(%s:%d)"), __FILE__, __LINE__); \
			goto KC_ERROR_CLEAR;\
		} \
}while (0)


/*****************************************************************************
*   检查UUID函数返回代码是否正确,如果不正确直接跳转至异常
*****************************************************************************/
#define KC_CHECK_UUID_RC(rc,error) do{\
	if (UUID_RC_OK !=(rc)){\
		resetStringBuffer((error));\
		appendStringBuffer((error),("uuid fail:%s"),uuid_error(rc));\
		goto KC_ERROR_CLEAR;\
	}\
}while (0)

/*****************************************************************************
*   FILE操作宏
*****************************************************************************/
//检查文件或目录是否有指定的权限
#define KC_FILE_ACCESS_CHECK(fullfile,flag,isexists,error) do {	\
	if((isexists)){ \
		if (0 != KC_ACCESS((fullfile), 0)) { \
				resetStringBuffer((error));				\
				appendStringBuffer((error), ("file \"%s\" not found."), (fullfile));	\
				goto KC_ERROR_CLEAR;	\
		}	\
	} \
	if (0 != KC_ACCESS((fullfile), (flag))) {	\
		resetStringBuffer((error));	\
		appendStringBuffer((error), ("file \"%s\" access denied."), (fullfile));	\
		goto KC_ERROR_CLEAR;\
	}	\
}  while (0)

#ifdef _MSC_VER
/*打开文件并错误检查*/
#	define KC_FILE_OPEN_AND_CHECK(stream,fullfile,flag,error) do {\
		errno_t errcode;\
		errcode = fopen_s((&(stream)), (fullfile), (flag));\
		if( (errcode) ) {\
			resetStringBuffer((error));\
			appendStringBuffer((error), ("open file \"%s\" failure."), (fullfile));\
			goto KC_ERROR_CLEAR;\
		}\
	} while (0)
#else
#	define KC_FILE_OPEN_AND_CHECK(stream,fullfile,flag,error) do {\
		(stream) = fopen((fullfile), (flag));\
		if( NULL == (stream) ) {\
			resetStringBuffer((error));\
			appendStringBuffer((error), ("open file \"%s\" failure."), (fullfile));\
			goto KC_ERROR_CLEAR;\
		}\
	} while (0)
#endif // _MSC_VER

/*检查文件描述符
https://docs.microsoft.com/en-us/cpp/c-runtime-library/errno-constants?view=msvc-170
EMFILE:打开的文件太多。没有更多的文件描述符可用，因此无法打开更多的文件。
ENFILE:系统中打开的文件过多。
*/
#define KC_FILE_DESCRIPTORS_CHECK(fullfile,error) do {\
	if (errno == EMFILE || errno == ENFILE){\
		resetStringBuffer((error));\
		appendStringBuffer((error),  ("out of file descriptors: \"%s\"; release and retry"), (fullfile));\
		goto KC_ERROR_CLEAR;\
	}\
} while (0)

/*移动文件指针检查*/
#define KC_FILE_SEEK_CHECK(rc,fullfile,error) do {\
	if ((rc)){\
		resetStringBuffer((error));\
		appendStringBuffer((error),  ("could not seek in file \"%s\""), (fullfile));\
		goto KC_ERROR_CLEAR;\
	}\
} while (0)

//循环读取文件是检查是否继续
#define KC_FILE_IS_CONTINUE_READ(fstream)  (!(feof((fstream)) || ferror((fstream))))

//当文件读写完成后检查是否发生错误
#define KC_CHECK_FILE_RW_AFTER_ERROR(fstream,error) do {\
	if (ferror(fstream)) {\
		KC_GOTOERR((error), "%s","an error occurred after reading or writing to the file."); \
	} \
} while (0)

#ifdef _MSC_VER
#   define fseeko(stream, offset, origin) _fseeki64(stream, offset, origin)
#   define ftello(stream) _ftelli64(stream)
#else
#   ifndef fseeko
#       define fseeko(stream, offset, origin) fseeko64(stream, offset, origin)
#   endif
#   ifndef ftello
#       define ftello(stream) ftello64(stream)
#   endif
#endif

/*****************************************************************************
*    获取进程编号
*****************************************************************************/
typedef unsigned int processid_t;
#ifdef _MSC_VER
#   define KC_GET_PROCESS_ID() (_getpid())
#else
#   define KC_GET_PROCESS_ID() (getpid())
#endif

/*****************************************************************************
*    获取线程编号
*****************************************************************************/
#ifdef _MSC_VER
typedef unsigned long ptherad_id;
#   define KC_GET_PTHREAD_ID() (pthread_getw32threadid_np(pthread_self()))
#else
typedef pthread_t ptherad_id;
#   define KC_GET_PTHREAD_ID() (pthread_self())
#endif

/*****************************************************************************
*	设置高质量随机数数因子
*  pid = KC_GET_PTHREAD_ID();
*  gettimeofday(&tv, NULL);
******************************************* **********************************/
#define KC_SET_RANDOM_FACTOR(pid,tv) do{ \
	srand( \
		(uint32_t)( \
			((uint32_t)((pid) << 16)) \
			^ ((uint32_t)(pid)) \
			^ ((uint32_t)((tv)->tv_sec)) \
			^ ((uint32_t)((tv)->tv_usec)) \
		) \
	); \
}while(0)

/*****************************************************************************
*	生成指定范围的随机数
******************************************* **********************************/
#define KC_RANDOM(kmin,kmax)  ( rand() % (kmax  - kmin + 1) + kmin )

/*****************************************************************************
*    copy string (truncating the result)
*****************************************************************************/
#if _MSC_VER >= 1400
# define KC_strcpy(buf, len, src) (void)strncpy_s((buf), (len), (src), _TRUNCATE)
#elif defined(HAVE_STRLCPY)
# define KC_strcpy(buf, len, src) (void)strlcpy((buf), (src), (len))
#else
# define KC_strcpy(buf, len, src) (void)((buf) == NULL || (len) <= 0 || (strncpy((buf), (src), (len) - 1), (buf)[(len) - 1] = '\0') || 1)
#endif

/* copy string up to n chars (sets string to empty on overrun and returns nonzero, zero if OK) */
#if _MSC_VER >= 1400
# define KC_strncpy(buf, len, src, num) ((buf) == NULL || ((size_t)(len) > (size_t)(num) ? strncpy_s((buf), (len), (src), (num)) : ((buf)[0] = '\0', 1)))
#else
# define KC_strncpy(buf, len, src, num) ((buf) == NULL || ((size_t)(len) > (size_t)(num) ? (strncpy((buf), (src), (num)), (buf)[(size_t)(num)] = '\0') : ((buf)[0] = '\0', 1)))
#endif

/* concat string up to n chars (truncates on overrun and returns nonzero, zero if OK) */
#if _MSC_VER >= 1400
# define KC_strncat(buf, len, src, num) ((buf) == NULL || ((size_t)(len) > strlen((buf)) + (size_t)(num) ? strncat_s((buf), (len), (src), (num)) : 1))
#else
# define KC_strncat(buf, len, src, num) ((buf) == NULL || ((size_t)(len) > strlen((buf)) + (size_t)(num) ? (strncat((buf), (src), (num)), (buf)[(size_t)(len) - 1] = '\0') : 1))
#endif

/* copy memory (returns ERANGE on overrun, zero if OK) */
#if _MSC_VER >= 1400
# define KC_MEMCPY(dest, destLen, src, srcLen) ((dest) && (size_t)(destLen) >= (size_t)(srcLen) ? memcpy_s((dest), (destLen), (src), (srcLen)) : ERANGE)
#else
# define KC_MEMCPY(dest, destLen, src, srcLen) ((dest) && (size_t)(destLen) >= (size_t)(srcLen) ? !memcpy((dest), (src), (srcLen)) : ERANGE)
#endif

/* move memory (returns ERANGE on overrun, zero if OK) */
#if _MSC_VER >= 1400
# define KC_MEMMOVE(buf, len, src, num) ((buf) && (size_t)(len) >= (size_t)(num) ? memmove_s((buf), (len), (src), (num)) : ERANGE)
#else
# define KC_MEMMOVE(buf, len, src, num) ((buf) && (size_t)(len) >= (size_t)(num) ? !memmove((buf), (src), (num)) : ERANGE)
#endif


#if _MSC_VER >= 1400
#	define KC_snprintf(buf, len,format, ...) _snprintf_s((buf), (len), _TRUNCATE,(format),##__VA_ARGS__)

#else
#	define KC_snprintf(buf, len, format, ...) snprintf((buf), (len), _TRUNCATE,(format),##__VA_ARGS__)
#endif


#if defined(_MSC_VER)	
#	define KC_INT64_TO_STR(str,i64val)  do{ \
		resetStringBuffer((str));	\
		appendStringBuffer((str), "%I64d", (i64val));\
	}while(0)

#	define KC_UINT64_TO_STR(str,i64val)  do{ \
		resetStringBuffer((str));	\
		appendStringBuffer((str), "%I64u", (i64val));\
	}while(0)

#	define KC_INT_TO_STR(str,i64val)  do{ \
		resetStringBuffer((str));	\
		appendStringBuffer((str), "%d", (i64val));\
	}while(0)

#	define KC_UINT_TO_STR(str,i64val)  do{ \
		resetStringBuffer((str));	\
		appendStringBuffer((str), "%u", (i64val));\
	}while(0)

#	define KC_DOUBLE_TO_STR(str,i64val)  do{ \
		resetStringBuffer((str));	\
		appendStringBuffer((str), "%Lf", (i64val));\
	}while(0)

#	define KC_int642str(buf,len,val)  _snprintf_s((buf),(len),"%I64d", (val));
#	define KC_uint642str(buf,len,val) _snprintf_s((buf),(len),"%I64u", (val));
#	define KC_str2int64(str,val) sscanf_s((str),"%I64d", (val));
#	define KC_str2uint64(str,val) sscanf_s((str),"%I64u", (val));
#	define KC_str2double(str,val) sscanf_s((str),"%Lf", (val));
#else
#	ifdef __x86_64__
#		define KC_INT64_TO_STR(str,i64val)  do{ \
			resetStringBuffer((str));	\
			appendStringBuffer((str), "%ld", (i64val));\
		}while(0)

#		define KC_UINT64_TO_STR(str,i64val)  do{ \
			resetStringBuffer((str));	\
			appendStringBuffer((str), "%lu", (i64val));\
		}while(0)

#		define KC_int642str(buf,len,val) snprintf((buf),(len),"%ld", (val));
#		define KC_uint642str(buf,len,val) snprintf((buf),(len),"%lu", (val));
#		define KC_str2int64(str,val) sscanf((str), "%ld", (val));
#		define KC_str2uint64(str,val) sscanf((str), "%lu", (val));
#	elif __i386__
#		define KC_INT64_TO_STR(str,i64val)  do{ \
			resetStringBuffer((str));	\
			appendStringBuffer((str), "%lld", (i64val));\
		}while(0)

#		define KC_UINT64_TO_STR(str,i64val)  do{ \
			resetStringBuffer((str));	\
			appendStringBuffer((str), "%llu", (i64val));\
		}while(0)


#		define KC_int642str(buf,len,val) snprintf((buf),(len),"%lld", (val));
#		define KC_uint642str(buf,len,val) snprintf((buf),(len),"%llu", (val));
#		define KC_str2int64(str,val) sscanf((str), "%lld", (val));
#		define KC_str2uint64(str,val) sscanf((str), "%llu", (val));
#	endif
#	define KC_INT_TO_STR(str,i64val)  do{ \
		resetStringBuffer((str));	\
		appendStringBuffer((str), "%d", (i64val));\
	}while(0)

#	define KC_UINT_TO_STR(str,i64val)  do{ \
		resetStringBuffer((str));	\
		appendStringBuffer((str), "%u", (i64val));\
	}while(0)

#	define KC_DOUBLE_TO_STR(str,i64val)  do{ \
		resetStringBuffer((str));	\
		appendStringBuffer((str), "%Lf", (i64val));\
	}while(0)
#	define KC_str2double(str,val) sscanf((str),"%Lf", (val));
#endif


/*****************************************************************************
*	文件夹和文件访问权限及创建目录API
#include <io.h>
00——只检查文件是否存在

02——写权限

04——读权限

06——读写权限
mkdir:创建目录
getcwd:取得当前的工作目录
*****************************************************************************/

/* ----------------------------------------------------------------
 *				Section 9: system-specific hacks
 *
 *		This should be limited to things that absolutely have to be
 *		included in every source file.  The port-specific header file
 *		is usually a better place for this sort of thing.
 * ----------------------------------------------------------------
 */

 /*
  *	NOTE:  this is also used for opening text files.
  *	WIN32 treats Control-Z as EOF in files opened in text mode.
  *	Therefore, we open files in binary mode on Win32 so we can read
  *	literal control-Z.  The other affect is that we see CRLF, but
  *	that is OK because we can already handle those cleanly.
  */
#if defined(_MSC_VER) || defined(__CYGWIN__)
#define PG_BINARY	O_BINARY
#define PG_BINARY_A "ab"
#define PG_BINARY_R "rb"
#define PG_BINARY_W "wb"
#else
#define PG_BINARY	0
#define PG_BINARY_A "a"
#define PG_BINARY_R "r"
#define PG_BINARY_W "w"
#endif

#if defined(_MSC_VER)
#define KC_ACCESS(_name,_mode) _access((_name),(_mode))
#define KC_MKDIR(_name,_mode) _mkdir((_name))
#define KC_GETCWD(__buf,__size) _getcwd((__buf),(__size))
#else
#define KC_ACCESS(_name,_mode) access((_name),(_mode))
#define KC_MKDIR(_name,_mode) mkdir((_name),(_mode))
#define KC_GETCWD(__buf,__size) getcwd((__buf),(__size))
#endif

  ///* Check for existence */
  //if ((_access("ACCESS.C", 0)) != -1) {
  //	printf("File ACCESS.C exists ");
  //	/* Check for write permission */
  //	if ((_access("ACCESS.C", 2)) != -1)
  //		printf("File ACCESS.C has write permission ");
  //}

#endif	/*KC_043055AA_F295_42FC_BE3A_D4B24B4AAE54*/
