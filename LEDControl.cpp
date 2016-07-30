#include "LEDControl.h"



LEDControl_::LEDControl_(void) {
    led_off.r = 0;
    led_off.g = 0;
    led_off.b = 0;
    led_steady.r = 0;
    led_steady.g = 255;
    led_steady.b = 0;
    led_blue.r = 0;
    led_blue.g = 0;
    led_blue.b = 255;
    led_dark_blue.r = 0;
    led_dark_blue.g = 0;
    led_dark_blue.b = 127;
    led_red.r = 255;
    led_red.g = 0;
    led_red.b = 0;
    led_dark_red.r = 127;
    led_dark_red.g = 0;
    led_dark_red.b = 0;

}


void LEDControl_::initialize_led_mode(uint8_t mode) {
    set_all_leds_to(led_off);
    if (mode == LED_MODE_OFF) {
        //        set_all_leds_to(led_off);
    } else if (mode == LED_MODE_HEATMAP) {
    } else if (mode == LED_MODE_BREATHE) {

    } else if (mode == LED_MODE_RAINBOW) {
    } else if (mode == LED_MODE_RAINBOW_WAVE) {
    } else if (mode == LED_MODE_CHASE) {
    } else if (mode == LED_MODE_STEADY) {
        set_all_leds_to(led_steady);
    }
}

void LEDControl_::set_all_leds_to(cRGB color) {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        led_set_crgb_at(i, color);
    }
}


void LEDControl_::next_mode() {
    if (led_mode++ >= LED_MODES) {
        led_mode = 0;
    }
}

void LEDControl_::set_mode(uint8_t mode) {
    led_mode = mode;
}




void LEDControl_::update(uint8_t current_keymap) {
    if (current_keymap == NUMPAD_KEYMAP) {
        if (led_mode != LED_SPECIAL_MODE_NUMLOCK) {
            stored_led_mode = led_mode;
        }
        led_mode = LED_SPECIAL_MODE_NUMLOCK;
    }
    if (current_keymap != NUMPAD_KEYMAP &&
            led_mode == LED_SPECIAL_MODE_NUMLOCK
       ) {
        led_mode = stored_led_mode;
    }


    if (led_mode != last_led_mode) {
        initialize_led_mode(led_mode);
    }
    if (led_mode == LED_MODE_OFF) {
    } else if (led_mode == LED_MODE_HEATMAP) {
    } else if (led_mode == LED_MODE_BREATHE) {
        effect_breathe_update();
    } else if (led_mode == LED_MODE_RAINBOW) {
        effect_rainbow_update();
    } else if (led_mode == LED_MODE_RAINBOW_WAVE) {
        effect_rainbow_wave_update();
    } else if (led_mode == LED_MODE_CHASE) {
        effect_chase_update();
    } else if (led_mode == LED_MODE_STEADY) {
        effect_steady_update();
    } else if (led_mode == LED_SPECIAL_MODE_NUMLOCK) {
        effect_numlock_update();

    }

    led_sync();
    last_led_mode = led_mode;
}



void LEDControl_::effect_numlock_update() {
    for (uint8_t i = 0; i < 44; i++) {
        led_set_crgb_at(i, led_off);
    }
    for (uint8_t i = 44; i < LED_COUNT; i++) {
        led_set_crgb_at(i, led_red);
    }
    led_compute_breath();
    led_set_crgb_at(60, led_breathe); // make numlock breathe
}

void LEDControl_::effect_steady_update() {
}

void LEDControl_::led_compute_breath() {
    // algorithm from http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
    breathe_brightness =  (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
    // change the brightness for next time through the loop:
    //breathe_brightness = breathe_brightness + breathe_fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (breathe_brightness == 0 || breathe_brightness == 150) {
        breathe_fadeAmount = -breathe_fadeAmount ;
    }


    hsv_to_rgb(&led_breathe,200, 255, breathe_brightness);
}

void LEDControl_::effect_breathe_update() {
    led_compute_breath();
    set_all_leds_to(led_breathe);
}

