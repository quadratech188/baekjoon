def main():
    n = int(input())

    power_of_2 = 1

    average = 0

    while power_of_2 <= n:
        remainder = n % (2 * power_of_2)

        if remainder < power_of_2:
            difference = -remainder
        else:
            difference = remainder - 2 * power_of_2

        probability = (n + difference) / 2 / n

        average += power_of_2 * (2 * probability - 2 * probability ** 2)
        power_of_2 *= 2

    print(average)

for _ in range(int(input())):
    main()
