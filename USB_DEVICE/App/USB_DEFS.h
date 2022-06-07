#ifndef USB_DEFINITIONS
#define USB_DEFINITIONS

#define USAGE_PAGE 	0x05
#define GENERIC_DESKTOP 0x01
#define BUTTON          0x09
#define HAT_SWITCH      0x39
#define HAPTICS_PAGE	0x0E
#define ORDINAL_PAGE	0x0A

#define USAGE_USAGE	0x09
#define USAGE_JOYSTICK  0x04
#define USAGE_GAMEPAD   0x05
#define USAGE_POINTER   0x01

#define REPORT_ID       0x85

#define USAGE_X         0x30 // control stick
#define USAGE_Y         0x31

#define USAGE_Z        	0x32 // C-stick
#define USAGE_RX        0x33
#define USAGE_CX        USAGE_Z
#define USAGE_CY        USAGE_RX

#define USAGE_RY        0x34 // L/R analog triggers
#define USAGE_RZ        0x35
#define USAGE_L         USAGE_RY
#define USAGE_R         USAGE_RZ

#define USAGE_MIN       0x19
#define USAGE_MAX       0x29
#define LOGICAL_MIN     0x15
#define LOGICAL_MAX     0x25
#define LOGICAL_MIN_2	0x16
#define LOGICAL_MAX_2	0x26
#define LOGICAL_MAX_4	0x27
#define REPORT_SIZE     0x75
#define REPORT_COUNT    0x95
#define PHYSICAL_MIN    0x35
#define PHYSICAL_MAX    0x46 // 2 INPUTS???????

#define REPORT_ID	0x85

#define SIMPLE_HAPTIC	0x01
#define WAVEFORM_LIST	0x10
#define DURATION_LIST	0x11
#define AUTO_TRIGGER	0x20
#define MANUAL_TRIGGER	0x21


#define UNIT_EXPONENT   0x55
#define UNIT            0x65

#define USB_INPUT       0x81
#define USB_FEATURE	0xB1
#define USB_OUTPUT 	0x91

#define CONST_VAR_ABS   0x03
#define DATA_VAR_ABS    0x02

#define COLLECTION      0xA1
#define COL_APPLICATION 0x01
#define COL_PHYSICAL    0x00
#define COL_LOGICAL	0x02
#define END_COLLECTION	0xC0




struct gamepad_report_t
{
  uint8_t buttons;
  uint8_t ctrl_X;
  uint8_t ctrl_Y;
  uint8_t cstk_X;
  uint8_t cstk_Y;
  uint8_t analog_L;
  uint8_t analog_R;
  uint8_t dPad;
};

#endif
