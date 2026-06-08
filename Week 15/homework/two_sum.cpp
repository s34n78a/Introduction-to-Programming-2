#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int main(void) {
    int t;
    cin >> t;

    while (t--)
    {
        int target;
        int n;
        cin >> target;
        cin >> n;

        int arr[n];
        for (int i = 0; i < n; i++)
        {
            cin >> arr[i];
        }

        unordered_map<int, int> map;
        int found = 0;
        for (int i = 0; i < n; i++)
        {
            int complement = target - arr[i];
            if (map.find(complement) != map.end())
            {
                cout << map[complement] << " " << i << endl;
                found = 1;
                break;
            }

            map[arr[i]] = i;
        }

        if (!found)
        {
            cout << "None" << endl;
        }
    }
    
    return 0;
}