#include "modules/SegmentTree.h"
#include "modules/ModInt.h"

typedef ModInt<int, 1000000007> modint;

struct Data {
	modint r, o, c, k;
	modint ro, oc, ck;
	modint roc, ock;
	modint rock;

	Data operator+(const Data other) const {
		return {
			.r = this->r + other.r,
			.o = this->o + other.o,
			.c = this->c + other.c,
			.k = this->k + other.k,
			.ro = this->ro + this->r * other.o + other.ro,
			.oc = this->oc + this->o * other.c + other.oc,
			.ck = this->ck + this->c * other.k + other.ck,
			.roc = this->r * other.oc + this->ro * other.c,
			.ock = this->o * other.ck + this->oc * other.k,
			.rock = this->rock + this->r * other.ock + this->ro * other.ck + this->roc * other.k + other.rock
		};
	}
};
