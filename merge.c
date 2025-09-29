#include <stdio.h>

void merge(int arr[], int low, int mid, int high){
  int i = low;
  int j = mid + 1;
  int k = 0;
  
  int temp[high - low + 1];
  
  while(i <= mid || j <= high){
     if(arr[i] <= arr[j]){
       temp[k++] = arr[i++];
     } else{
       temp[k++] = arr[j++];
     }
  }
  
  while(i <= mid){
    temp[k++] = arr[i++];
  }
  
  while(j <= high){
    temp[k++] = arr[j++];
  }
  
  for (int x = 0; x < k; x++) {
        arr[low + x] = temp[x];
  }    
}

void mergeSort(int arr[], int low, int high){
  if(low >= high){
    return;
  }
  int mid = (low + high)/2;
  mergeSort(arr, low, mid);
  mergeSort(arr, mid + 1, high); 
  merge(arr, low, mid, high);
}   

int main(){
  int n;
  int arr[];
  printf("Enter the size of array: ");
  scanf("%d", &n);
  for(int i = 0; i < n; i++){
     scanf("%d", &arr[i]);
  }
  return 0;
}