#include "modules/SegmentTree.h"

struct Sequence {
	int start;
	int length;
	int end;
};

struct Data {
	int length;
	Sequence inc;
	Sequence dec;
	Sequence inc_dec;
};
