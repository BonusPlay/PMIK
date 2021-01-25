#include "main.h"

#define STM32_IC2_ADDR (0x15 << 1)
#define ESP8266_IC2_ADDR (0x19 << 1)

typedef struct Button {
	uint8_t xPos;
	uint8_t yPos;
	uint8_t xSize;
	uint8_t ySize;
	uint8_t pressed;

	/* potentially fields for display data */

} menu_button;

static menu_button buttons[3];

static uint8_t needsUpdate = 0;

static void SystemClock_Config(void);
static void configure_buttons(void);
static void get_position(void);
static void configure_i2c();
static void error();

extern I2C_HandleTypeDef I2cHandle;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config(); 

  configure_i2c();

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

static void configure_i2c()
{
    I2cHandle.Instance             = I2C3;

    I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2cHandle.Init.ClockSpeed      = 400000;
    I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    I2cHandle.Init.OwnAddress1     = STM32_IC2_ADDR;

    if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
        error();    

    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

    while (BSP_PB_GetState(BUTTON_KEY) != 1) {}

    while (BSP_PB_GetState(BUTTON_KEY) != 0) {}

    uint8_t buf[] = "A";
    if(HAL_I2C_Master_Transmit_IT(&I2cHandle, ESP8266_IC2_ADDR, buf, sizeof(uint8_t))!= HAL_OK)
      error();
}

/**
 * @brief generic error handler, it lights LED4 and ganks execution
 *
 */
static void error()
{
    BSP_LED_On(LED4);
    while(1)
    {}
}
