#include "main.h"
#include "cmd.h"
#include "io.h"
#include "adns9800.h"


typedef struct _p_list
{
    char *para;
    struct _p_list *next;
} P_LIST_t;



void cmd_process(unsigned char *cmd_buf)
{
    P_LIST_t *para_head, *curr;
    char *p = NULL;
    unsigned char p_count = 0;


    para_head = (P_LIST_t *)alloca(sizeof(P_LIST_t));

    p = strtok(cmd_buf, ",");
    curr = para_head;

    while (p)
    {
        p_count++;

        curr->para = p;
        curr->next = (P_LIST_t *)alloca(sizeof(P_LIST_t));
        curr = curr->next;

        p = strtok(NULL, ",");
    }
    curr->next = NULL;

    do 
    {        
        if ( !strcmp_P(para_head->para, PSTR("hi")) )
        {
            printf("compiled @ %s\n", __DATE__ "," __TIME__);
        }
        else if ( !strcmp_P(para_head->para, PSTR("spi_r")) )
        {
            printf("-> 0x%02x\n", adns9800_read_reg(0x02));
            printf("-> 0x%02x\n", adns9800_read_reg(0x07));
        }
        else if ( !strcmp_P(para_head->para, PSTR("spi_w")) )
        {
            printf("write\n");
            adns9800_write_reg(0x0a, 0x19);
        }
        else if ( !strcmp_P(para_head->para, PSTR("motion")) )
        {
            unsigned char m, o, s;
            signed short x, y;

            adns9800_motion(&m, &o, &x, &y, &s);
            printf("0x%02x, 0x%02x, 0x%02x | dx: %d, dy: %d\n", m, o, s, x, y);
        }
        else if ( !strcmp_P(para_head->para, PSTR("capture")) )
        {
            adns9800_capture();
        }
        else
        {
            printf("unknow cmd: %s\n", para_head->para);
        }

    } while (0);
    
}

