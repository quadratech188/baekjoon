n = int(input())

a = list(map(int, input().split()))
a_indices = [0] * len(a)
for i, v in enumerate(a):
    a_indices[v] = i

b = list(map(int, input().split()))
b_indices = [0] * len(b)
for i, v in enumerate(b):
    a_indices[v] = i

size = len(a)

def solution(start, end):
    if end - start == 1:
        if a[start] == b[start]:
            return 1
        else:
            return 0
    center = (start + end) // 2
    
    # For two nodes to satisfy the criteria, they have to either
    # 1) belong to different children in both a and b, or
    # 2) belong to the same children in both a and b

    # In case 1), no further checks are needed
    result = solution(start, center) + solution(center, end)

    # In case 2) 

    return result

solution(0, size)
