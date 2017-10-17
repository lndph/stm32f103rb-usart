#include "includes.h"

static void prvSetupHardware( void )
{
    /* Start with the clocks in their expected state. */
    RCC_DeInit();
    /* Enable HSE (high speed external clock). */
    RCC_HSEConfig( RCC_HSE_ON );

    /* Wait till HSE is ready. */
    while ( RCC_GetFlagStatus( RCC_FLAG_HSERDY ) == RESET ) {
    }

    /* HCLK = SYSCLK */
    RCC_HCLKConfig( RCC_SYSCLK_Div1 );
    /* PCLK2 = HCLK */
    RCC_PCLK2Config( RCC_HCLK_Div1 );
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config( RCC_HCLK_Div2 );
    /*ADCCLK = PCLK2/8 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);//Max ACDCLK is 14M, PCLK2 is 72M, so we use Para "RCC_PCLK2_Div8",The ADCCLK = 72/8 = 9M
    /* PLLCLK = 12MHz * 6 = 72 MHz. */
    RCC_PLLConfig( RCC_PLLSource_HSE_Div1, RCC_PLLMul_6 );
    /* Enable PLL. */
    RCC_PLLCmd( ENABLE );
    /* Wait till PLL is ready. */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
    }

    /* Select PLL as system clock source. */
    RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

    /* Wait till PLL is used as system clock source. */
    while ( RCC_GetSYSCLKSource() != 0x08 ) {
    }

    /* Enable GPIOA, GPIOB, GPIOC, GPIOD,  and AFIO clocks */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
                            | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE );
    /* Configure HCLK clock as SysTick clock source. */
    SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
}

void BoardInit(void)
{
    LedInit();

    KeyInit();

    UsartInit();

    SpiInit();

    Timer2Init();
}

int main()
{
    uint8_t ch;
    NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );

    prvSetupHardware();

    BoardInit();

    while(1)
    {
        ch = getchar();
        printf("%x\n",ch);
    };
}

uint32_t tb_cnt;
extern void Tick1sCallback(void)
{
    tb_cnt++;
    switch(tb_cnt%2)
    {
        case 0:
            LedBlink();

            printf("key %d\n",GetKeyValue());
            break;

        case 1:
            printf("AD %x\n",ReadCount());
            break;

        default:
            break;
    }
}

