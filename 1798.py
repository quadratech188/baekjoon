import math
while True:
    try:
        r, h, d_1, A_1, d_2, A_2 = tuple(map(float, input().split()))
    except:
        break

    large_radius = (r * r + h * h) ** 0.5
    angle = abs(A_1 - A_2) % 360
    if angle > 180:
        angle = 360 - angle

    angle = angle * r / large_radius

    print("{:.2f}".format((d_1 * d_1 + d_2 * d_2 - 2 * d_1 * d_2 * math.cos(angle * math.pi / 180)) ** 0.5))


