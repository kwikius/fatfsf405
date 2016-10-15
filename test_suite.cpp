#include "../system/serial_port.hpp"

namespace {

   constexpr char test_magic[] = "$$$<<<***>>>$$$ ";
}

bool test(bool (*pfn)(), const char* name)
{
   serial_port::write(test_magic);
   serial_port::write("test = \"");
   serial_port::write(name);
   serial_port::write("\" {\n");

   bool result = pfn();
   serial_port::write(test_magic);
   serial_port::write("result = \"");
   if ( result){
       serial_port::write("PASS");
   }else{
       serial_port::write("FAIL");
   }
   serial_port::write("\"\n");
   serial_port::write(test_magic);
   serial_port::write("}\n\n");
   return result;
}