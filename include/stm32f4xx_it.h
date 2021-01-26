#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "main.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void SysTick_Handler(void);
#ifdef USE_USB_OTG_FS
void OTG_FS_IRQHandler(void);
#else
void OTG_HS_IRQHandler(void);
#endif
void SPIx_IRQHandler(void);
void ADC_IRQHandler(void);
void TIMx_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */
