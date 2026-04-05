#include <stdio.h>
#include <stdint.h>


int main(void) {
    
    int t = 0;
    scanf("%d", &t);

    for (int i = 0; i < t; i++)
    {
        uint32_t signal = 0;
        scanf("%x", &signal);
        
        //printf("in: %0.8X\n", signal);

        //rule 1: check adjacent 1s
        if (signal & (signal << 1))
        {
            signal = 0xFFFFFFFF;
            printf("0x%0.8X\n", signal);
            continue;
        }

        //rule 2: msb lsb
        int msb = 31 - __builtin_clz(signal);
        int lsb = __builtin_ctz(signal);
        //printf("%d\n", msb);
        //printf("%d\n", lsb);

        //flip inbetween bits
        if (msb > lsb)
        {
            for (int i = lsb + 1; i < msb; i++)
            {
                signal ^= (1 << i);
            }
            
        }
        //printf("0x%0.8X\n", signal);
        
        //rule 3: checksum bytes
        uint8_t byte0 = (signal >> 24) & 0xFF;
        uint8_t byte1 = (signal >> 16) & 0xFF;
        uint8_t byte2 = (signal >> 8) & 0xFF;
        uint8_t byte3 = signal & 0xFF;
        // printf("0x%0.2X\n", byte0);
        // printf("0x%0.2X\n", byte1);
        // printf("0x%0.2X\n", byte2);
        // printf("0x%0.2X\n", byte3);

        byte3 = byte0 ^ byte1 ^ byte2 ^ byte3;
        signal = (signal & 0xFFFFFF00) | byte3;  
		printf("0x%08X\n", signal); 
    }
    return 0;
}