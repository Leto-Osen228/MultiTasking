/**************************
* Watchdog operating mode *
**************************/
#define RESET_MODE        0x08  // To reset the CPU if there is a timeout
#define INTERRUPT_MODE      0x40  // Timeout will cause an interrupt
#define INTERRUPT_RESET_MODE  0x48  // First time-out interrupt , the second time out - reset


/**********************************
* Watchdog time-out vs prescaler: *
**********************************/
#define WDT_PRESCALER_2   0x00  // (16 ± 1.6) ms
#define WDT_PRESCALER_4   0x01  // (32 ± 3.2) ms
#define WDT_PRESCALER_8   0x02  // (64 ± 6.4) ms
#define WDT_PRESCALER_16  0x03  // (128 ± 12.8) ms
#define WDT_PRESCALER_32  0x04  // (256 ± 25.6) ms
#define WDT_PRESCALER_64  0x05  // (512 ± 51.2) ms
#define WDT_PRESCALER_128 0x06  // (1024 ± 102.4) ms
#define WDT_PRESCALER_256 0x07  // (2048 ± 204.8) ms
#define WDT_PRESCALER_512 0x08  // (4096 ± 409.6) ms
#define WDT_PRESCALER_1024  0x09  // (8192 ± 819.2) ms


void (*isr_wdt)();

/* непосредственно прерывание ватчдога */
ISR(WDT_vect) {
  (*isr_wdt)();
}

/* сброс счетчика watchdog в 0 */
void wdt_reset(void) {
  asm volatile("wdr"); // ассемблерная вставка "watchdog reset"
}

/* Полное отключение watchdog'a */
void wdt_disable(void) {
  WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешение на вмешательство
  WDTCSR = 0; // обнуление регистра wdt
}

/* Вариант функции для работы ватчдога с прерываниями при таймауте */
void wdt_enable(uint8_t  mode , uint8_t prescaler , void (*isr)()) {
  isr_wdt = *isr;  //указатель на функцию
  uint8_t wdtReg;
  if (mode) wdtReg = (1 << WDIE) | (1 << WDE);
  else wdtReg = (1 << WDIE);
  if (prescaler > 7) wdtReg |= (1 << WDP3)|(prescaler - 8);
  else wdtReg |= prescaler;
  cli();
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = wdtReg;
  sei();
}

/* вариант для работы без прерываний (только сброс) */
void wdt_enable(uint8_t prescaler) {
  uint8_t wdtReg = (1 << WDE);
  if (prescaler > 7) wdtReg |= (1 << WDP3)|(prescaler - 8);
  else wdtReg |= prescaler;
  cli();
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = wdtReg;
  sei();
}
