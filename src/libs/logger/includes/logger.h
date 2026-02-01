#pragma once

#include <nanoprintf.h>
#include <stddef.h>

#define LOG_OUTPUT_MAX (8)

#define log$(...) _log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define warn$(...) _log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define error$(...) _log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define panic$(...)                                       \
    {                                                     \
        _log(LOG_PANIC, __FILE__, __LINE__, __VA_ARGS__); \
        for (;;)                                          \
            __asm__ volatile("wfi");                      \
    }
#define print$(...) _log(LOG_NONE, "", 0, __VA_ARGS__)

typedef enum {
    LOG_NONE,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_PANIC,

    LOG_EVENT_LENGTH
} LogEvent;

static char const *level_names[LOG_EVENT_LENGTH] = {
    [LOG_NONE] = "",
    [LOG_INFO] = "INFO",
    [LOG_WARN] = "WARN",
    [LOG_ERROR] = "ERROR",
    [LOG_PANIC] = "PANIC",
};

static char const *level_colors[LOG_EVENT_LENGTH] = {
    [LOG_NONE] = "",
    [LOG_INFO] = "\e[1;34m",
    [LOG_WARN] = "\e[1;33m",
    [LOG_ERROR] = "\e[1;31m",
    [LOG_PANIC] = "\e[1;31m",
};

int logger_add_output(npf_putc output);

void _log(LogEvent level, char const *filename, size_t line, char const *fmt, ...);
