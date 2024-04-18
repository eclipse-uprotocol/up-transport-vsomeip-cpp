#ifndef STREAMER_LOGGER_H_
#define STREAMER_LOGGER_H_
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <csignal>

void handleLogLevelSignal(int signal, siginfo_t *sip, void *ptr);
void setupLogger();

//* Sys Logging */
#define LogTrace(...)        SPDLOG_INFO(__VA_ARGS__)
#define LogInfo(...)         SPDLOG_INFO(__VA_ARGS__)
#define LogWarn(...)         SPDLOG_WARN(__VA_ARGS__)
#define LogDebug(...)        SPDLOG_INFO(__VA_ARGS__)
#define LogErr(...)          SPDLOG_ERROR(__VA_ARGS__)
#define LogCrit(...)         SPDLOG_CRITICAL(__VA_ARGS__)

#endif /* STREAMER_LOGGER_H_ */
