#pragma once

#define RTC_INTERRUPT_IRQ_NUMBER 8
extern "C" void isrRtcHandler();

void initRtcInterrupt();