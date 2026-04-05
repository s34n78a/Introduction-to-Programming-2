#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "function_magic.h"

int main() {
    int n, q, rule;
    Mage mages[100];
    scanf("%d %d", &n, &q);
    for (int i = 0; i < n; i++) {
        scanf("%s %d", mages[i].name, &mages[i].value);
    }
    for (int i = 0; i < q; i++) {
        scanf("%d", &rule);
        solve(mages, n, rule);
        for (int j = 0; j < n; j++) {
            printf("%s\n", mages[j].name);
        }
    }
}