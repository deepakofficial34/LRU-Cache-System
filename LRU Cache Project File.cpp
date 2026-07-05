/*

Project - LRU Cache implementation 
Name - G. Deepak
College - IIIT Lucknow 

*/
#include <iostream>
// #include<bits/stdc++.h>
// we have used doubly linked list as it was readily availaible in the c++ stl library 
// hence no need to write its functions 
#include <list>
#include<stack>
// we do not need ordered map , unordered map is enough for hashing 
#include <unordered_map>
using namespace std;


// LRU = Least Recently Used 
// In this Caching scheme we keep the recently used data on the top of cache 
// and the least recently used data is thrown out of cache 
// LRU Cache can be implemented using DOUBLY LINKED LIST and UNORDERED MAP 

//////////////////////////////////////////////////////////////////////
// Declaring a class for LRU cache 
class LRU_Cache{
    
    // everything in my class is public 
    public:
    
    // this cache stores pairs of (key, data)
    // integers are mapped to iterators of these pairs, allowing O(1) eviction
    list<pair<int, string>> L;
    unordered_map<int, list<pair<int, string>>::iterator> M;
    // capacity is the size of the cache , i.e. how many strings it can hold 
    int capacity ;
    
//////////////////////////////////////////////////////////////////////    
    // CONSTRUCTOR -->
    LRU_Cache(int cap) : capacity(0){
        // Capacity being less than 0 dosent make any sense ,
        // so setting warnings for the same 
        if(cap>0){
            capacity = cap;
        }
        else{
            cout<<"ERROR: LRU Capacity must be more than 0 , please reinitialise the cache"<<endl;
            cout<<"Random behaviour will take place since capacity is set to 0 "<<endl;
        }
        // clearing list and map when cache initialised 
        L.clear();
        M.clear();
    }
///////////////////////////////////////////////////////////////////
    // The METHOD SIZE of this class returns the capacity of the cache 
    int size() const {
        return capacity;
    }
////////////////////////////////////////////////////////////////    
    // The METHOD FEEDIN is used to feed the data to the cache 
    void feedin(int key , const string& data){
        if(capacity <= 0) return;
        
        // if key not present in cache already 
        if(M.find(key)==M.end()){
            // when cache is full then remove last then insert else just insert 
            if(capacity > 0 && L.size()==(size_t)capacity){
                // remove the last element first from map then from list
                int last_key = L.back().first;
                M.erase(last_key);
                
                // removing from list 
                L.pop_back();
            }
            // now insertion 
            L.push_front({key, data});
            M[key]=L.begin();
            return;
        }
        // key is present in cache already 
        else{
            // erase the already present data for that key in the list 
            L.erase(M[key]);
            // add the data to the list 
            L.push_front({key, data});
            // reassign the value of iterator in the map for that key 
            M[key]=L.begin();
            return;
        }
        
    }
////////////////////////////////////////////////////////////////////////// 
    // The METHOD GETTIN is used for getting the data for a certain key from LRU Cache 
    string gettin(int key) const {
        auto it = M.find(key);
        if(it==M.end()){
            // If the key dosent have a specific value to it 
            // then returns 0 
            return "0";
        }
        else{
            // returning the value of the key 
            return it->second->second;
        }
        
    }
    
};
//////////////////////////////////////////////////////////////////////////
int main()
{
    // Declaring a LRU Cache 
    LRU_Cache lru1(2);
    
    // Checking the size
    cout<<"The size of this LRU Cache is : " <<lru1.size()<<endl;
    
    // Adding data to it 
    lru1.feedin(1,"beta");
    lru1.feedin(3,"alpha");
    lru1.feedin(8,"gamma");
    // since capacity is 2 hence entry 1 goes out 
    
    // checking the data now 
    // only 3 and 8 are there in cache now 
    // since 1 went out hence shows 0 
    cout<<lru1.gettin(1)<<endl;
    cout<<lru1.gettin(3)<<endl;
    // 6 was never inserted so shows 0 
    cout<<lru1.gettin(6)<<endl;
    cout<<lru1.gettin(8)<<endl;
    
    return 0;
}
//////////////////////////////////////////////////////////////////////////

