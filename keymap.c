#include <print.h>
#include "ergodox_ez.h"
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "keymap_german.h"
#include "keymap_nordic.h"
#include "keymap_french.h"
#include "keymap_spanish.h"
#include "keymap_hungarian.h"

/* Hack to include files since compilation only does this file */
#include "vim.c"

#define KC_MAC_UNDO LGUI(KC_Z)
#define KC_MAC_CUT LGUI(KC_X)
#define KC_MAC_COPY LGUI(KC_C)
#define KC_MAC_PASTE LGUI(KC_V)
#define KC_PC_UNDO LCTL(KC_Z)
#define KC_PC_CUT LCTL(KC_X)
#define KC_PC_COPY LCTL(KC_C)
#define KC_PC_PASTE LCTL(KC_V)

/* Timers for super alt-tab, adopted from qmk docs */
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
#define ALT_TAB_LENGTH 600

/* Keycodes */
enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE, // can always be here
    EPRM,
    RGB_SLD,
    
    /* Have one button press that cycles through all the layers */
    CYCLE_OS_LAYERS,

    /* Super alt tab for one button alt-tab */
    SUPER_ALT_TAB,
    
    /* Custom Vim Keycodes -- not using currently */
    VIM_ESCAPE_HATCH,
    VIM_TOGGLE,
    VIM_WINDOW_MOVE_LEFT,
    VIM_WINDOW_MOVE_RIGHT,
    VIM_WINDOW_MOVE_UP,
    VIM_WINDOW_MOVE_DOWN,
    VIM_WINDOW_CREATE,
};

/*
 * Various notes about the ergodox implementation:
 * - There is a lot of info that is needed to be displayed with just 3 LEDs, until I have
 *   a better soln (or better keyboard), the solution is as follows:
 *   
 *   LED COLOR         ON               OFF
 *   RED               QWERTY           DVORAK
 *   GREEN             MAC              LINUX
 *   BLUE              GOLAND/VIM ON    NO APP LAYER
 */

/* Layer names easily defined */
#define __DVORAK 0
#define __DVORAK_SHIFT 1
#define __QWERTY 2

/* OS Specific Layers + the default*/
#define __LINUX 3
#define __MAC 4
#define __DEFAULT_OS __MAC
#define OS_XOR 0x18 /* bits for 3 & 4 set */ 
    
/* Application Specific Layers */
#define __VIM 5
#define __GOLAND 6

