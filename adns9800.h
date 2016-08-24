#ifndef _ADNS9800
#define _ADNS9800 1

#define ADNS9800_SPI  SPIC
#define ADNS9800_PORT PORTC
    #define ADNS9800_NCS_PIN PIN4_bm
    #define ANDS9800_MOTION_PIN PIN3_bm
    #define ADNS9800_MOTION_PINCTRL PIN3CTRL
    #define ADNS9800_MOTION_vect    PORTC_INT0_vect


#define ADNS9800_CS_H   (ADNS9800_PORT.OUTSET = ADNS9800_NCS_PIN)
#define ADNS9800_CS_L   (ADNS9800_PORT.OUTCLR = ADNS9800_NCS_PIN)

#define ADNS9800_MOTION_READ (ADNS9800_PORT.IN & ANDS9800_MOTION_PIN)


#define A_PID                    0x00
#define A_VID                    0x01
#define A_MOTION                 0x02
#define A_DX_L                   0x03
#define A_DX_H                   0x04
#define A_DY_L                   0x05
#define A_DY_H                   0x06
#define A_SQUAL                  0x07
#define A_PIXEL_SUM              0x08
#define A_MAX_PIXEL              0x09
#define A_MIN_PIXEL              0x0a
#define A_SHUTTER_LOWER          0x0b
#define A_SHUTTER_UPPER          0x0c
#define A_FRAME_PERIOD_LOWER     0x0d
#define A_FRAME_PERIOD_UPPER     0x0e
#define A_CONFIG_I               0x0f
#define A_CONFIG_II              0x10
#define A_FRAME_CAPTURE          0x12
#define A_SROM_ENABLE            0x13
#define A_RUN_DOWNSHIFT          0x14
#define A_REST1_RATE             0x15
#define A_REST1_DOWNSHIFT        0x16
#define A_REST2_RATE             0x17
#define A_REST2_DOWNSHIFT        0x18
#define A_REST3_RATE             0x19
#define A_FRAME_PERIOD_MAX_BOUND_LOWER 0x1a
#define A_FRAME_PERIOD_MAX_BOUND_UPPER 0x1b
#define A_FRAME_PERIOD_MIN_BOUND_LOWER 0x1c
#define A_FRAME_PERIOD_MIN_BOUND_UPPER 0x1d
#define A_SHUTTER_MAX_BOUND_LOWER 0x1e
#define A_SHUTTER_MAX_MOUND_UPPER 0x1f
#define A_LASER_CTRL0            0x20
#define A_OBSERVATION            0x24
#define A_DATA_OUT_LOWER         0x25
#define A_DATA_OUT_UPPER         0x26
#define A_SROM_ID                0x2a
#define A_LIFT_DETECTION_THR     0x2e
#define A_CONFIG_V               0x2f
#define A_CONFIG_IV              0x39
#define A_POWER_UP_RESET         0x3a
#define A_SHUTDOWN               0x3b
#define A_INVERSE_PID            0x3f
#define A_SHAP_ANGLE             0x42
#define A_MOTION_BURST           0x50
#define A_SROM_LOAD_BURST        0x62
#define A_PIXEL_BURST            0x64



extern volatile unsigned char MOTION;


void adns9800_init(void);
unsigned char adns9800_read_reg(unsigned char addr);
void adns9800_write_reg(unsigned char addr, unsigned char val);


void adns9800_motion(unsigned char *motion, unsigned char *observation,
                     signed short *dx, signed short *dy,
                     unsigned char *squal);

void adns9800_capture(void);


#endif
