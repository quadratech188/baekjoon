while True:
    m = float(input())
    if m < 0:
        break

    print(f'Objects weighing {m:.2f} on Earth will weigh {0.167 * m:.2f} on the moon.')
