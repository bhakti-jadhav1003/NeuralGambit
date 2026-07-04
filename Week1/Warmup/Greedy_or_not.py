n=int(input())
arr=[]
for i in range(n):
  arr.append(int(input()))
if n==0:
  print("tie")



def score(l,r):
  if l==r:
    return arr[l]
  else:
    take_left=arr[l]-score(l+1,r)#my net advantage is my choice-opponents choice
    take_right=arr[r]-score(l,r-1)
    return(max(take_left,take_right))#get the max of the two whichever gives a higher diff score 
diff=score(0,n-1)#final diff of score of players
if(diff>0):
  print("Player 1 wins by",diff)
elif(diff<0):
  print("Player 2 wins by",-diff)
else:
  print("tie")




    
        
    
    































