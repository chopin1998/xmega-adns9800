#ifndef _SPI_H
#define _SPI_H 1



unsigned char spi_writeread(SPI_t *dev, unsigned char data);
void spi_init(SPI_t *dev);

#endif
