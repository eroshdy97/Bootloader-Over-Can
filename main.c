#include "APP/Bootloader/BOOTLOADER_interface.h"
#include "APP/Sender/SENDER_interface.h"


int main()
{
//
//   /* Initialize the bootloader module */
   BOOTLOADER_Init();

   /* Start the bootloader operation */
   BOOTLOADER_Start();

//   /* Initialize the Sender module */
//   SENDER_Init();
//
//   /* Start the Sender operation */
//   SENDER_Start();



    return 0;
}
