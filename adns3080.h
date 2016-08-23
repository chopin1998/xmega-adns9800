#ifndef _ADNS3080
#define _ADNS3080 1

#define ADNS3080_SPI  SPIC
#define ADNS3080_PORT PORTC
    #define ADNS3080_NCS_PIN PIN4_bm


#define ADNS3080_CS_H   (ADNS3080_PORT.OUTSET = ADNS3080_NCS_PIN)
#define ADNS3080_CS_L   (ADNS3080_PORT.OUTCLR = ADNS3080_NCS_PIN)



#define A_PID                    0x00
#define A_VID                    0x01
#define A_MOTION                 0x02
#define A_DX                     0x03
#define A_DY                     0x04
#define A_SQUAL                  0x05
#define A_PIXEL_SUM              0x06
#define A_MAX_PIXEL              0x07
#define A_CONF                   0x0a
#define A_EXT_CONF               0x0b
#define A_DATA_OUT_LOWER         0x0c
#define A_DATA_OUT_UPPER         0x0d
#define A_SHUTTER_LOWER          0x0e
#define A_SHUTTER_UPPER          0x0f
#define A_FRAME_PERIOD_LOWER     0x10
#define A_FRAME_PERIOD_UPPER     0x11
#define A_MOTION_CLEAR           0x12
#define A_FRAME_CAPTURE          0x13
#define A_SROM_ENABLE            0x14
#define A_FRAME_PERIOD_MAX_BOUND_LOWER 0x19
#define A_FRAME_PERIOD_MAX_BOUND_UPPER 0x1a
#define A_FRAME_PERIOD_MIN_BOUND_LOWER 0x1b
#define A_FRAME_PERIOD_MIN_BOUND_UPPER 0x1c
#define A_SHUTTER_MAX_BOUND_LOWER 0x1d
#define A_SHUTTER_MAX_BOUND_UPPER 0x1e
#define A_OBSERVATION            0x3d
#define A_PIV_INVERSE            0x3f
#define A_PIXEL_BURST            0x40
#define A_MOTION_BURST           0x50
#define A_SROM_LOAD              0x60




void adns3080_init(void);
unsigned char adns3080_read_reg(unsigned char addr);
void adns3080_write_reg(unsigned char addr, unsigned char val);


void adns3080_motion(unsigned char *motion,
                     signed char *dx, signed char *dy,
                     unsigned char *squal,
                     unsigned char *shutter_upper, unsigned char *shutter_lower,
                     unsigned char *max_pixel);

void adns3080_capture(void);


#endif
