/**
 * @file bootstrap.h
 * @brief Explicit hand-off from CubeMX startup to the application runtime.
 */

#ifndef BD_BASE_BOOTSTRAP_H
#define BD_BASE_BOOTSTRAP_H

#ifdef __cplusplus
extern "C" {
#define APP_NORETURN [[noreturn]]
#else
#define APP_NORETURN _Noreturn
#endif

APP_NORETURN void app_bootstrap(void);
APP_NORETURN void app_fatal(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#undef APP_NORETURN

#endif
