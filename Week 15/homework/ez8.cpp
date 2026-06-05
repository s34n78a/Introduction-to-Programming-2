#include <iostream>
#include <algorithm>

using namespace std;

int main(void) {
    int t;
    int arr[3][3];
    int solution[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

    cin >> t;
    
    while (t--) {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                cin >> arr[i][j];
            }
        }

        
    }
    return 0;
}