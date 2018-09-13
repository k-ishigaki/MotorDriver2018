#ifndef LOG_H
#define LOG_H

#include <stdio.h>

// convinient log macros
#define log_v(...) application::log::v(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_i(...) application::log::i(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define log_e(...) application::log::e(__FILE__, __LINE__, __func__, __VA_ARGS__)

namespace application {
    namespace log {
        enum class Level {
            Vervose,
            Info,
            Error,
        };

        namespace impl {
            // do not use from other modules
            bool needsOutput(Level);
            template<typename ... Args> void func(
                    Level level,
                    const char tagChar,
                    const char* file,
                    const int line,
                    const char* func,
                    const char* format,
                    Args const& ... args) {
                if (needsOutput(level) == false) {
                    return;
                }
                printf("[%c]%s(%d):%s ", tagChar, file, line, func);
                printf(format, args ...);
                putchar('\n');
                putchar('\r');
            }
        }

        /**
         * Configure log module.
         *
         * @param function pointer implements putchar
         * @param log output level
         */
        void configure(void (*)(char), Level);


        /**
         * Put vervose log.
         *
         * @param file name
         * @param line number
         * @param function name
         * @param formated string
         * @param parameters which belongs to string
         */
        template<typename ... Args> void v(
                const char* file,
                const int line,
                const char* func,
                const char* format,
                Args const& ... args) {
            impl::func(Level::Vervose, 'V', file, line, func, format, args ...);
        }

        /**
         * Put info log.
         *
         * @param file name
         * @param line number
         * @param function name
         * @param formated string
         * @param parameters which belongs to string
         */
        template<typename ... Args> void i(
                const char* file,
                const int line,
                const char* func,
                const char* format,
                Args const& ... args) {
            impl::func(Level::Info, 'I', file, line, func, format, args ...);
        }

        /**
         * Put error log.
         *
         * @param file name
         * @param line number
         * @param function name
         * @param formated string
         * @param parameters which belongs to string
         */
        template<typename ... Args> void e(
                const char* file,
                const int line,
                const char* func,
                const char* format,
                Args const& ... args) {
            impl::func(Level::Error, 'E', file, line, func, format, args ...);
        }
    }
}

#endif