/* Function keys */
#define __FN 7

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Base layout */
    [__DVORAK] = LAYOUT_ergodox(\
            /* Left Hand */
            KC_DLR,             KC_AMPR,  KC_LBRACKET,     KC_LCBR, KC_RCBR,KC_LPRN, KC_EQUAL,\
            KC_TAB,             KC_SCOLON,KC_COMMA,KC_DOT, KC_P,    KC_Y,      KC_BSLASH,     \
            KC_ESCAPE,          KC_A,     KC_O,    KC_E,   KC_U,    KC_I,                     \
            MO(__DVORAK_SHIFT), KC_QUOTE, KC_Q,    KC_J,   KC_K,    KC_X,      KC_LGUI,       \
            KC_LCTRL,           OSL(__FN),KC_LALT, KC_LEFT,KC_RIGHT,
            /* Left Thumb */
            TG(__VIM),             TG(__GOLAND),
                                   _______,
            KC_SPACE, LCTL(KC_B),  SUPER_ALT_TAB, \
            
            /* Right Hand */
            KC_HASH,        KC_ASTR,KC_RPRN,KC_PLUS,KC_RBRACKET,KC_EXLM, KC_BSPACE, \
            KC_AT,          KC_F,   KC_G,   KC_C,   KC_R,       KC_L,    KC_SLASH,  \
                            KC_D,   KC_H,   KC_T,   KC_N,       KC_S,    KC_MINUS,  \
            LGUI(KC_GRAVE), KC_B,   KC_M,   KC_W,   KC_V,       KC_Z,    KC_RSHIFT, \
                                    KC_UP,  KC_DOWN,_______,    _______, _______,   \
            /* Right Thumb */
            _______,  _______,
            DF(__QWERTY),
            CYCLE_OS_LAYERS, KC_TAB, KC_ENTER \
            ),

    /* Base layout when shift is pressed */
    [__DVORAK_SHIFT] = LAYOUT_ergodox(\
            /* Left Hand */ 
            KC_TILD,KC_7,KC_5,KC_3,KC_1,KC_9,KC_PERC,
            LSFT(KC_TAB),KC_COLN,KC_LABK,KC_RABK,LSFT(KC_P),LSFT(KC_Y),LSFT(KC_BSLASH),
            _______,LSFT(KC_A),LSFT(KC_O),LSFT(KC_E),LSFT(KC_U),LSFT(KC_I),
            _______,KC_DQUO,LSFT(KC_Q),LSFT(KC_J),LSFT(KC_K),LSFT(KC_X),_______,
            _______,_______,_______,_______,_______,
            /* Left Thumb */
            _______,_______,_______,_______,_______,_______,
            
            /* Right Hand */
            KC_GRAVE, KC_0,KC_2,KC_4,KC_6,KC_8,_______,
            KC_CIRC,LSFT(KC_F),LSFT(KC_G),LSFT(KC_C),LSFT(KC_R),LSFT(KC_L),KC_QUES,
            LSFT(KC_D),LSFT(KC_H),LSFT(KC_T),LSFT(KC_N),LSFT(KC_S),KC_UNDS,
            _______,LSFT(KC_B),LSFT(KC_M),LSFT(KC_W),LSFT(KC_V),LSFT(KC_Z),_______,
            _______,_______,_______,_______,_______,
            /* Right Thumb */
            _______,_______,_______,_______,LSFT(KC_TAB),LSFT(KC_ENTER)),
    
    /* QWERTY Mode */
    [__QWERTY] = LAYOUT_ergodox(\
            /* Left Hand */
            KC_EQUAL,KC_1,KC_2,KC_3,KC_4,KC_5,KC_DELETE,
            KC_TAB, KC_Q,KC_W,KC_E,KC_R,KC_T,_______,
            KC_ESCAPE,KC_A,KC_S,KC_D,KC_F,KC_G,
            KC_LSHIFT,LCTL_T(KC_Z),KC_X,KC_C,KC_V,KC_B,KC_LGUI,
            KC_LCTRL,KC_QUOTE,KC_LALT,KC_LEFT,KC_RIGHT,
            /* Left Thumb */ 
            LALT_T(KC_APPLICATION),KC_LGUI,KC_F11,KC_SPACE,KC_BSPACE,SUPER_ALT_TAB,
             
            /* Right Hand */
            KC_RIGHT,KC_6,KC_7,KC_8,KC_9,KC_0,KC_BSPACE,
            _______,KC_Y,KC_U,KC_I,KC_O,KC_P,KC_BSLASH,
            KC_H,KC_J,KC_K,KC_L,KC_SCOLON,_______,
            KC_MEH,KC_N,KC_M,KC_COMMA,KC_DOT,RCTL_T(KC_SLASH),KC_RSHIFT,
            KC_UP,KC_DOWN,KC_LBRACKET,KC_RBRACKET,_______,
            /* Right thumb */
            KC_LALT,LCTL_T(KC_ESCAPE),DF(__DVORAK),CYCLE_OS_LAYERS,KC_TAB,KC_ENTER),

 
    /* Linux specific shortcuts */
    [__LINUX] = LAYOUT_ergodox( \
            /* Left Hand */ 
            _______, _______, _______, _______, _______, _______,           _______,  
            _______,       _______, _______, _______, _______, _______,     _______, 
            _______,       _______, _______, _______, _______, _______, 
            _______,       _______, _______, _______, _______, _______,     _______, 
            _______, _______, _______, _______, _______, 
            /* Left Thumb */
            _______,          _______, 
                              _______, 
            _______, _______, _______, 

            /* Right Hand */ 
            _______, _______, _______, _______, _______, _______,         _______,  
            _______,      _______, _______, _______, _______, _______,    _______, 
                          _______, _______, _______, _______, _______,    _______, 
            _______,      _______, _______, _______, _______, _______,    _______, 
            _______, _______, _______, _______, _______, 
            /* Right Thumb */
            _______, _______, 
            _______, 
            _______, _______, _______ \
    ),
    /* Mac specific shortcuts */
    [__MAC] = LAYOUT_ergodox( \
            /* Left Hand */ 
            _______, _______, _______, _______, _______, _______,           _______,  
            _______,       _______, _______, _______, _______, _______,     _______, 
            _______,       _______, _______, _______, _______, _______, 
            _______,       _______, _______, _______, _______, _______,     _______, 
            _______, _______, _______, _______, _______, 
            /* Left Thumb */
            _______,          _______, 
                              _______, 
            _______, _______, _______, 

            /* Right Hand */ 
            _______, _______, _______, _______, _______, _______,         _______,  
            _______,      _______, _______, _______, _______, _______,    _______, 
                          _______, _______, _______, _______, _______,    _______, 
            _______,      _______, _______, _______, _______, _______,    _______, 
            _______, _______, _______, _______, _______, 
            /* Right Thumb */
            _______, _______, 
            _______, 
            _______, _______, _______ \
    ),

    /* Vim specific shortcuts */
    [__VIM] = LAYOUT_ergodox( \
            /* Left Hand */ 
            _______, _______, _______, _______, _______, _______,           _______,  
            _______,       _______, _______, _______, _______, _______,     _______, 
            _______,       _______, _______, _______, _______, _______, 
            _______,       _______, _______, _______, _______, _______,     _______, 
            _______, _______, _______, _______, _______, 
            /* Left Thumb */
            _______,          _______, 
                              _______, 
            _______, _______, _______, 

            /* Right Hand */ 
            _______, _______, _______, _______, _______, _______,         _______,  
            _______,      _______, _______, _______, _______, _______,    _______, 
                          _______, _______, _______, _______, _______,    _______, 
            _______,      _______, _______, _______, _______, _______,    _______, 
            _______, _______, _______, _______, _______, 
            /* Right Thumb */
            _______, _______, 
            _______, 
            _______, _______, _______ \
    ),

    /* Goland specific shortcuts */
    [__GOLAND] = LAYOUT_ergodox( \
            /* Left Hand */ 
            _______, _______, _______, _______, _______, _______,           _______,  
            _______,       _______, _______, _______, _______, _______,     _______, 
            _______,       _______, _______, _______, _______, _______, 
            _______,       _______, _______, _______, _______, _______,     _______, 
            _______, _______, _______, _______, _______, 
            /* Left Thumb */
            _______,          _______, 
                              _______, 
            _______, _______, _______, 

            /* Right Hand */ 
            _______, _______, _______, _______, _______, _______,         _______,  
            _______,      _______, _______, _______, _______, _______,    _______, 
                          _______, _______, _______, _______, _______,    _______, 
            _______,      _______, _______, _______, _______, _______,    _______, 
            _______, _______, LGUI(KC_LBRACKET), LGUI(KC_RBRACKET), _______, 
            /* Right Thumb */
            LAG(KC_B), LGUI(KC_B), 
            _______, 
            _______, LSG(KC_E), _______ \
    ),
    [__FN] = LAYOUT_ergodox( \
            /* Left Hand */ 
            _______, KC_F7, KC_F5, KC_F3, KC_F1, KC_F9,                     _______,  
            _______,       _______, _______, _______, _______, _______,     _______, 
            _______,       _______, _______, _______, _______, _______, 
            _______,       _______, _______, _______, _______, _______,     _______, 
            _______, _______, _______, _______, _______, 
            /* Left Thumb */
            _______,          _______, 
                              _______, 
            _______, _______, _______, 

            /* Right Hand */ 
            _______,      _______, KC_F2, KC_F4, KC_F6, KC_F8,            _______,  
            _______,      _______, _______, _______, _______, _______,    _______, 
                          _______, _______, _______, _______, _______,    _______, 
            _______,      _______, _______, _______, _______, _______,    _______, 
                                   _______, _______, LGUI(KC_LBRACKET), LGUI(KC_RBRACKET), _______, 
            /* Right Thumb */
            _______, _______, 
            _______, 
            _______, LSG(KC_E), _______ \
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // dynamically generate these.
        case EPRM:
            if (record->event.pressed) {
                eeconfig_init();
            }
            return false;
        
        case CYCLE_OS_LAYERS:
            if (record->event.pressed) {
                layer_xor(OS_XOR);
            }
            return false;
        
        /* Alt-Tab Features */
        case SUPER_ALT_TAB:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;

                    if (IS_LAYER_ON(__MAC)) 
                        register_code(KC_LGUI);
                    else
                        register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            return true;
    }
    return true;
}

