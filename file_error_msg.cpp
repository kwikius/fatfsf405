#include "fatfs.h"
#include "../system/serial_port.hpp"

const char * file_error_msg( FRESULT fr)
{
   const char * emsg = nullptr;
   switch (fr) {
      case FR_OK :
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
   return emsg;
}

