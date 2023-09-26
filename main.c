#include "TX/TX.h"
#include "RX/RX.h"
#include "test/test.h"
#include "def.h"

int main()
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

#if isTX
    TX();
#else
    RX();
#endif

//    test();
    return 0;
}
