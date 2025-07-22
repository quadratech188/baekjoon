#include "../../modules/Matrix.h"
#include <algorithm>
#include <string>

void print1() {
	std::cout
		<< "#include <cstdio>\n"
		<< "int main(){\n"
		<< "    int N;\n"
		<< "    scanf(\"%d\",&N);\n"
		<< "    if(N==1){\n"
		<< "        puts(\"4\");\n"
		<< "    }\n";

	for (int i = 2; i <= 20000; i++)
		std::cout
			<< "    else if(N==" << i << "){\n"
			<< "        puts(\"" << 4 * i << "\");\n"
			<< "    }\n";

	std::cout
		<< "    else{\n"
		<< "        puts(\"Still working on it...\");\n"
		<< "    }\n"
		<< "    return 0;\n"
		<< "}\n";
}

void print2() {
	static std::string prefix1 = "BaBeBaBkBaBeBaB";
	static std::string prefix2 = "jojojojnjojojoj";
	static std::string prefix3 = "OnOlOnO";
	static std::string prefix4 = "inieini";
	static std::string prefix5 = "JuJ";
	static std::string final = "dgdedgd!dgdedgd";

	static std::string result = final;

	static std::string result5;
	for (auto ch: final) {
		result5 += prefix5;
		result5 += ch;
	}
	result5 += prefix5;

	static std::string result4;
	for (auto ch: result5) {
		result4 += prefix4;
		result4 += ch;
	}
	result4 += prefix4;

	static std::string result3;
	for (auto ch: result4) {
		result3 += prefix3;
		result3 += ch;
	}
	result3 += prefix3;

	static std::string result2;
	for (auto ch: result3) {
		result2 += prefix2;
		result2 += ch;
	}
	result2 += prefix2;

	for (auto ch: result2)
		std::cout << prefix1 << ch;
	std::cout << prefix1 << '\n';
}

static std::string last_row = R"( \     \/ /\ \/\   \/\   \ \/  \    /\   \/\/\   \ \/ / /   / / /\/ /\/\  / /  \   \/   /\/   /    \/ /\/\/  \    / / / /\/   /\ \ \   \/    \ \ \/ /\      /\/     /\  /\/ /\/\ \/  \ \  /\   \/ /\ \/\  /\/ /\/  \/\/   / / /\  /\/\ \ \/\ \ \/   /   / / / /  \ \   \/   /  \/\ \ \/\ \/ /\/\   \   \ \/\/  \/\/   /\/  \/ /\/   / /\/ /\/\ \ \ \ \ \/\/\   \/\  / /  \/\ \ \/\/\ \/  \/  \ \/\/\ \/  \/ /  \/  \/\/\/   /\/ / /\/  \ \  /\ \     \     \   \                                                                 \/\/ /  \ \/\/\      /\/ /  \ \ \/\  / /\  /\/\  / / /\/ /\ \ \/\/\     \/    \   \  /\/\ \/\/ /\/\ \/  \/\   \/\ \/\ \ \    /\ \    /\  /  \/  \/   /\ \  /\ \/    \ \/ /  \  / /\/  \/\/ / /  \ \/ /\/ / /\ \ \/\/\/\/\/\/\/\/\ \   \  / /  \/   /\  /      \/\/ /\    /      \  / /    \/\ \/  \  / /\ \/\/ /\  /\ \/ /\/\ \  /  \/ /     / /\/  \/   /    \  /   /\/ / /\/  \  / /    \ \/ /   /\  /    \/\/\  / / /\ \   \/\/ /\ \/\    / /  \ \/  \  /\/\  / /   /   /\  / /\/ /  \ \/  \/\ \     \ \/ / /\    /\ \ \/\ \/\ \  /\ \/\  /  \ \ \/    \ \ \ \/\/\  /\ \/\/\/\/  \ \/ / / /\ \/   / /\/\/\/\ \/ /\/\  /   /  \    /\/  \/ /\/\ \/\/\ \/\  / /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ /\ \   \  / /\/ /\   \/\ \  / / /  \ \/\ \ \/   / /  \/     /\ \  /\/    \ \ \ \/\     \/ /\ \/ / /\/   / /   /\  / / /  \ \/\/\/   /  \/  \   \/\/    \ \/\ \/ / /\/  \/    \/\ \ \/   /  \  / /\  /\ \      /\/ / /\/ / / / /    \  / / /\  /\ \/\/ /\ \/\/ / /\ \ \/    \/          \     \ \/ /  \/\/\/\ \   \/\/   /     /\     \   \ \ \ \/\/     /\/   /\/\ \/ / /\/ /\  /  \  /\/\/\ \/\ \ \/\ \  / / / /\    /  \/ /     /  \/  \ \ \/\ \      /\ \  / /\/ /   /\ \ \/ /  \/  \/ /\/  \ \/\/   /    \ \/  \ \ \/ /\ \ \/ /\/  \/\ \/\/\    /  \ \ \/ /\/ /  \ \ \/\/\  /  \/\/\  /     / /\/\/  \     \/\ \/ /  \ \/ / /\  /\/\/\ \/\/     /\ \   \/\/\/   / /\/\ \/\/\ \/\/\ \ \/\  /  \/ /\ \ \/ / /\     \ \ \/ /\ \/   /    \  / /\/\  /\/     /  \  /\/\  /\/\/  \/ / /\/\/\    /\ \   \/\/\ \/\    /\/  \/\/    \   \ \ \/\/ /\/ /\/\         \)";

void solve3(Matrix<char>& buffer, Int2 start, char ch, int length) {
	int range_start, range_end;
	if (ch == '/') {
		range_start = start.x - 1024 + start.y + 1;
		range_end = start.x + 1;
	}
	else {
		range_start = start.x;
		range_end = range_start + 1024 - start.y;

	}

	bool stop = true;
	for (int i = range_start; i < range_end; i++) {
		if (last_row[i] != ' ') stop = false;
	}
	if (stop) return;

	bool recurse = true;
	if (length == 0) {
		length = 1;
		recurse = false;
	}

	Int2 dir;
	if (ch == '/') dir = Int2(-1, 1);
	else dir = Int2(1, 1);

	for (int i = 0; i < length; i++) {
		buffer[start + dir * i] = ch;
	}

	if (!recurse) return;

	solve3(buffer, start + dir * length, ch, length / 2);
	solve3(buffer, start + dir * (length - 1) + Int2::j(), ch == '/'? '\\': '/', length / 2);
}

void print3() {
	static Matrix<char> buffer(2048, 1024, ' ');

	solve3(buffer, Int2(1023, 0), '/', 512);
	solve3(buffer, Int2(1024, 0), '\\', 512);

	for (int j = 0; j < 1024; j++) {
		for (int i = 0; i < 2048; i++)
			std::cout << buffer(i, j);
		std::cout << '\n';
	}
}

static std::string repeating = R"(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)";

void print5() {
	for (int i = 0; i < 200; i++)
		std::cout << repeating;
	std::cout << '\n';
}

int main(int argc, char** argv) {
	int n;
#ifndef ONLINE_JUDGE
	if (argc == 0)
		std::cin >> n;
	else
		n = std::stoi(argv[1]);
#else
	std::cin >> n;
#endif

	switch(n) {
		case 0:
			std::cout << "BOJ 20000\n";
			break;
		case 1:
			print1();
			break;
		case 2:
			print2();
			break;
		case 3:
			print3();
			break;
		case 5:
			print5();
			break;
	}
}
