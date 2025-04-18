import math

def to_seconds(value: str):
    values = list(map(int, value.split(':')))

    return values[0] * 3600 + values[1] * 60 + values[2]

while True:
    inputs = input().split()

    if inputs == ['-1']:
        break

    t1 = to_seconds(inputs[0])
    t2 = to_seconds(inputs[1])
    t3 = to_seconds(inputs[2])
    t4 = to_seconds(inputs[3])
    t5 = to_seconds(inputs[4])
    t = ( t5 * (t1 - t2) * (t4 - t3) - t2 * (t3 - t1) * (t5 - t4) ) / ( (t1 - t2) * (t4 - t3) - (t3 - t1) * (t5 - t4) )

    hour = int(t // 3600)
    minute = int((t - hour * 3600) // 60
)
    second = round(t - 3600 * hour - 60 * minute)

    print('{:02d}:{:02d}:{:02d}'.format(hour, minute, second))
