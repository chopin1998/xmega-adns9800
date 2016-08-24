#include "main.h"
#include "cmd_buff.h"
#include "cmd.h"
#include "io.h"
#include "clock.h"
#include "adns9800.h"


USB_ClassInfo_CDC_Device_t CDC_IF =
	{
		.Config =
			{
				.ControlInterfaceNumber   = INTERFACE_ID_CDC_CCI,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};


// static FILE USBSerialStream;
volatile CMD_BUF_t CDC_BUF;

void _cdc_send_char(unsigned char data)
{
    CDC_Device_SendByte(&CDC_IF, data);
}


signed char _cdc_tick()
{
    unsigned char rev = CDC_Device_BytesReceived(&CDC_IF);
    unsigned char ch;
    
    if (rev > 0)
    {   
        ch = CDC_Device_ReceiveByte(&CDC_IF);
        return ch;
    }
    else
    {
        return -1;
    }
}


int main(void)
{
    signed char data;
    
    CCP = CCP_IOREG_gc;
    MCU.MCUCR = MCU_JTAGD_bm;
    
    clock_init();
    clock_rtc_init();
    io_init();
    adns9800_init();

    PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
    USB_Disable();
    USB_Detach();
    _delay_ms(100);
    USB_Attach();
    USB_Init();
    
	// CDC_Device_CreateStream(&CDC_IF, &USBSerialStream);
    fdevopen(_cdc_send_char, NULL);

    cmd_buf_init(&CDC_BUF, _cdc_tick, cmd_process);
    
	GlobalInterruptEnable();    /* sei(); */

    printf("\nstart\n");
	for (;;)
	{
        if (1)
        {
            cmd_buf_tick(&CDC_BUF);
        }

        if (1)
        {                       // USB stuff
            CDC_Device_USBTask(&CDC_IF);
            USB_USBTask(); // should be called in 30ms/loop
        }

        if (MOTION)
        {
            MOTION = 0;
            adns9800_write_reg(A_MOTION, 0x00);
            /*
            printf("0x%02x, 0x%02x | 0x%02x, 0x%02x\n",
                   adns9800_read_reg(A_DX_H), adns9800_read_reg(A_DX_L),
                   adns9800_read_reg(A_DY_H), adns9800_read_reg(A_DY_L));
            */
            unsigned char m, o, s;  
            signed short x, y;

            // adns9800_motion(&m, &o, &x, &y, &s);
            // printf("0x%02x, 0x%02x, 0x%02x | dx: %d, dy: %d\n", m, o, s, x, y);
        }
    }
}


/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	// LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	// LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&CDC_IF);

	// LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&CDC_IF);
}

