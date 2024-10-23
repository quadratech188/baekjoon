n=int(input())
p=input().split()
u=set()
c=0
for i in range(n):
 if i not in u:
  c+=1
  for _ in range(n):u.add(i);i=int(p[i])
print(0 if c==1 else c)
