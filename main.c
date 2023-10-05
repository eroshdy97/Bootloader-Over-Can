#include "APP/Bootloader/BOOTLOADER_interface.h"
#include "APP/Sender/SENDER_interface.h"

/* Define a macro to select the module to run (0 for Sender on ECU1, 1 for Bootloader on ECU2) */
#define bl 0

int main()
{
    /* Check if the 'bl' macro is set to 1 (Bootloader) or 0 (Sender) */
#if (bl == 1)
    /* Initialize the bootloader module */
    BOOTLOADER_Init();

    /* Start the bootloader operation */
    BOOTLOADER_Start();
#else
    /* Initialize the Sender module */
    SENDER_Init();

    /* Start the Sender operation */
    SENDER_Start();
#endif

    /* Return 0 to indicate successful execution (optional) */
    return 0;
}
