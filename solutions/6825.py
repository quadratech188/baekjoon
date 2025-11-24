w = float(input())
h = float(input())
bmi = w / h ** 2

if 25 < bmi:
    print('Overweight')
elif 18.5 <= bmi <= 25:
    print('Normal weight')
else:
    print('Underweight')
