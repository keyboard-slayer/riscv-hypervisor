#pragma once

#include <stddef.h>
#include <stdint.h>

#define MAX_ERRNO 4095
#define IS_ERR_VALUE(x) ((uintptr_t)(x) >= (uintptr_t)-MAX_ERRNO)

[[gnu::warn_unused_result]] static inline void *ERR_PTR(intptr_t error) {
    return (void *)error;
}

[[gnu::warn_unused_result]] static inline long PTR_ERR(void const *ptr) {
    return (long)ptr;
}

[[gnu::warn_unused_result]] static inline bool IS_ERR(void const *ptr) {
    return IS_ERR_VALUE((uintptr_t)ptr);
}

[[gnu::warn_unused_result]] static inline bool IS_ERR_OR_NULL(void const *ptr) {
    return ptr == NULL || IS_ERR_VALUE((unsigned long)ptr);
}

[[gnu::warn_unused_result]] static inline void *ERR_CAST(void const *ptr) {
    return (void *)ptr;
}

#define E2BIG 1            // Argument list too long
#define EACCES 2           // Permission denied
#define EADDRINUSE 3       // Address already in use
#define EADDRNOTAVAIL 4    // Cannot assign requested address
#define EAFNOSUPPORT 5     // Address family not supported by protocol
#define EAGAIN 6           // Try again
#define EALREADY 7         // Operation already in progress
#define EBADF 8            // Bad file number
#define EBADMSG 9          // Not a data message
#define EBUSY 10           // Device or resource busy
#define ECANCELED 11       // Operation canceled
#define ECHILD 12          // No child processes
#define ECONNABORTED 13    // Software caused connection abort
#define ECONNREFUSED 14    // Connection refused
#define ECONNRESET 15      // Connection reset by peer
#define EDEADLK 16         // Resource deadlock would occur
#define EDESTADDRREQ 17    // Destination address required
#define EDOM 18            // Math argument out of domain of func
#define EDQUOT 19          // Reserved
#define EEXIST 20          // File exists
#define EFAULT 21          // Bad address
#define EFBIG 22           // File too large
#define EHOSTUNREACH 23    // No route to host
#define EIDRM 24           // Identifier removed
#define EILSEQ 25          // Illegal byte sequence
#define EINPROGRESS 26     // Operation now in progress
#define EINTR 27           // Interrupted function
#define EINVAL 28          // Invalid argument
#define EIO 29             // I/O error
#define EISCONN 30         // Socket is already connected
#define EISDIR 31          // Is a directory
#define ELOOP 32           // Too many symbolic links encountered
#define EMFILE 33          // Too many open file
#define EMLINK 34          // Too many links
#define EMSGSIZE 35        // Message too long
#define EMULTIHOP 36       // Reserved
#define ENAMETOOLONG 37    // File name too long
#define ENETDOWN 38        // Network is down
#define ENETRESET 39       // Network dropped connection because of reset
#define ENETUNREACH 40     // Network is unreachable
#define ENFILE 41          // File table overflow
#define ENOBUFS 42         // No buffer space available
#define ENODATA 43         // No data available
#define ENODEV 44          // No such device
#define ENOENT 45          // No such file or directory
#define ENOEXEC 46         // Exec format error
#define ENOLCK 47          // No record locks available
#define ENOLINK 48         // Reserved
#define ENOMEM 49          // Not enough space
#define ENOMSG 50          // No message of desired type
#define ENOPROTOOPT 51     // Protocol not available
#define ENOSPC 52          // No space left on device
#define ENOSR 53           // No STREAM resources
#define ENOSTR 54          // Not a STREAM
#define ENOSYS 55          // Function not implemented
#define ENOTCONN 56        // The socket is not connected
#define ENOTDIR 57         // Not a directory
#define ENOTEMPTY 58       // Directory not empty
#define ENOTRECOVERABLE 59 // State not recoverable
#define ENOTSOCK 60        // Not a socket
#define ENOTSUP 61         // Not supported
#define ENOTTY 62          // Inappropriate I/O control operation
#define ENXIO 63           // No such device or address
#define EOPNOTSUPP 64      // Operation not supported on transport endpoint
#define EOVERFLOW 65       // Value too large to be stored in data type
#define EOWNERDEAD 66      // Previous owner died
#define EPERM 67           // Operation not permitted
#define EPIPE 68           // Broken pipe
#define EPROTO 69          // Protocol error
#define EPROTONOSUPPORT 70 // Protocol not supported
#define EPROTOTYPE 71      // Protocol wrong type for socket
#define ERANGE 72          // Result too large
#define EROFS 73           // Read-only file system
#define ESPIPE 74          // Invalid seek
#define ESRCH 75           // No such process
#define EDTALE 76          // Reserved
#define ETIME 77           // Stream ioctl timeout
#define ETIMEDOUT 78       // Connection timed out
#define ETXTBSY 79         // Text file busy
#define EWOULDBLOCK EAGAIN // Operation would block
#define EXDEV 80           // Cross-device link

