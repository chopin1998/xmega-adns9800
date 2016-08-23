#include "cmd_buff.h"

void cmd_buf_init(CMD_BUF_t *buf,
                  signed char (*ser_dequeue)(void),
                  void (*process)(unsigned char *))
{
    buf->status = OUT_PACKET;
    buf->data[0] = 0x00;
    buf->index = 0;

    buf->ser_dequeue = ser_dequeue;
    buf->process = process;
}

signed char cmd_buf_tick(CMD_BUF_t *buf)
{
    signed char recv;
    
    recv = buf->ser_dequeue();

    if (recv == -1)
    {
        return -1;
    }

    if (buf->status == OUT_PACKET)
    {
        if (recv == STX)
        {
            buf->status = IN_PACKET;
            buf->index = 0;
            buf->data[0] = 0x00;
        }
        else
        {
            /* ignor */
        }
    }
    else
    {                           /* packet_status == IN_PACK */
        if (recv == ETX)
        {                       /* end of a packet */
            buf->status = OUT_PACKET;
            buf->data[buf->index] = 0x00; /* padding a terminal char */

            buf->process(buf->data);
        }
        else
        {                       /* normally */
            buf->data[buf->index] = recv;
            buf->index += 1;

            if (buf->index >= CMD_BUF_MAX-1)
            {                   /* a huge packet ? */
                buf->status = OUT_PACKET;
                buf->index = 0;
                buf->data[0] = 0x00;
            }
        }
    }

    return 0;
}



