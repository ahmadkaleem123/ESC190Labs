


def find(L,e):
    n = len(L)
    r = n-1
    l = 0
   
    while(l<=r):
        mid = (l + r) // 2
        if(e <= L[mid]):
            r = mid-1


        else:
            l = mid +1
    if(L[mid] == e):
        return mid
    elif(mid + 1 != n and L[mid + 1] == e):
        return mid + 1
   
    else:
        return -1

    
def find_max(L,e):
    n = len(L)
    r = n-1
    l = 0
   
    while(l<=r):
        mid = (l + r) // 2
        if(e >=  L[mid]):
            l = mid +1
          


        else:
            r = mid-1
           
    if(L[mid] == e):
        return mid
    if(mid - 1 != -1 and L[mid -1] == e):
        return mid - 1
   
    else:
        return -1


        


if __name__ == "__main__":
    x = [1,3,3, 5]
    print(find_max(x,2))