/* On initialization of the keyboard, set the default OS layer */
void matrix_init_user(void) {
    layer_on(__DEFAULT_OS);
}

uint32_t layer_state_set_user(uint32_t state) {

    /* NOTE: Can't use layer_state_is, or IS_LAYER_ON because the layer_state
     * variable isn't actually updated until after this function is finished,
     * so have to use the layer_state_cmp directly */
    if (layer_state_cmp(state, __VIM))
        ergodox_right_led_3_on();
    else if (layer_state_cmp(state, __GOLAND))
        ergodox_right_led_3_on();
    else
        ergodox_right_led_3_off();

    if (layer_state_cmp(state, __MAC))
        ergodox_right_led_2_on();
    else if (layer_state_cmp(state, __LINUX))
        ergodox_right_led_2_off();

    return state;
}

uint32_t default_layer_state_set_user(uint32_t state) {
    if (layer_state_cmp(state, __DVORAK))
        ergodox_right_led_1_off();
    else if (layer_state_cmp(state, __QWERTY))
        ergodox_right_led_1_on();
    return state;
}

void matrix_scan_user(void) {
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > ALT_TAB_LENGTH) {
            if (IS_LAYER_ON(__MAC))
                unregister_code(KC_LGUI);
            else
                unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
    } 
}
