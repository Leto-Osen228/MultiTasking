class LED {
  public:
    LED(uint8_t pin, bool invert = false) {
      _pin = pin;
      _invert = invert;
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, invert);
    }
    void set(bool val) {
      digitalWrite(_pin, val ^ _invert);
      state = val;
    }
    void toggle() {
      state = !state;
      digitalWrite(_pin, state ^ _invert);
    }
    String str_toggle() {
      toggle();
      return String(state);
    }
    String str_getState() {
      return String(state);
    }
    bool state;
  private:
    uint8_t _pin;
    uint8_t _invert;
};
