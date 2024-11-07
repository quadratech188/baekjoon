for m in range(int(input())):
 s=''.join(list(filter(lambda c: c.isalpha() and c.islower() or c in '|&^~()', input())))
 v={c for c in s if c.isalpha()}
 h=''.join(f'{c}=1\n' for c in v)
 for i in range(len(s)):
  l=s[:i]
  r=s[i:]
  try:
   exec(h+l)
   exec(h+r)
  except:
   continue
  else:
   break
 for u in (format(i,f'0{len(v)}b') for i in range(len(v))):
  h=''.join(f'{c}={u[j]}\n' for j,c in enumerate(v))
  q={}
  exec(h+'v1='+l,{},q)
  exec(h+'v2='+r,{},q)
  if q['v1']!=q['v2']:
   print(f'Data set {m + 1}: Different')
   break
 else:
  print(f'Data set {m + 1}: Equivalent')
