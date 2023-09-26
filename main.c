//#include "TX/TX.h"
//#include "RX/RX.h"
//#include "test/test.h"
//#include "def.h"

#include "APP/Bootloader/BOOTLOADER_interface.h"
#include "APP/Sender/SENDER_interface.h"


int main()
{

//#if isTX
//    TX();
//#else
//    RX();
//#endif
//    test();

    BOOTLOADER_Init();
    BOOTLOADER_Start();

    SENDER_Init();
    SENDER_Start();



    return 0;
}
