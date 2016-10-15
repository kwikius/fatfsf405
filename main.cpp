

#include "../system/serial_port.hpp"
#include "../system/led.hpp"
#include <quan/stm32/millis.hpp>

bool test_read_file();
bool test_write_file();
bool test_mkdir();

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
        {.pfn = test_mkdir, .name = "dir ops"}
       ,{.pfn = test_write_file, .name = "write a file"}
       ,{.pfn = test_read_file , .name = "read a file"}
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


