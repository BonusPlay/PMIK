#include "main.h"

typedef struct Button {
    uint8_t xPos;
    uint8_t yPos;
    uint8_t xSize;
    uint8_t ySize;
    uint8_t pressed;

    /* potentially fields for display data */

} menu_button;

static menu_button buttons[3];

static int32_t temperature;
static int32_t pressure;
static int32_t humidity;

static uint8_t needsUpdate = 0;

static void SystemClock_Config(void);
static void configure_buttons(void);
static void get_position(void);
static void configure_usart();
static void configure_spi();
static void configure_adc();
static void configure_timer();
static void error();

static void broadcast();

UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;

BMP280_HandleTypedef BmpHandle;

ADC_HandleTypeDef AdcHandle;

TIM_HandleTypeDef TimHandle;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    HAL_Init();
    SystemClock_Config(); 

    // debugging
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

    /* error(); */

    /* configure_adc(); */
    configure_usart();
    /* configure_spi(); */
    /* configure_timer(); */

    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_LAYER1);
    BSP_LCD_SelectLayer(1);
    BSP_LCD_Clear(LCD_COLOR_WHITE);  
    BSP_LCD_SetColorKeying(1, LCD_COLOR_WHITE);
    BSP_LCD_SetLayerVisible(1, DISABLE);
  
    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER_LAYER0);
    BSP_LCD_SelectLayer(0);
 
    BSP_LCD_DisplayOn();

    BSP_LCD_Clear(LCD_COLOR_WHITE);

    calibrate_ts();

    BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

    configure_buttons();

    while (1)
    { 
        get_position();
    }
}

static void reset_buttons(void) {
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    BSP_LCD_DrawRect(buttons[0].xPos, buttons[0].yPos, buttons[0].xSize, buttons[0].ySize);
    BSP_LCD_DisplayStringAt(buttons[0].xPos + 3, buttons[0].yPos + buttons[0].ySize/2, (uint8_t *)"Status data here", CENTER_MODE);

    BSP_LCD_DrawRect(buttons[1].xPos, buttons[1].yPos, buttons[1].xSize, buttons[1].ySize);
    BSP_LCD_DisplayStringAt(buttons[1].xPos + 3, buttons[1].yPos + buttons[1].ySize/2, (uint8_t *)"Pressure/temp", LEFT_MODE);
    BSP_LCD_DisplayStringAt(buttons[1].xPos + 3, buttons[1].yPos + buttons[1].ySize/2+15, (uint8_t *)"data here", LEFT_MODE);

    BSP_LCD_DrawRect(buttons[2].xPos, buttons[2].yPos, buttons[2].xSize, buttons[2].ySize);
    BSP_LCD_DisplayStringAt(buttons[2].xPos + 3, buttons[2].yPos + buttons[2].ySize/2, (uint8_t *)"Something else", LEFT_MODE);
    BSP_LCD_DisplayStringAt(buttons[2].xPos + 23, buttons[2].yPos + buttons[2].ySize/2+15, (uint8_t *)"or like", LEFT_MODE);
    BSP_LCD_DisplayStringAt(buttons[2].xPos + 3, buttons[2].yPos + buttons[2].ySize/2+30, (uint8_t *)"door bell here", LEFT_MODE);
    buttons[0].pressed = 0;
    buttons[1].pressed = 0;
    buttons[2].pressed = 0;

    needsUpdate = 0;
}

/**
  * @brief  Configures and gets Touch screen position.
  * @param  None
  * @retval None
  */
static void get_position(void)
{
    static uint32_t x = 0, y = 0;
    static TS_StateTypeDef  TS_State;  
  
    BSP_TS_GetState(&TS_State); 
  
    x = Calibration_GetX(TS_State.X);
    y = Calibration_GetX(TS_State.Y);
  
    if ((TS_State.TouchDetected) & (x > buttons[0].xPos) & (x < buttons[0].xPos + buttons[0].xSize) &
          (y > buttons[0].yPos) & (y < buttons[0].yPos + buttons[0].ySize))
    {
        broadcast();
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_FillRect(buttons[0].xPos, buttons[0].yPos, buttons[0].xSize, buttons[0].ySize);
        HAL_Delay(50);
        needsUpdate = 1;
    }
    else if ((TS_State.TouchDetected) & (x > buttons[1].xPos) & (x < buttons[1].xPos + buttons[1].xSize) &
            (y > buttons[1].yPos) & (y < buttons[1].yPos + buttons[1].ySize))
    {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_FillRect(buttons[1].xPos, buttons[1].yPos, buttons[1].xSize, buttons[1].ySize);
        HAL_Delay(50);
        needsUpdate = 1;
    }
    else if ((TS_State.TouchDetected) & (x > buttons[2].xPos) & (x < buttons[2].xPos + buttons[2].xSize) &
            (y > buttons[2].yPos) & (y < buttons[2].yPos + buttons[2].ySize))
    {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_FillRect(buttons[2].xPos, buttons[2].yPos, buttons[2].xSize, buttons[2].ySize);
        HAL_Delay(50);
        needsUpdate = 1;
    }
    else
    {
        if(needsUpdate)
        {
            reset_buttons();
        }
    }
}

/**
  * @brief Set button boundaries.
  * @param  None
  * @retval None
  */
