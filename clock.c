#include "main.h"
#include "clock.h"
#include "io.h"


volatile unsigned char rtc_flag = 0;

ISR (RTC_OVF_vect)
{
    static unsigned char sys_led = 0;
    
    rtc_flag++;

    for (unsigned char i=0; i<RTC_HANDLER_NUMBER; i++)
    {
        if (rtc_handler[i])
        {
            rtc_handler[i]();
        }
    }

    // if (sys_led > 2)
    if (1)
    {
        LED_PORT.OUTTGL = LED_SYS;
        sys_led = 0;
    }
    else
    {
        sys_led++;
    }
}



void clock_init(void)
{
    /*
      OSC.CTRL |= OSC_RC2MEN_bm;
      loop_until_bit_is_set(OSC.STATUS, OSC_RC2MRDY_bp);

      OSC.PLLCTRL = OSC_PLLSRC_RC2M_gc | 16;
      OSC.CTRL |= OSC_PLLEN_bm;
      loop_until_bit_is_set(OSC.STATUS, OSC_PLLRDY_bp);

      CCP = CCP_IOREG_gc;
      CLK.CTRL = CLK_SCLKSEL_PLL_gc;
    */
    
    
    /* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
    XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC2MHZ);
    XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
    XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);


    /* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
    XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
    XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);
}

void clock_rtc_init(void)
{
    OSC.CTRL |= OSC_RC32KEN_bm;
    loop_until_bit_is_set(OSC.STATUS, OSC_RC32KRDY_bp);
    
    CLK.RTCCTRL = CLK_RTCEN_bm | CLK_RTCSRC_RCOSC_gc;
    loop_until_bit_is_clear(RTC.STATUS, WDT_SYNCBUSY_bp);
    
    RTC.PER = (512-1);         /* 8 - 1 */
    RTC.CNT = 0;
    RTC.CTRL = RTC_PRESCALER_DIV1_gc;

    RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;
}

