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
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "../system/serial_port.hpp"

void Error_Handler(void);

static void MX_GPIO_Init(void);

static FATFS FatFs; 

int main(void)
{
   MX_GPIO_Init();
   serial_port::write("SD test\n");
   
   if ( BSP_SD_Init() != MSD_OK){
      serial_port::write("Card init failed\n");
      while (1) {;}
   }
   
   MX_FATFS_Init();

   /* Register work area to the default drive */
   f_mount(&FatFs, "", 0);

   serial_port::write("\n--------------opening message.txt-------------------------------------------\n\n");
   FIL fil;       /* File object */
   FRESULT fr  = f_open(&fil, "message.txt", FA_READ);
   bool success = false;
   const char * emsg = nullptr;
   switch (fr) {
      case FR_OK :
         success = true;
         emsg = "no error";
      break;
      case FR_DISK_ERR:
         emsg = "disk error";
      break;
      case FR_INT_ERR:
         emsg = "internal error";
      break;
      case FR_NOT_READY:
         emsg = "storage device not ready";
      break;
      case FR_NO_FILE:
         emsg = "file not found";
      break;
      case FR_NO_PATH:
         emsg = "invalid path";
      break;
      case FR_INVALID_NAME:
         emsg = "invalid path name";
      break;
      case FR_DENIED:
         emsg = "access denied";
      break;
      case FR_EXIST:
         emsg = "file of same name exists already";
      break;
      case FR_INVALID_OBJECT:
         emsg = "invalid object";
      break;
      case FR_WRITE_PROTECTED:
         emsg = "file is write protected";
      break;
      case FR_INVALID_DRIVE:
         emsg = "invalid drive";
      break;
      case  FR_NOT_ENABLED:
         emsg = "not enabled";
      break;
      case FR_NO_FILESYSTEM:
         emsg = "no filesystem";
      break;
      case FR_MKFS_ABORTED:
         emsg = "mkfs aborted";
      break;
      case FR_TIMEOUT:
         emsg = "fs timeout";
      break;
      case FR_LOCKED:
         emsg = "fs locked";
      break;
      case FR_NOT_ENOUGH_CORE:
         emsg = "out of memory";
      break;
      case FR_TOO_MANY_OPEN_FILES:
         emsg = "too many open files";
      break;
      case FR_INVALID_PARAMETER:
         emsg = "invalid parameter";
      break;
      default:
         emsg = "unknown error";
      break;
   }

   if (success){
      char line[82]; /* Line buffer */
      /* Read all lines and display it */
      while (f_gets(line, sizeof line, &fil)){
         while (serial_port::get_tx_buffer_free_space() < sizeof line){
            asm volatile ("nop":::);
         }
         serial_port::printf<sizeof line>("%s",line);
      }
      f_close(&fil);
      serial_port::write("\n-------------closed message.txt-------------------------------------------\n\n");
   }else{
      serial_port::write("file open failed\n");
      serial_port::write(emsg);
      serial_port::write("\n");
   }

   while (1){
      asm volatile ("nop":::);
   }

}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

