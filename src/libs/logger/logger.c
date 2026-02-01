#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 1

#include <errno.h>
#include <logger.h>
#include <nanoprintf.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static size_t _output_count = 0;
static npf_putc _outputs[LOG_OUTPUT_MAX];

int logger_add_output(npf_putc output) {
    if (_output_count >= LOG_OUTPUT_MAX) {
        return -ERANGE;
    }

    _outputs[_output_count++] = output;
    return 0;
}

void _log(LogEvent event, char const *filename, size_t line, char const *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[1024] = {0};
    npf_vsnprintf(buffer, 1024, format, args);

    for (size_t i = 0; i < _output_count; i++) {
        npf_putc output = _outputs[i];
        if (event != LOG_NONE) {
            npf_pprintf(output, NULL, "%s%s\e[0m %s:%ld ", level_colors[event], level_names[event], filename, line);
        }

        for (char *c = buffer; *c != '\0'; c++) {
            output(*c, NULL);
        }

        if (event != LOG_NONE) {
            output('\n', NULL);
        }
    }

    va_end(args);

    if (event == LOG_PANIC) {
        abort();
    }
}
