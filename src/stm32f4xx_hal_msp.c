#include "main.h"

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();
  /* Enable USART1 clock */
  USARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
    
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
    
  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART1 */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure UART Tx as alternate function */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
  
  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USARTx_IRQn);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  SPIx_SCK_GPIO_CLK_ENABLE();
  SPIx_MISO_GPIO_CLK_ENABLE();
  SPIx_MOSI_GPIO_CLK_ENABLE();
  /* SPIx_NSS_GPIO_CLK_ENABLE(); */

  /* Enable SPI clock */
  SPIx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* SPI SCK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = SPIx_SCK_AF;
  
  HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
    
  /* SPI MISO GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_MISO_PIN;
  GPIO_InitStruct.Alternate = SPIx_MISO_AF;
  
  HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);
  
  /* SPI MOSI GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
  GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    
  HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

  /* SPI NSS GPIO pin configuration */
/*   GPIO_InitStruct.Pin = SPIx_NSS_PIN;
 *   GPIO_InitStruct.Alternate = SPIx_NSS_AF;
 *
 *   HAL_GPIO_Init(SPIx_NSS_GPIO_PORT, &GPIO_InitStruct); */

  /*##-3- Configure the NVIC for SPI #########################################*/
  /* NVIC for SPI */
  HAL_NVIC_SetPriority(SPIx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(SPIx_IRQn);
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  /*##-1- Reset peripherals ##################################################*/
  SPIx_FORCE_RESET();
  SPIx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure SPI SCK as alternate function  */
  HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
  /* Configure SPI MISO as alternate function  */
  HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
  /* Configure SPI MOSI as alternate function  */
  HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
  
  /*##-3- Disable the NVIC for SPI ###########################################*/
  HAL_NVIC_DisableIRQ(SPIx_IRQn);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  return;
  GPIO_InitTypeDef          GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  ADCx_CHANNEL_GPIO_CLK_ENABLE();
  ADCx_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/ 
  GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
  
  /*##-3- Enable the NVIC for ADC ###########################################*/
  HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);   
  HAL_NVIC_EnableIRQ(ADC_IRQn);
}
  
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
  return;
  /*##-1- Reset peripherals ##################################################*/
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC3 Channel8 GPIO pin */
  HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
    
  /*##-3- Disable the NVIC for ADC ###########################################*/
  HAL_NVIC_DisableIRQ(ADC_IRQn);
}

void HAL_TIM_MspInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIM3 Peripheral clock enable */
  TIMx_CLK_ENABLE();

  /*##-2- Enable the NVIC for TIM ###########################################*/
  HAL_NVIC_SetPriority(TIMx_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIMx_IRQn);
}

void HAL_TIM_MspDeInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Reset peripherals ##################################################*/
  TIMx_FORCE_RESET();
  TIMx_RELEASE_RESET();
  
  /*##-2- Disable the NVIC for ADC ###########################################*/
  HAL_NVIC_DisableIRQ(TIMx_IRQn);
}
