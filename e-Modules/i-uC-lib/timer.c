
#include <timer.h>


/*0-64 seconds*/
void delay(unsigned long ticks)
{
  
   unsigned long long lt = jiffers;

   while(!timeafter(jiffers,(lt+ ticks)));
   
   return;

}
