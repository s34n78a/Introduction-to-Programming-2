#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_table.h"
#define TABLE_COUNT 50
#define GUEST_COUNT 50
#define OPEN_TIME 660
#define CLOSE_TIME 900

Table* createTable() {
    Table *table = (Table*)malloc(sizeof(Table));
    table->guest = NULL;

    scanf("%d", &table->tableSize);
    table->leaveTime = 0;
    return table;
}

Guest* createGuest() {
    Guest *guest = (Guest*)malloc(sizeof(Guest));

    guest->guestName = (char*)malloc(10*sizeof(char));
    scanf("%s %d %d %d", guest->guestName,  &guest->groupSize, &guest->arriveTime, &guest->diningTime);
    
    return guest;
}

Guest* checkLeave(Table **table, int tableCount, int currentTime) {
    Guest *leaving_guest = (Guest*)malloc(sizeof(Guest));
    leaving_guest = NULL;
    for (int i = 0; i < tableCount; i++)
    {
        //printf("checkLeave: table %d, leaveTime %d, currentTime %d\n", i, table[i]->leaveTime, currentTime);
        if (table[i]->leaveTime == currentTime)
        {
            leaving_guest = table[i]->guest;
            table[i]->guest = NULL;
            table[i]->leaveTime = 0;
            break;
        }
    }
    //printf("checkLeave: leaving_guest %s at %d\n", leaving_guest ? leaving_guest->guestName : "NULL", currentTime);
    return leaving_guest;
    free(leaving_guest);
}

int assignTable(Table **table, int tableCount, int currentTime, Guest *guest) {
    for (int i = 0; i < tableCount; i++)
    {
        if (table[i]->tableSize >= guest->groupSize && table[i]->leaveTime <= currentTime)
        {
            table[i]->guest = guest;
            table[i]->leaveTime = currentTime + guest->diningTime;
            return 1;
        }
    }
    return 0;
}
