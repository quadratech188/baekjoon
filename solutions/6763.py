a = int(input())
b = int(input())

def c(x):
    if x <= 20: return 100
    elif x <= 30: return 270
    else: return 500

if b <= a:
    print('Congratulations, you are within the speed limit!')
else:
    print(f'You are speeding and your fine is ${c(b - a)}.')
