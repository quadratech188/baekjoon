import sys

n = int(input())

sys.setrecursionlimit(100000)

in_order = list(map(int, input().split())) # left - root - right
post_order = list(map(int, input().split())) # left - right - root


def solution(io_start: int, io_end: int, po_start, po_end):
    if io_end - io_start == 1:
        print(in_order[io_start], end=' ')
        return

    if io_end - io_start <= 0:
        return

    root = post_order[po_end - 1]

    in_order_root = in_order.index(root)

    # root
    print(root, end=' ')
    # left
    solution(io_start, in_order_root, po_start, po_start - io_start + in_order_root)
    # right
    solution(in_order_root + 1, io_end, po_start - io_start + in_order_root, po_end - 1)

solution(0, n, 0, n)
