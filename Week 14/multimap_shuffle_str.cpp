#include <iostream>
#include <vector>
#include <map>

using namespace std;

string restoreString(string s, vector<int>& indices) {
    string shuffledString = "";
    
    map<int, char> mapis;
    map<int, char>::iterator iter;
    
    for(int i = 0; i < indices.size(); i++){
        mapis.insert(pair<int, char> (indices [i], s[i]));
    }
    for(iter = mapis.begin(); iter!=mapis.end(); iter++){
        shuffledString += iter->second;
    }
    
    return shuffledString;
}

int main()
{
    string s = "codeleet";
    vector<int> indices = {4,5,6,7,0,2,1,3};
    
    cout << restoreString(s, indices) << endl;
    
    return 0;
}