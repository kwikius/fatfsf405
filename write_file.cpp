#include "fatfs.h"
#include "../system/serial_port.hpp"

const char* file_error_msg( FRESULT fr);

bool write_file_test()
{
   const char * filename = "dummy.txt";
   serial_port::write("\n---- SD Card write test : opening \"");
   serial_port::write(filename);
   serial_port::write("\" ---------\n\n");

   FIL fil; 
   FRESULT fr  = f_open(&fil,filename, FA_CREATE_ALWAYS | FA_WRITE);
   if ( fr == FR_OK){
      if ( f_puts("-----------\n",&fil) == EOF){
         serial_port::write("file write line failed\n");
      }
      f_puts("Hello World\n",&fil);
      f_puts("-----------\n",&fil);
      f_close(&fil);
      serial_port::write("\n-------------- closed \"");
      serial_port::write(filename);
      serial_port::write("\" ----------------\n\n");
      return true;
   }else{
      serial_port::write("file open failed\n");
      serial_port::write(file_error_msg(fr));
      serial_port::write("\n");
      return false;
   }
}