arr=[-5,5,9,-4,10,-9,0,3,-10]
l=arr[0]
r=arr[8]
n=len(arr)

def score(l,r):
  if l==r:
    return arr[l]
  else:
    take_left=arr[l]-score(l+1,r)
    take_right=arr[r]-score(l,r-1)
    return(max(take_left,take_right))#each player tries to maximise their score
diff=score(0,n-1)#final diff of score of players
if(diff>0):
  print("Player 1 wins by",diff)
elif(diff<0):
  print("Player 2 wins by",-diff)
else:
  print("tie")
