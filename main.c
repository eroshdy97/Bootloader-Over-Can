#include "TX/TX.h"
#include "RX/RX.h"
#include "test/test.h"
#include "def.h"

int main()
{

#if isTX
    TX();
#else
    RX();
#endif

//    test();
    return 0;
}
