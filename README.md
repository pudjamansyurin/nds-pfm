# Andes Performance Monitor (PFM)
## It is similar with DWT cycle counter in ARM architecture.



#### **`main.c`**
```c
#include "nds-pfm/pfm.h"


int main(void)
{
    unsigned int cycle;

    // calculate instruction cycles (I/D Cache enabled)
    pfm_start();
    {
        // process to be measured
    }
    cycle = pfm_read();
    pfm_stop();


    // calculate instruction cycles (I/D Cache disabled)
    pfm_disableIDCache();
    pfm_start();
    {
        // process to be measured
    }
    cycle = pfm_read();
    pfm_stop();
    pfm_enableIDCache();


    while(1) 
    {
        // main loop
    }
}

```
