// **************************************************************************
//  File       [sort_tool.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The implementation of the SortTool Class]
//  Modify     [2020/9/15 Mu-Ting Wu]
//  Modify     [2023/9/12 Ming-Bang Fan]
// **************************************************************************

#include "sort_tool.h"
#include<iostream>
#include<climits>
#include<cstdlib>
#define ll long long 
// Constructor
SortTool::SortTool() {}

// Insertsion sort method
void SortTool::InsertionSort(vector<int>& data) {
    // Function : Insertion sort
    // TODO : Please complete insertion sort code here
    int key,i;
    for(int j=1;j<data.size();j++){
        key = data[j];
        i = j-1;
        while(i>=0 && data[i]>key){
          data[i+1] = data[i];
          i--;
        }
        data[i+1] = key;
    }  
}

// Quick sort method
void SortTool::QuickSort(vector<int>& data, int f){
    QuickSortSubVector(data, 0, data.size() - 1, f);
}
// Sort subvector (Quick sort)
void SortTool::QuickSortSubVector(vector<int>& data, int low, int high, const int flag) {
    // Function : Quick sort subvector
    // TODO : Please complete QuickSortSubVector code here
    if(low < high){
        if(flag == 0){
            int q = Partition(data,low,high);
            QuickSortSubVector(data,low,q,0);
            QuickSortSubVector(data,q+1,high,0);
        }
        else if(flag == 1){
            int q = RandomizedPartition(data,low,high);
            QuickSortSubVector(data,low,q,1);
            QuickSortSubVector(data,q+1,high,1);       
        }        
    }
    // Hint : recursively call itself
    //        Partition function is needed
    // flag == 0 -> normal QS
    // flag == 1 -> randomized QS
}
int SortTool::RandomizedPartition(vector<int>& data, int low, int high){

    // Function : RQS's Partition the vector 
    // TODO : Please complete the function
    int random = rand() % (high - low +1) + low;
    swap(data[low],data[random]);
    return Partition(data,low,high);

}

int SortTool::Partition(vector<int>& data, int low, int high) {
    // Function : Partition the vector 
    // TODO : Please complete the function
    int x = data[low];
    int i = low-1, j = high+1;
    while(1){
        do{
            i++;
        }while(data[i] < x);
        do{
            j--;
        }while(data[j] > x);
        if(i < j){
            swap(data[i],data[j]);
        }
        else
            return j;
    }
}

// Merge sort method
void SortTool::MergeSort(vector<int>& data){
    MergeSortSubVector(data, 0, data.size() - 1);
}

// Sort subvector (Merge sort)
void SortTool::MergeSortSubVector(vector<int>& data, int low, int high) {

    // Function : Merge sort subvector
    // TODO : Please complete MergeSortSubVector code here
    if(low < high){
        int middle = (low+high)/2;
        MergeSortSubVector(data,low,middle);
        MergeSortSubVector(data,middle+1,high);
        Merge(data,low,middle,middle+1,high);
    }
    // Hint : recursively call itself
    //        Merge function is needed

}

// Merge
void SortTool::Merge(vector<int>& data, int low, int middle1, int middle2, int high) {
    // Function : Merge two sorted subvector
    // TODO : Please complete the function

    int length1 = middle2 - low;
    int length2 = high - middle1;
    int leftSubarr[length1] = {};
    int rightSubarr[length2]= {};

//    leftSubarr[length1] = INT_MAX;
//    rightSubarr[length2] = INT_MAX;

    for(int i=0;i<length1;i++)
        leftSubarr[i] = data[low+i];
    for(int i=0;i<length2;i++)
        rightSubarr[i] = data[middle2+i];

    int i = 0, j = 0, k = low;
    while(i<length1 && j<length2){
        if(leftSubarr[i] <= rightSubarr[j]){
            data[k] = leftSubarr[i];
            i++;
        }
        else{
            data[k] = rightSubarr[j];
            j++;
        }
		    k++;
    }
	
	  while(i < length1){
  	    data[k] = leftSubarr[i];
  	    i++;
  	    k++;
    }

  	while(j < length2){
  	    data[k] = rightSubarr[j];
  	    j++;
  	    k++;
	  }

}

// Heap sort method
void SortTool::HeapSort(vector<int>& data) {
    // Build Max-Heap
    BuildMaxHeap(data);
    // 1. Swap data[0] which is max value and data[i] so that the max value will be in correct location
    // 2. Do max-heapify for data[0]
    for (int i = data.size() - 1; i >= 1; i--) {
        swap(data[0],data[i]);
        heapSize--;
        MaxHeapify(data,0);
    }
}

//Max heapify
void SortTool::MaxHeapify(vector<int>& data, int root) {

    // Function : Make tree with given root be a max-heap if both right and left sub-tree are max-heap
    // TODO : Please complete max-heapify code here
    int largest = root;
    int l = 2 * root + 1;
    int r = 2 * root + 2;
 
    if (l < heapSize && data[l] > data[largest])
        largest = l;
 
    if (r < heapSize && data[r] > data[largest])
        largest = r;
 
    if (largest != root) {
        swap(data[root], data[largest]);
        MaxHeapify(data,largest);
    }

}

//Build max heap
void SortTool::BuildMaxHeap(vector<int>& data) {

    heapSize = data.size(); // initialize heap size
    // Function : Make input data become a max-heap
    // TODO : Please complete BuildMaxHeap code here
    for(int i=heapSize/2-1;i>=0;i--){
        MaxHeapify(data,i);
    }
}