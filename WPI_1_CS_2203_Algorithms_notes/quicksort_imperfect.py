def quick_sort(arr, min=0,max=0):
    if(max == 0):
        max = arr.len() -1
    if(min >= max):
        # one element to sort
        return
    else:
        pivot = partition(arr, min, max)
        quick_sort(arr,min, pivot-1)
        quick_sort(arr, pivot+1,max)
        
    return arr

def partition(arr,min,max):
    pivot = arr[max]
    
    before_pivot = min - 1
    
    for j in range(min,max-1):
        if(arr[j] <= pivot):
            before_pivot += 1
            
            temp=arr[before_pivot]
            arr[before_pivot] = arr[j]
            arr[j] = temp
    
    temp = arr[before_pivot+1]
    arr[before_pivot+1] = arr[max]
    arr[max] = temp
    
    return before_pivot + 1


print quick_sort([2,5,7,1,4,3],0)