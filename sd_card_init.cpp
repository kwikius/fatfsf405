#include "fatfs.h"
#include "../system/serial_port.hpp"


static FATFS FatFs; 

void SD_Card_Init()
{
   if ( BSP_SD_Init() != MSD_OK){
      serial_port::write("Card init failed\n");
      while (1) {;}
   }
   
   MX_FATFS_Init();

   /* Register work area to the default drive */
   f_mount(&FatFs, "", 0);
}