#ifndef _CMD_BUFF_H
#define _CMD_BUFF_H 1


#define STX     '['
#define ETX     ']'


enum {IN_PACKET, OUT_PACKET};

#define CMD_BUF_MAX 128
typedef struct
{
    unsigned char status;
    
    unsigned char data[CMD_BUF_MAX];
    unsigned char index;

    signed char (*ser_dequeue)(void);
    void (*process)(unsigned char *);

} CMD_BUF_t;
// volatile CMD_BUF_t CMD_BUF;



void cmd_buf_init(CMD_BUF_t *,
                  signed char (*ser_dequeue)(void),
                  void (*process)(unsigned char *));
signed char cmd_buf_tick(CMD_BUF_t *);

#endif
