#include "main.h"
#include "adns9800.h"
#include "spi.h"
#include "io.h"


volatile unsigned char MOTION = 0;

ISR (ADNS9800_MOTION_vect)
{
    LED_PORT.OUTTGL = LED_DIAG;
    // adns9800_write_reg(A_MOTION, 0x00);

    MOTION = 1;
}

void adns9800_init(void)
{
    unsigned char reg;
    
    
    ADNS9800_PORT.DIRSET = PIN7_bm | PIN5_bm | PIN4_bm;
    ADNS9800_PORT.DIRCLR = PIN6_bm;
    ADNS9800_PORT.DIRCLR = ANDS9800_MOTION_PIN;
    ADNS9800_PORT.ADNS9800_MOTION_PINCTRL = PORT_ISC_FALLING_gc;
    ADNS9800_PORT.INT0MASK |= ANDS9800_MOTION_PIN;
    ADNS9800_PORT.INTCTRL |= PORT_INT0LVL_LO_gc;

    spi_init(&ADNS9800_SPI);
    
    ADNS9800_CS_H;
    _delay_ms(100);
    
    adns9800_write_reg(A_POWER_UP_RESET, 0x5a);
    _delay_ms(100);
    adns9800_read_reg(A_MOTION);
    adns9800_read_reg(A_DX_L);
    adns9800_read_reg(A_DX_H);
    adns9800_read_reg(A_DY_L);
    adns9800_read_reg(A_DY_H);

    reg = adns9800_read_reg(A_LASER_CTRL0);
    reg &= 0xf0;
    adns9800_write_reg(A_LASER_CTRL0, reg);
}


unsigned char adns9800_read_reg(unsigned char addr)
{
    unsigned char rev;
    
    ADNS9800_CS_L; asm("nop\n"); asm("nop\n");

    spi_writeread(&ADNS9800_SPI, addr); _delay_us(50);
    rev = spi_writeread(&ADNS9800_SPI, 0x00);

    ADNS9800_CS_H;

    return rev;
}


void adns9800_write_reg(unsigned char addr, unsigned char val)
{
    ADNS9800_CS_L; asm("nop\n"); asm("nop\n");

    spi_writeread(&ADNS9800_SPI, addr | 0x80); _delay_us(50);
    spi_writeread(&ADNS9800_SPI, val);

    ADNS9800_CS_H;
}


void adns9800_motion(unsigned char *motion, unsigned char *observation,
                     signed short *dx, signed short *dy,
                     unsigned char *squal)
{
    unsigned char reg;
    
    ADNS9800_CS_L; asm("nop\n"); asm("nop\n");

    spi_writeread(&ADNS9800_SPI, A_MOTION_BURST); _delay_us(25);
    
    *motion = spi_writeread(&ADNS9800_SPI, 0x00);
    *observation = spi_writeread(&ADNS9800_SPI, 0x00);
    
    reg = spi_writeread(&ADNS9800_SPI, 0x00); /* dx_l */
    *dx = (spi_writeread(&ADNS9800_SPI, 0x00) << 8) | reg;

    reg = spi_writeread(&ADNS9800_SPI, 0x00);
    *dy = (spi_writeread(&ADNS9800_SPI, 0x00) << 8) | reg;
    
    *squal = spi_writeread(&ADNS9800_SPI, 0x00);
        
    ADNS9800_CS_H;
    _delay_us(1);
}


void adns9800_capture(void)
{
    unsigned int i = 0;
    unsigned char start = 0;
    unsigned char reg = 0x00;
    
    adns9800_write_reg(A_FRAME_CAPTURE, 0x83);
    _delay_ms(2);
    
    ADNS9800_CS_L; asm("nop\n"); asm("nop\n");
    spi_writeread(&ADNS9800_SPI, A_PIXEL_BURST); _delay_us(55);

    // printf("--->");
    for (;;)
    {
        reg = spi_writeread(&ADNS9800_SPI, 0x00);

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
    
    ADNS9800_CS_H;
    _delay_us(4);
}

