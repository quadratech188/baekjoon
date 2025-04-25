#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef tuple<int, int, int> tiii;

#define F first
#define S second
const int MAXN = 100001;
int N, M;
vector<pii> graph[MAXN];
pii edge[MAXN];

int SubSize[MAXN];
int DFS_cnt = 1;
int DFS_num[MAXN];
int chain_lv[MAXN], chain_head[MAXN], chain_par[MAXN];
//lv : chain의 깊이, head : chain을 대표하는 노드 번호, par : chain의 부모 노드

int segTree[MAXN*4];

void update(int s, int e, int m, int idx, int val)
{
    if(m < s || e < m) return;
    if(s == e)
    {
        segTree[idx] = val;
        return;
    }
    
    update(s, (s+e)/2, m, idx*2, val);
    update((s+e)/2+1, e, m, idx*2+1, val);
    segTree[idx] = max(segTree[idx*2], segTree[idx*2+1]);
}

int getVal(int s, int e, int l, int r, int idx)
{
    if(e < l || r < s) return 0;
    if(l <= s && e <= r) return segTree[idx];
    
    return max(getVal(s, (s+e)/2, l, r, idx*2), getVal((s+e)/2+1, e, l, r, idx*2+1));
}

void buildSubSize(int u, int p) //각 노드를 루트로 하는 트리의 크기를 구한다.
{
    SubSize[u] = 1;
    for(auto& child : graph[u])
    {
        auto [v, d] = child;
        if(v == p) continue; //자식 노드가 부모 노드면 continue;
        
        buildSubSize(v, u);
        SubSize[u] += SubSize[v]; //자식의 서브트리의 크기를 구해 더한다.
        
        //서브트리의 크기가 가장 큰 자식을 swap을 이용해 첫 번째 자식으로 둔다.
        //auto&를 사용한 이유가 swap을 이용하기 위함이다.
        if(SubSize[v] > SubSize[graph[u].front().F] || graph[u].front().F == p)
            swap(graph[u].front(), child);
    }
}

void DFS(int u, int p, int lv)
{
    DFS_num[u] = DFS_cnt++;
    chain_lv[u] = lv;
    
    bool isFirst = true;
    for(auto [v, d] : graph[u])
    {
        if(v == p) continue;
        
        if(isFirst) //v가 서브트리의 크기가 가장 큰 노드라면, u와 v가 같은 chain으로 묶인다.
        {
        	//chain의 대표 번호, chain의 부모 노드는 u, v가 서로 같다.
            chain_head[v] = chain_head[u];
            chain_par[v] = chain_par[u];
            DFS(v, u, lv); //chain이 같기 때문에 lv도 증가하지 않는다.
            isFirst = false;
        }
        else //그렇지 않다면 u와 v는 서로 다른 chain에 속한다. 즉 v부터는 새로운 chain을 생성한다.
        {
            chain_head[v] = v; //v는 새로운 chain의 대표 번호가 된다.
            chain_par[v] = u; //chain v의 부모 노드는 u이다.
            DFS(v, u, lv+1); //chain이 다르기에 lv이 1 증가한다.
        }
        
        update(1, N, DFS_num[v], 1, d); //segTree의 DFS_num[v]에 d를 저장한다.
    }
}

int main()
{
	ios::sync_with_stdio(0);
	cin.tie(0), cout.tie(0);

    cin >> N;
    for(int i=1; i<N; i++)
    {
        int u, v, w; cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
        edge[i] = {u, v};
    }
    
    buildSubSize(1, 0); 
    
    chain_head[1] = 1;
    DFS(1, 0, 1);
    
    cin >> M;
    while(M--)
    {
        int q; cin >> q;
        if(q == 1)
        {
            int i, c; cin >> i >> c;
            auto [u, v] = edge[i];
            if(DFS_num[u] > DFS_num[v]) swap(u, v);
            update(1, N, DFS_num[v], 1, c); //segTree의 DFS_num[v]에 c를 저장한다.
        }
        else
        {
            int u, v; cin >> u >> v;
            if(chain_lv[u] > chain_lv[v]) swap(u, v);
            
            int ans = 0;
            //두 노드의 chain의 깊이를 동일하게 맞춘다.
            while(chain_lv[u] < chain_lv[v])
            {
            	//v가 속한 chain의 대표 번호에서 v까지 간선 가중치의 최댓값을 구한다.
                ans = max(ans, getVal(1, N, DFS_num[chain_head[v]], DFS_num[v], 1));
                v = chain_par[v];
            }
            
            //두 노드가 속한 chain이 동일할 때까지 while 문을 실행한다.
            while(chain_head[u] != chain_head[v])
            {
                ans = max(ans, getVal(1, N, DFS_num[chain_head[u]], DFS_num[u], 1));
                ans = max(ans, getVal(1, N, DFS_num[chain_head[v]], DFS_num[v], 1));
                u = chain_par[u];
                v = chain_par[v];
            }
            
            //두 노드가 속한 chain이 같다면, 두 노드 사이의 간선의 가중치의 최댓값을 구한다.
            if(DFS_num[u] > DFS_num[v]) swap(u, v);
            ans = max(ans, getVal(1, N, DFS_num[u] + 1, DFS_num[v], 1));
            
            cout << ans << "\n";
        }
    }
}
