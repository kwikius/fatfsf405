#include "stm32f4xx_hal.h"

#include <quan/stm32/gpio.hpp>
#include <quan/stm32/rcc.hpp>

extern "C" void HAL_SD_MspInit(SD_HandleTypeDef* hsd)
{

   if(hsd->Instance==SDIO){
/*
    PC8     ------> SDIO_D0
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD 
*/
   typedef quan::mcu::pin<quan::stm32::gpioc,8> sdio_d0;
   typedef quan::mcu::pin<quan::stm32::gpioc,12> sdio_ck;
   typedef quan::mcu::pin<quan::stm32::gpiod,2> sdio_cmd;

   quan::stm32::module_enable<sdio_d0::port_type>();
   quan::stm32::module_enable<sdio_ck::port_type>();
   quan::stm32::module_enable<sdio_cmd::port_type>();
 
   typedef quan::stm32::gpio::mode::af12 sdio_mode;

   quan::stm32::apply<
      sdio_d0
      ,sdio_mode 
      ,quan::stm32::gpio::pupd::none         //  Use external pullup 5V tolerant pins
      ,quan::stm32::gpio::ospeed::fast
   >();

   quan::stm32::apply<
      sdio_ck
      ,sdio_mode 
      ,quan::stm32::gpio::pupd::none         //  Use external pullup 5V tolerant pins
      ,quan::stm32::gpio::ospeed::fast
   >();

   quan::stm32::apply<
      sdio_cmd
      ,sdio_mode 
      ,quan::stm32::gpio::pupd::none         //  Use external pullup 5V tolerant pins
      ,quan::stm32::gpio::ospeed::fast
   >();

  quan::stm32::rcc::get()->apb2enr.setbit<11>();  // enable SDIO clock
  }
}