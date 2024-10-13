n = int(input())

digits = []

power_of_2 = 1

n_copy = n

while n_copy != 0:
    if n_copy % (2 * power_of_2) != 0:
        digits.append(1)
        n_copy -= power_of_2
    else:
        digits.append(0)

    power_of_2 *= 2

print(digits);

below = None

if 0 not in digits:
    below = 0
    above = n + 2 ** (len(digits) - 1)

for i in range(0, len(digits)):
    if digits[i] == 1:
        one_index = i
        
        for j in range(i - 1, -1, -1):
            if digits[j] == 0:
                zero_index = j
                break
        else:
            continue

        below = n + 2 ** zero_index - 2 ** one_index
        break

else:
    below = 0

above = None

for i in range(0, len(digits)):
    if digits[i] == 0:
        zero_index = i
        
        for j in range(i - 1, -1, -1):
            if digits[j] == 1:
                one_index = j
                break
        else:
            continue

        above = n + 2 ** zero_index - 2 ** one_index
        break

else:
    above = 0
    
print(below, above)
