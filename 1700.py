n, k = tuple(map(int, input().split()))

appliances = list(map(int, input().split()))

current_appliances = set()

count = 0

for index, appliance in enumerate(appliances):
    if appliance in current_appliances:
        continue

    if len(current_appliances) < n:
        current_appliances.add(appliance)
        continue

    m = 0
    m_i = 0

    for used_appliance in current_appliances:
        try:
            last_used = appliances.index(used_appliance, index + 1)
        except:
            last_used = k

        if last_used >= m:
            m = last_used
            m_i = used_appliance

    current_appliances.remove(m_i)

    count += 1

    current_appliances.add(appliance)


print(count)
