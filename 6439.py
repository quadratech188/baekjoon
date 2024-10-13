def main():
    x_start, y_start, x_end, y_end, x_left, y_top, x_right, y_bottom = tuple(map(int, input().split()))

    # Line: (y_end - y_start)(x - x_start) - (x_end - x_start)(y - y_start) = 0
    # Perpendicular_1: (x_end - x_start)(x - x_start) + (y_end - y_start)(y - y_start) = 0
    # Perpendicular_2: (x_end - x_start)(x - x_end) + (y_end - y_start)(y - y_end) = 0

    points = [
            (x_left, y_top),
            (x_left, y_bottom),
            (x_right, y_top),
            (x_right, y_bottom)
    ]
    locations = []

    for point in points:
        x, y = point
        a = (y_end - y_start) * (x - x_start) - (x_end - x_start) * (y - y_start)
        b = (x_end - x_start) * (x - x_start) + (y_end - y_start) * (y - y_start)
        c = (x_end - x_start) * (x - x_end) + (y_end - y_start) * (y - y_end)
        locations.append((a, b, c))
   
    # All above/below perpendicular lines
    if all(i[1] > 0 and i[2] > 0 for i in locations): return False
    if all(i[1] < 0 and i[2] < 0 for i in locations): return False
    
    # All above/below original line
    if all(i[0] < 0 for i in locations): return False
    if all(i[0] > 0 for i in locations): return False
    
    return True

n = int(input())

for _ in range(n):
    print('T' if main() else 'F')

