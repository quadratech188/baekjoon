(import (chicken foreign))

(foreign-declare
"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <ctype.h>

#define FASTISTREAM_BUFFER_SIZE (1 << 20)

static char buffer[FASTISTREAM_BUFFER_SIZE];
static char *buf_ptr = buffer;
static char *buf_end = buffer;

static inline char fast_getchar(void) {
    if (buf_ptr == buf_end) {
        ssize_t bytes_read = read(STDIN_FILENO, buffer, FASTISTREAM_BUFFER_SIZE);
        if (bytes_read <= 0) {
            return EOF;
        }
        buf_ptr = buffer;
        buf_end = buffer + bytes_read;
    }
    return *(buf_ptr++);
}

int fast_read_uint() {
    int x = 0;
    char c;
    do { c = fast_getchar(); } while (!isdigit(c));
    while (isdigit(c)) {
        x = 10 * x + (c - '0');
        c = fast_getchar();
    }
    return x;
}

int64_t fast_read_int64() {
    int64_t x = 0;
    int neg = 0;
    char c;
    do { c = fast_getchar(); } while (isspace(c));
    if (c == '-') { neg = 1; c = fast_getchar(); }
    while (isdigit(c)) {
        x = 10 * x + (c - '0');
        c = fast_getchar();
    }
    return neg ? -x : x;
}

char fast_read_char() {
    char c;
    do { c = fast_getchar(); } while (isspace(c));
    return c;
}

#define MAXN 1000000
#define TREE_SIZE (4 * MAXN)

typedef struct {
    size_t start, end;
} Segment;

static inline size_t segment_size(Segment s) {
    return s.end - s.start;
}

static inline size_t segment_center(Segment s) {
    return (s.start + s.end) / 2;
}

static inline Segment segment_left(Segment s) {
    Segment result = { s.start, segment_center(s) };
    return result;
}

static inline Segment segment_right(Segment s) {
    Segment result = { segment_center(s), s.end };
    return result;
}

static inline int segment_includes(Segment outer, Segment inner) {
    return outer.start <= inner.start && inner.end <= outer.end;
}

int64_t tree[TREE_SIZE];
size_t input_size;

void init(Segment seg, size_t index) {
    if (segment_size(seg) == 1) {
		tree[index] = fast_read_int64();
        return;
    }

    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    init(segment_left(seg), left);
    init(segment_right(seg), right);

    tree[index] = tree[left] + tree[right];
}

int64_t query(Segment query_seg, Segment seg, size_t index) {
    if (segment_includes(query_seg, seg)) {
        return tree[index];
    }

    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;

    if (segment_center(seg) <= query_seg.start)
        return query(query_seg, segment_right(seg), right);
    if (query_seg.end <= segment_center(seg))
        return query(query_seg, segment_left(seg), left);

    return query(query_seg, segment_left(seg), left) +
           query(query_seg, segment_right(seg), right);
}

void update(size_t idx, size_t tree_idx, Segment seg, int64_t value) {
    if (segment_size(seg) == 1) {
        tree[tree_idx] = value;
        return;
    }

    size_t left = 2 * tree_idx + 1;
    size_t right = 2 * tree_idx + 2;

    if (idx < segment_center(seg))
        update(idx, left, segment_left(seg), value);
    else
        update(idx, right, segment_right(seg), value);

    tree[tree_idx] = tree[left] + tree[right];
}

int sol() {
    int n = fast_read_uint();
    int m = fast_read_uint();
    int k = fast_read_uint();
    input_size = n;

    init((Segment){0, input_size}, 0);

    for (int i = 0; i < m + k; ++i) {
        char op = fast_read_char();
        if (op == '1') {
            size_t idx = fast_read_uint() - 1;
            int64_t val = fast_read_int64();
            update(idx, 0, (Segment){0, input_size}, val);
        } else if (op == '2') {
            size_t l = fast_read_uint() - 1;
            size_t r = fast_read_uint();
            printf(\"%ld\\n\", query((Segment){l, r}, (Segment){0, input_size}, 0));
        }
    }
    return 0;
}
  ")

(foreign-code "sol();")
