#include <RSLED.h>

bool R;
bool G;
bool B;

bool _led_r;
bool _led_g;
bool _led_b;

void initLED() {
    pinMode(RSLED_R, OUTPUT);
    pinMode(RSLED_G, OUTPUT);
    pinMode(RSLED_B, OUTPUT);
}

void updateLED() {
    if(R != _led_r) {
        _led_r = R;
        digitalWrite(RSLED_R, _led_r);
    }
    if(G != _led_g) {
        _led_g = G;
        digitalWrite(RSLED_G, _led_g);
    }
    if(B != _led_b) {
        _led_b = B;
        digitalWrite(RSLED_B, _led_b);
    }
}

void ledcmd(String cmd) {
    if(cmd.startsWith("LED")) {
        cmd = cmd.substring(3);
    }

    for(uint8_t i = 0; i < 3 && i < cmd.length(); i++) {
        char c = cmd.charAt(i);
        if(c == 'R') {
            R = true;
        }else if(c == 'r') {
            R = false;
        }else if(c == 'G') {
            G = true;
        }else if(c == 'g') {
            G = false;
        }else if(c == 'B') {
            B = true;
        }else if(c == 'b') {
            B = false;
        }
    }
}

