#include "fatfs.h"
#include "../system/serial_port.hpp"

const char* file_error_msg( FRESULT fr);

bool read_file(const char* filename)
{
   serial_port::write("\n---- SD Card read test : opening \"");
   serial_port::write(filename);
   serial_port::write(" \"---------\n\n");
   FIL fil;       /* File object */
   FRESULT fr  = f_open(&fil, "message.txt", FA_READ);
   if ( fr == FR_OK){
      char line[82]; 
      while (f_gets(line, sizeof line, &fil)){
         while (serial_port::get_tx_buffer_free_space() < sizeof line){
            asm volatile ("nop":::);
         }
         serial_port::printf<sizeof line>("%s",line);
      }
      f_close(&fil);
      serial_port::write("\n--------------closed \"");
      serial_port::write(filename);
      serial_port::write(" \"----------------\n\n");
      
      while (! serial_port::tx_reg_empty()) {asm volatile ("nop":::) ;}
      return true;
   }else{
      serial_port::write("file open failed\n");
      serial_port::write(file_error_msg(fr));
      serial_port::write("\n");

      return false;
   }
}
