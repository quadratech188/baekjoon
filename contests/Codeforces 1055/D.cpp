/*
 *
 * ptr = [0, 0, 0, 0, 0]...
 *
 * Poby: ptr[i] ++
 * Rekkles: b[i] += (2 ^ ptr[i])
 *
 * Target: max_bit(b[i]) == ptr[i]
 *
 * [4, 3]:
 * 1
 * 0 1
 * 0 1
 *
 * [3, 2, 5]:
 *     1
 * 1 1 0
 * 1 0 1
 *
 *
 * f(range, poby_cnt, rekkles_cnt) = max(
 * 	
 * )
