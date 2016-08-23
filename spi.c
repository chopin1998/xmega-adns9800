#include "main.h"
#include "spi.h"


inline unsigned char spi_writeread(SPI_t *dev, unsigned char data)
{
    dev->DATA = data;
    loop_until_bit_is_set(dev->STATUS, SPI_IF_bp);
    return dev->DATA;
}

void spi_init(SPI_t *dev)
{
    dev->CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESCALER_DIV16_gc | SPI_MODE_3_gc;
}
