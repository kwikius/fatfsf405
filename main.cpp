/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx_hal.h"

#include "../system/serial_port.hpp"
#include "../system/led.hpp"
#include <quan/stm32/millis.hpp>

void Error_Handler(void);

bool read_file();

bool write_file_test();

void SD_Card_Init();

bool test(bool (*pfn)(), const char* name);

namespace {
   struct fun_pair{
      typedef bool (*pfn_t)();

      pfn_t pfn;
      const char* name;
   };

   void blink_forever();
}

using quan::stm32::millis;

int main(void)
{
   SD_Card_Init();

   fun_pair fun_tests [] = {
       {.pfn = write_file_test,.name = "write_file"}
       ,{.pfn = read_file, .name = "read_file"}
   };

   for ( auto pr : fun_tests){
      test(pr.pfn,pr.name);
   }

   blink_forever();
}

namespace {

   void blink_forever()
   {
      auto now = millis();
      while (1){
         if ( (millis() - now) > quan::time::ms{250}){
            now = millis();
            led::complement();
         }
      }
   }
}

void Error_Handler(void)
{
  led::on();
  while(1) 
  {
  }
}


