#include <iostream>
#include <vector>
#define ll long long

using namespace std;

namespace fio
{
    //read
    const int SIZE = 1 << 20;
    char arBuffer[SIZE]{};
    int nReadIndex = SIZE;

    inline char ReadChar()
    {
        if (nReadIndex == SIZE)
        {
            fread(arBuffer, 1, SIZE, stdin);
            nReadIndex = 0;
        }

        return arBuffer[nReadIndex++];
    }

    inline int ReadInt()
    {
        char cRead = ReadChar();

        while ((cRead < 48 || cRead > 57) && cRead != '-')
            cRead = ReadChar();

        int nRes = 0;
        bool bNeg = (cRead == '-');

        if (bNeg)
            cRead = ReadChar();

        while (cRead >= 48 && cRead <= 57)
        {
            nRes = nRes * 10 + cRead - 48;
            cRead = ReadChar();
        }

        return bNeg ? -nRes : nRes;
    }

    //inline int ReadString(char* strRead)
    //{
    //    char cRead = ReadChar();
    //    int nLength = 0;

    //    while (cRead <= 32)
    //        cRead = ReadChar();

    //    while (cRead >= 48)
    //    {
    //        strRead[nLength++] = cRead;
    //        cRead = ReadChar();
    //    }

    //    strRead[nLength++] = '\0';

    //    return nLength - 1;
    //}

    //write
    char arWBuffer[SIZE]{};
    int nWriteIndex;

    inline int GetSize(int nWrite)
    {
        if (nWrite < 0)
            nWrite = -nWrite;

        int nSize = 1;

        while (nWrite >= 10)
        {
            nSize++;
            nWrite /= 10;
        }

        return nSize;
    }

    inline void Flush()
    {
        fwrite(arWBuffer, 1, nWriteIndex, stdout);
        nWriteIndex = 0;
    }

    inline void WriteChar(char cWrite)
    {
        if (nWriteIndex >= SIZE)
            Flush();

        arWBuffer[nWriteIndex++] = cWrite;
    }

    inline void WriteInt(int nWrite)
    {
        int nSize = GetSize(nWrite);

        if (nWriteIndex + nSize >= SIZE)
            Flush();

        if (nWrite < 0)
        {
            nWrite = -nWrite;
            arWBuffer[nWriteIndex++] = '-';
        }

        int nNext = nWriteIndex + nSize;

        while (nSize--)
        {
            arWBuffer[nSize + nWriteIndex] = nWrite % 10 + 48;
            nWrite /= 10;
        }

        nWriteIndex = nNext;
        WriteChar('\n');
    }

    //inline void WriteString(char* strWrite)
    //{
    //    int nSize = strlen(strWrite);

    //    if (nWriteIndex + nSize + 1 >= SIZE)
    //        Flush();

    //    int nIndex = 0;

    //    while (nSize--)
    //        arWBuffer[nWriteIndex++] = strWrite[nIndex++];

    //    WriteChar('\n');
    //}
}

const ll MOD = 1000000007;
const int ARRSIZE = 100001;
vector<pair<int, int>> vTree[ARRSIZE]{};
ll nAns;

bool Solve();
ll Dfs(int nCur, int nParent);
int GetMax(int nData1, int nData2);
int GetMin(int nData1, int nData2);
int GetAbs(int nData);

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	Solve();
}

bool Solve()
{
	bool bReturn = false;
	int N, A, B, W, nIter;

	do
	{
        N = fio::ReadInt();

		nIter = N - 1;

		while (nIter--)
		{
            A = fio::ReadInt();
            B = fio::ReadInt();
            W = fio::ReadInt();

			vTree[A].push_back({ B, W });
			vTree[B].push_back({ A, W });
		}

		nAns = 0;
		Dfs(1, 0);
		cout << nAns;

		bReturn = true;
	} 
    while (false);

	return bReturn;
}

ll Dfs(int nCur, int nParent)
{
	ll nSum, nChildSum;

	nSum = 1;

	for (auto prNext : vTree[nCur])
	{
		if (prNext.first == nParent)
			continue;

		nChildSum = Dfs(prNext.first, nCur) * prNext.second % MOD;
		nAns = (nAns + nSum * nChildSum) % MOD;
		nSum = (nSum + nChildSum) % MOD;
	}

	return nSum;
}

int GetMax(int nData1, int nData2)
{
	return nData1 < nData2 ? nData2 : nData1;
}

int GetMin(int nData1, int nData2)
{
	return nData1 > nData2 ? nData2 : nData1;
}

int GetAbs(int nData)
{
	return nData < 0 ? -nData : nData;
}