static void configure_buttons(void)
{ 
    BSP_LCD_SelectLayer(0);
  
    BSP_LCD_Clear(LCD_COLOR_WHITE);  
  
    buttons[0].xPos = 3;
    buttons[0].yPos = 3;
    buttons[0].xSize = (BSP_LCD_GetXSize() - 6);
    buttons[0].ySize = (BSP_LCD_GetYSize()/2 - 3);

    buttons[1].xPos = 3;
    buttons[1].yPos = (BSP_LCD_GetYSize()/2 + 3);
    buttons[1].xSize = (BSP_LCD_GetXSize()/2 - 6);
    buttons[1].ySize = (BSP_LCD_GetYSize()/2 - 6);

    buttons[2].xPos = (BSP_LCD_GetXSize()/2 + 3);
    buttons[2].yPos = (BSP_LCD_GetYSize()/2 + 3);
    buttons[2].xSize = (BSP_LCD_GetXSize()/2 - 6);
    buttons[2].ySize = (BSP_LCD_GetYSize()/2 - 6);

    reset_buttons();

//  BSP_LCD_DisplayStringAt(210, (BSP_LCD_GetYSize()-55), (uint8_t *)"Color", LEFT_MODE);
//  BSP_LCD_DisplayStringAt(210, (BSP_LCD_GetYSize()-45), (uint8_t *)"Size", LEFT_MODE);
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
  
    /* The voltage scaling allows optimizing the power consumption when the device is 
       clocked below the maximum system frequency, to update the voltage scaling value 
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

static void configure_usart()
{
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle.Instance          = USARTx;
  
    UartHandle.Init.BaudRate     = 9600;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&UartHandle) != HAL_OK)
      error();

    DEBUG_BLINK();
}

static void configure_spi()
{
    /*##-1- Configure the SPI peripheral #######################################*/
    /* Set the SPI parameters */
    BmpHandle.spi.Instance               = SPIx;
    BmpHandle.spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    BmpHandle.spi.Init.Direction         = SPI_DIRECTION_2LINES;
    BmpHandle.spi.Init.CLKPhase          = SPI_PHASE_1EDGE;
    BmpHandle.spi.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    BmpHandle.spi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    BmpHandle.spi.Init.CRCPolynomial     = 7;
    BmpHandle.spi.Init.DataSize          = SPI_DATASIZE_8BIT;
    BmpHandle.spi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    BmpHandle.spi.Init.NSS               = SPI_NSS_HARD_OUTPUT;
    BmpHandle.spi.Init.TIMode            = SPI_TIMODE_DISABLE;
    BmpHandle.spi.Init.Mode              = SPI_MODE_MASTER;

    if(HAL_SPI_Init(&BmpHandle.spi) != HAL_OK)
        error();

    /*##-3- Wait for the end of the transfer ###################################*/  
    /*  Before starting a new communication transfer, you need to check the current   
        state of the peripheral; if its busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */  
    while (HAL_SPI_GetState(&BmpHandle.spi) != HAL_SPI_STATE_READY)
    {}

    bmp280_params_t params;
    bmp280_init_default_params(&params);
    if (!bmp280_init(&BmpHandle, &params))
        error();
}

/**
 * @brief generic error handler, it lights LED4 and ganks execution
 */
static void error()
{
    BSP_LED_On(LED4);
    while(1)
    {}
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete*/
    UartReady = SET;
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
/* void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
 * {
 *     [> Set transmission flag: transfer complete<]
 *     UartReady = SET;
 * } */

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
/* void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
 * {
 *     error();
 * } */

static void broadcast()
{
    uint8_t buf[] = "A";
    /*##-2- Start the transmission process #####################################*/  
    /* While the UART in reception process, user can transmit data through 
       "aTxBuffer" buffer */
    if (HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)buf, sizeof(uint8_t)) != HAL_OK)
        error();

    /*##-3- Wait for the end of the transfer ###################################*/   
    while (UartReady != SET)
    {}
  
    /* Reset transmission flag */
    UartReady = RESET;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi)
{
    // pull NSS down
    /* HAL_GPIO_WritePin(SPIx_NSS_GPIO_PORT, SPIx_NSS_PIN, GPIO_PIN_RESET); */
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    // pull NSS up
    /* HAL_GPIO_WritePin(SPIx_NSS_GPIO_PORT, SPIx_NSS_PIN, GPIO_PIN_SET); */
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
    temperature = (((HAL_ADC_GetValue(AdcHandle) / 1024) - 0.5) * 100);
}

void configure_adc()
{
    /* setup adc */
    AdcHandle.Instance = ADCx;
 
    AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
    AdcHandle.Init.ScanConvMode = DISABLE;
    AdcHandle.Init.ContinuousConvMode = ENABLE;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    AdcHandle.Init.NbrOfDiscConversion = 0;
    AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    AdcHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
    AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.NbrOfConversion = 1;
    AdcHandle.Init.DMAContinuousRequests = ENABLE;
    AdcHandle.Init.EOCSelection = DISABLE;

    if(HAL_ADC_Init(&AdcHandle) != HAL_OK)
        error();

    /* setup channel */
    ADC_ChannelConfTypeDef adcChannel;

    adcChannel.Channel = ADCx_CHANNEL;
    adcChannel.Rank = 1;
    adcChannel.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    adcChannel.Offset = 0;

    if (HAL_ADC_ConfigChannel(&AdcHandle, &adcChannel) != HAL_OK)
        error();

    HAL_ADC_Start_IT(&AdcHandle);
}

HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // TODO: get data from BSP
    if (!bmp280_read_fixed(&BmpHandle, &temperature, &pressure, &humidity))
        error();
}

static void configure_timer()
{
    TimHandle.Instance = TIMx;
    TimHandle.Init.Prescaler = 40000;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TimHandle.Init.Period = 500;
    TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TimHandle.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init(&TimHandle);
    HAL_TIM_Base_Start(&TimHandle);
}
