#include "fatfs.h"
#include "../system/serial_port.hpp"

const char* file_error_msg( FRESULT fr);

/*

*/
bool test_mkdir()
{
  // check cur dir is root
   
   char pwd_buf [30] = {'\0'};
   FRESULT fr = f_getcwd(pwd_buf,30);

   if (fr == FR_OK){
     serial_port::write("pwd is \"");
     serial_port::write(pwd_buf);
     serial_port::write("\"\n");
   }else{
      serial_port::write("get cwd failed\n");
      serial_port::write(file_error_msg(fr));
      serial_port::write("\n");
      return false;
   }
   const char subdir_name[] = "aerflite";
   // check for exist of dir to make
   FILINFO fno;
   fr = f_stat(subdir_name, &fno);
   if (fr == FR_NO_FILE){
      fr = f_mkdir(subdir_name);
      if ( fr == FR_OK){
         fr = f_chdir(subdir_name);
         
         if (fr == FR_OK){
            fr = f_getcwd(pwd_buf,30);
            serial_port::write("dir change\n  pwd is \"");
            serial_port::write(pwd_buf);
            serial_port::write("\"\n");
            return true;
         }else{
            serial_port::write("cd subdir failed\n");
            serial_port::write(file_error_msg(fr));
            serial_port::write("\n");
            return false;
         }
      }else{
         serial_port::write("mkdir subdir failed\n");
         serial_port::write(file_error_msg(fr));
         serial_port::write("\n");
         return false;
      }
   }else {
      if (fr == FR_OK){
         serial_port::write("subdir exists\"");
         serial_port::write(subdir_name);
         serial_port::write("\"\n");
      }else{
         serial_port::write("stat subdir failed\n");
         serial_port::write(file_error_msg(fr));
         serial_port::write("\n");
      }
      return false;
    }
}