static inline const char *strerror(int errnum) {
    if (errnum < 0) {
        errnum = -errnum;
    }

    switch (errnum) {
    case E2BIG:
        return "Argument list too long";
    case EACCES:
        return "Permission denied";
    case EADDRINUSE:
        return "Address already in use";
    case EADDRNOTAVAIL:
        return "Cannot assign requested address";
    case EAFNOSUPPORT:
        return "Address family not supported by protocol";
    case EAGAIN:
        return "Try again";
    case EALREADY:
        return "Operation already in progress";
    case EBADF:
        return "Bad file number";
    case EBADMSG:
        return "Not a data message";
    case EBUSY:
        return "Device or resource busy";
    case ECANCELED:
        return "Operation canceled";
    case ECHILD:
        return "No child processes";
    case ECONNABORTED:
        return "Software caused connection abort";
    case ECONNREFUSED:
        return "Connection refused";
    case ECONNRESET:
        return "Connection reset by peer";
    case EDEADLK:
        return "Resource deadlock would occur";
    case EDESTADDRREQ:
        return "Destination address required";
    case EDOM:
        return "Math argument out of domain of func";
    case EDQUOT:
        return "Reserved";
    case EEXIST:
        return "File exists";
    case EFAULT:
        return "Bad address";
    case EFBIG:
        return "File too large";
    case EHOSTUNREACH:
        return "No route to host";
    case EIDRM:
        return "Identifier removed";
    case EILSEQ:
        return "Illegal byte sequence";
    case EINPROGRESS:
        return "Operation now in progress";
    case EINTR:
        return "Interrupted function";
    case EINVAL:
        return "Invalid argument";
    case EIO:
        return "I/O error";
    case EISCONN:
        return "Socket is already connected";
    case EISDIR:
        return "Is a directory";
    case ELOOP:
        return "Too many symbolic links encountered";
    case EMFILE:
        return "Too many open file";
    case EMLINK:
        return "Too many links";
    case EMSGSIZE:
        return "Message too long";
    case EMULTIHOP:
        return "Reserved";
    case ENAMETOOLONG:
        return "File name too long";
    case ENETDOWN:
        return "Network is down";
    case ENETRESET:
        return "Network dropped connection because of reset";
    case ENETUNREACH:
        return "Network is unreachable";
    case ENFILE:
        return "File table overflow";
    case ENOBUFS:
        return "No buffer space available";
    case ENODATA:
        return "No data available";
    case ENODEV:
        return "No such device";
    case ENOENT:
        return "No such file or directory";
    case ENOEXEC:
        return "Exec format error";
    case ENOLCK:
        return "No record locks available";
    case ENOLINK:
        return "Reserved";
    case ENOMEM:
        return "Not enough space";
    case ENOMSG:
        return "No message of desired type";
    case ENOPROTOOPT:
        return "Protocol not available";
    case ENOSPC:
        return "No space left on device";
    case ENOSR:
        return "No STREAM resources";
    case ENOSTR:
        return "Not a STREAM";
    case ENOSYS:
        return "Function not implemented";
    case ENOTCONN:
        return "The socket is not connected";
    case ENOTDIR:
        return "Not a directory";
    case ENOTEMPTY:
        return "Directory not empty";
    case ENOTRECOVERABLE:
        return "State not recoverable";
    case ENOTSOCK:
        return "Not a socket";
    case ENOTSUP:
        return "Not supported";
    case ENOTTY:
        return "Inappropriate I/O control operation";
    case ENXIO:
        return "No such device or address";
    case EOPNOTSUPP:
        return "Operation not supported on transport endpoint";
    case EOVERFLOW:
        return "Value too large to be stored in data type";
    case EOWNERDEAD:
        return "Previous owner died";
    case EPERM:
        return "Operation not permitted";
    case EPIPE:
        return "Broken pipe";
    case EPROTO:
        return "Protocol error";
    case EPROTONOSUPPORT:
        return "Protocol not supported";
    case EPROTOTYPE:
        return "Protocol wrong type for socket";
    case ERANGE:
        return "Result too large";
    case EROFS:
        return "Read-only file system";
    case ESPIPE:
        return "Invalid seek";
    case ESRCH:
        return "No such process";
    case EDTALE:
        return "Reserved";
    case ETIME:
        return "Stream ioctl timeout";
    case ETIMEDOUT:
        return "Connection timed out";
    case ETXTBSY:
        return "Text file busy";
    case EXDEV:
        return "Cross-device link";
    default:
        return "Unknown error";
    }
}
