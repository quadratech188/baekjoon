
n = int(input())
# Left - Root - Right
in_order = [int(x) for x in input().split()]
# Left - Right - Root
post_order = [int(x) for x in input().split()]

# Root - Left - Right

def pre_order(in_order_start, post_order_start, length):
    in_order_root = in_order_start
    post_order_root = post_order_start
    while True:
        if in_order[in_order_root] != post_order[post_order_root]:
            break
        in_order_root += 1
        post_order_root += 1
    length = 0
    while True:
        if in_order[in_order_root] == post_order[post_order_root]:
            break
        post_order_root += 1
        length += 1
    
    # Root
    print(in_order[in_order_root])

    # Left
    pre_order(in_order_start, )
    # Left
    for i in range(in_order_start, in_order_root):
        print(in_order[i])

    # Right
    pre_order(in_order_root + 1, post_order_root)

pre_order(0, 0)