void LEDControl_::effect_chase_update() {
    if (current_chase_counter++ < chase_threshold) {
        return;
    }
    current_chase_counter = 0;
    led_set_crgb_at(pos - (chase_sign* chase_pixels), led_off);
    led_set_crgb_at(pos, led_off);

    pos += chase_sign;
    if (pos >= LED_COUNT || pos <= 0) {
        chase_sign = -chase_sign;
    }
    led_set_crgb_at(pos, led_blue);
    led_set_crgb_at(pos - (chase_sign * chase_pixels), led_red);
}

void LEDControl_::effect_rainbow_update() {
    if (rainbow_current_ticks++ < rainbow_ticks) {
        return;
    } else {
        rainbow_current_ticks = 0;
    }
    hsv_to_rgb( &rainbow, rainbow_hue, rainbow_saturation, rainbow_value);
    rainbow_hue += rainbow_steps;
    if (rainbow_hue >= 255)          {
        rainbow_hue %= 255;
    }
    set_all_leds_to(rainbow);
}

void LEDControl_::effect_rainbow_wave_update() {
    if (rainbow_current_ticks++ < rainbow_wave_ticks) {
        return;
    } else {
        rainbow_current_ticks = 0;
    }

    for (uint8_t i = 0; i < LED_COUNT; i++) {
        uint16_t key_hue = rainbow_hue +16*(i/4);
        if (key_hue >= 255)          {
            key_hue %= 255;
        }
        hsv_to_rgb(&rainbow, key_hue, rainbow_saturation, rainbow_value);
        led_set_crgb_at(i,rainbow);
    }
    rainbow_hue += rainbow_wave_steps;
    if (rainbow_hue >= 255)          {
        rainbow_hue %= 255;
    }
}

void LEDControl_::boot_animation() {
    set_all_leds_to(led_off);

    type_letter(LED_K);
    type_letter(LED_E);
    type_letter(LED_Y);
    type_letter(LED_B);
    type_letter(LED_O);
    type_letter(LED_A);
    type_letter(LED_R);
    type_letter(LED_D);
    type_letter(LED_I);
    type_letter(LED_O);
    type_letter(LED_SPACE);
    type_letter(LED_0);
    type_letter(LED_PERIOD);
    type_letter(LED_9);
    led_mode = LED_MODE_RAINBOW;


}

void LEDControl_::type_letter(uint8_t letter) {
    led_set_crgb_at(letter,led_red);
    led_sync();
    delay(250);
    led_set_crgb_at(letter,led_off);
    led_sync();
    delay(10);

}


// From http://web.mit.edu/storborg/Public/hsvtorgb.c - talk to Scott about licensing
void LEDControl_::hsv_to_rgb(cRGB *cRGB, uint16_t h, uint16_t s, uint16_t v)  {
    /* HSV to RGB conversion function with only integer
     * math */
    uint16_t region, fpart, p, q, t;

    if(s == 0) {
        /* color is grayscale */
        cRGB->r = cRGB->g = cRGB->b = v;
        return;
    }

    /* make hue 0-5 */
    region = h / 43;
    /* find remainder part, make it from 0-255 */
    fpart = (h - (region * 43)) * 6;

    /* calculate temp vars, doing integer multiplication */
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * fpart) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;

    /* assign temp vars based on color cone region */
    switch(region) {
    case 0:
        cRGB->r = v;
        cRGB->g = t;
        cRGB->b = p;
        break;
    case 1:
        cRGB->r = q;
        cRGB->g = v;
        cRGB->b = p;
        break;
    case 2:
        cRGB->r = p;
        cRGB->g = v;
        cRGB->b = t;
        break;
    case 3:
        cRGB->r = p;
        cRGB->g = q;
        cRGB->b = v;
        break;
    case 4:
        cRGB->r = t;
        cRGB->g = p;
        cRGB->b = v;
        break;
    default:
        cRGB->r = v;
        cRGB->g = p;
        cRGB->b = q;
        break;
    }

    return;
}



LEDControl_ LEDControl;
