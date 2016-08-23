#include "main.h"
#include "adns3080.h"
#include "spi.h"


void adns3080_init(void)
{
    ADNS3080_PORT.DIRSET = PIN7_bm | PIN5_bm | PIN4_bm;
    ADNS3080_PORT.DIRCLR = PIN6_bm;

    spi_init(&ADNS3080_SPI);
    ADNS3080_CS_H;
}


unsigned char adns3080_read_reg(unsigned char addr)
{
    unsigned char rev;
    
    ADNS3080_CS_L; asm("nop\n"); asm("nop\n");

    spi_writeread(&ADNS3080_SPI, addr); _delay_us(50);
    rev = spi_writeread(&ADNS3080_SPI, 0x00);

    ADNS3080_CS_H;

    return rev;
}


void adns3080_write_reg(unsigned char addr, unsigned char val)
{
    ADNS3080_CS_L; asm("nop\n"); asm("nop\n");

    spi_writeread(&ADNS3080_SPI, addr | 0x80); _delay_us(50);
    spi_writeread(&ADNS3080_SPI, val);

    ADNS3080_CS_H;
}


void adns3080_motion(unsigned char *motion,
                     signed char *dx, signed char *dy,
                     unsigned char *squal,
                     unsigned char *shutter_upper, unsigned char *shutter_lower,
                     unsigned char *max_pixel)
{
    ADNS3080_CS_L; asm("nop\n"); asm("nop\n");

    spi_writeread(&ADNS3080_SPI, A_MOTION_BURST); _delay_us(25);
    
    *motion = spi_writeread(&ADNS3080_SPI, 0x00);
    *dx = spi_writeread(&ADNS3080_SPI, 0x00);
    *dy = spi_writeread(&ADNS3080_SPI, 0x00);
    *squal = spi_writeread(&ADNS3080_SPI, 0x00);
    *shutter_upper = spi_writeread(&ADNS3080_SPI, 0x00);
    *shutter_lower = spi_writeread(&ADNS3080_SPI, 0x00);
    *max_pixel = spi_writeread(&ADNS3080_SPI, 0x00);
    
    ADNS3080_CS_H;
    _delay_us(4);
}


void adns3080_capture(void)
{
    unsigned int i = 0;
    unsigned char start = 0;
    unsigned char reg = 0x00;
    
    adns3080_write_reg(A_FRAME_CAPTURE, 0x83);
    _delay_ms(2);
    
    ADNS3080_CS_L; asm("nop\n"); asm("nop\n");
    spi_writeread(&ADNS3080_SPI, A_PIXEL_BURST); _delay_us(55);

    // printf("--->");
    for (;;)
    {
        reg = spi_writeread(&ADNS3080_SPI, 0x00);

        if (!start && (reg & 0x80) && (reg & 0x40))
        {
            start = 1;
            reg <<= 2;
            // printf("%02x ", reg);
            CDC_Device_SendByte(&CDC_IF, reg);
        }
        else if (start)
        {
            reg <<= 2;
            // printf("%02x ", reg);
            CDC_Device_SendByte(&CDC_IF, reg);
            _delay_us(1);

            if (i >= 900)
            {
                // printf("\n****\n");
                break;
            }
        }
        else
        {
            if (i > 32)
            {
                printf("\nfailed to find img head\n");
                break;
            }
        }

        i++;
    }
    
    ADNS3080_CS_H;
    _delay_us(4);
}

