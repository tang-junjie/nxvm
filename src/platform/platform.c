/* Copyright 2012-2014 Neko. */

/* PLATFORM is the hub that connects all platform-specific modules */

#include "../utils.h"

#include "platform.h"

t_platform platform;

#if GLOBAL_PLATFORM == GLOBAL_VAR_WIN32
#include "win32/win32.h"
void platformSleep(uint32_t milisec) {
    win32Sleep(milisec);
}
void platformDisplaySetScreen() {
    win32DisplaySetScreen(platform.flagMode);
}
void platformDisplayPaint() {
    win32DisplayPaint(platform.flagMode);
}
void platformStart() {
    win32StartMachine(platform.flagMode);
}
#elif GLOBAL_PLATFORM == GLOBAL_VAR_LINUX
#include "linux/linux.h"
void platformSleep(uint32_t milisec) {
    linuxSleep(milisec);
}
void platformDisplaySetScreen() {
    linuxDisplaySetScreen(platform.flagMode);
}
void platformDisplayPaint() {
    linuxDisplayPaint(platform.flagMode);
}
void platformStart() {
    linuxStartMachine(platform.flagMode);
}
#endif

void platformInit() {
    MEMSET((void *)(&platform), 0x00, sizeof(t_platform));
    platform.flagMode = 0;
}

void platformFinal() {}
