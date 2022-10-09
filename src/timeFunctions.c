#include <fxcg/rtc.h>

int millis() {
    // The RTC clock runs at 128Hz
    return RTC_GetTicks() * 1000 / 128;
}