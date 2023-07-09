#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include <queue>

#define MAX_WEIGHT 10000000   
#define MAX 1000
#define PARENT(i) (i-1)/2       //父节点    构建堆
#define LEFT(i) 2*i+1         //左孩子
#define RIGHT(i) 2*i+2      //右孩子

using namespace std;

int D[MAX][MAX];    // 所有结点对的最短路径距离
int Pre[MAX][MAX];  // 记录前驱
bool no_negative_weight_cycle = true;	// 标志是否有负环 

typedef struct vertex{ 
    int num;        // num存储顶点编号
    int d;          // 最短路径估计值
    int id;       // 用于DIJKSTRA算法中, 记录次序 
    vertex *p;      // 前驱顶点
    vector<pair<int, int> >edges; // 邻接表,第一个 int 为顶点编号, 同时也对应着在图G.V容器中的存储下标;第二个 int 为边的权值
}Vertex;

typedef struct graph{   // 有向图
    vector<Vertex *>V;  // 图结构中，存储结点的容器
}Graph; 

void RELAX(Vertex *u, Vertex *v, int w){
    if (v->d > u->d + w){
        v->d = u->d + w;
        v->p = u;
    }
    return;
}

void INITIALIZE_SINGLE_SOURCE(Graph &G, Vertex *s){    // Initialization
    // Vertex *v;
    for (auto v : G.V){
        v->d = MAX_WEIGHT;
        v->p = nullptr;
    }
    s->d = 0;
}

bool BELLMAN_FORD(Graph &G, Vertex *s){
    int i;
    // Vertex *u;
    INITIALIZE_SINGLE_SOURCE(G, s);
    for (i = 1; i < G.V.size(); i++)
        for (auto u : G.V)             // 相比于课本而言多了一个for, 这是因为采用邻接表存储, 要遍历所有顶点才能获得所有边
            for (auto edge : u->edges)
                RELAX(u, G.V[edge.first], edge.second);
    for (auto u : G.V)
        for (auto edge : u->edges)
            if (G.V[edge.first]->d > u->d + edge.second)
                return false;
    return true;
}

// struct compare{		// 比较函数 
//     bool operator()(Vertex *x, Vertex *y){
//         return x->d > y->d;
//     }
// };

//bool compare(Vertex *x, Vertex *y){
//        return x->d > y->d;
//    }
//
//void DIJKSTRA(Graph &G, Vertex *s){
//    Vertex *v, *u;
//    INITIALIZE_SINGLE_SOURCE(G, s);
//    // vector<Vertex *>S;
//    // S.clear();
//    // priority_queue<Vertex *, vector<Vertex *>, compare>Q;
//    vector<Vertex *>Q;
//    for (auto v : G.V) {
//        Q.push_back(v);
//        // cout << "add " << v->num << "	d: 	" << v->d << endl; 
//    }
//    make_heap(Q.begin(), Q.end(), compare);
//    while (!Q.empty()) {
//        u = Q.front();
//        // Q.pop();
//        // S.push_back(u);
//        pop_heap(Q.begin(), Q.end(), compare);
//        Q.pop_back();
//        // cout << u->num << "	:	" << u->d << endl;
//        for (auto edge : u->edges){
//            RELAX(u, G.V[edge.first], edge.second);
//        }
//        make_heap(Q.begin(), Q.end(), compare);
//    }
//    return;
//}

void MIN_HEAPIFY(vector<Vertex *> &A, int i, int heapsize){
    int l, r, min;
    l = LEFT(i);
    r = RIGHT(i);
    if(l < heapsize && A[l]->d < A[i]->d)
        min = l;
    else
        min = i;
    if(r < heapsize && A[r]->d < A[min]->d)
        min = r;
    if(min != i){
        auto x = A[i];
        A[i] = A[min];
        A[i]->id = i;
        A[min] = x;
        A[min]->id = min;
        MIN_HEAPIFY(A, min, heapsize);
    }
}

Vertex *HEAP_EXTRACT_MIN(vector<Vertex *> &A, int heapsize){
    if(heapsize < 1){
        printf("heap underflow! \n");
        return(nullptr);
    }
    Vertex *min = A[0];
    // cout << "add min " << min->num << "	d: 	" << min->d << endl;
    A[0] = A[heapsize-1];
    // cout << "add A[0] " << A[0]->num << "	d: 	" << A[0]->d << endl;
    heapsize--;
    MIN_HEAPIFY(A, 0, heapsize);
    return min;
}

void HEAP_DECREASE_KEY(vector<Vertex *> &A, int i, int key){
    if(key > A[i]-> d){
        printf("new key is larger than current key!\n");
        return;
    }
    A[i]->d = key;
    while(i > 0 & A[PARENT(i)]->d > A[i]->d){
        auto x = A[i];
        A[i] = A[PARENT(i)];
        A[i]->id = i;
        A[PARENT(i)] = x;
        A[PARENT(i)]->id = PARENT(i);
        i = PARENT(i);
    }
}

void BUILD_MIN_HEAP(vector<Vertex *> &A, int heapsize){
    for(int i = heapsize/2 - 1; i >= 0; i--)
        MIN_HEAPIFY(A, i, heapsize);
}

void DIJKSTRA(Graph &G, Vertex *s){
    Vertex *v, *u;
    INITIALIZE_SINGLE_SOURCE(G, s);
    vector<Vertex *>Q;
    int i = 0;
    for (auto v : G.V) {
    	v->id = i++;        // 在最小堆里的下标
        Q.push_back(v);
    }
    BUILD_MIN_HEAP(Q, G.V.size());
    while (!Q.empty()){
        u = HEAP_EXTRACT_MIN(Q, Q.size());
        Q.pop_back();   //弹出最后一个元素以更新维护Q.size()
        for (auto edge : u->edges){
            if(G.V[edge.first]->d > u->d + edge.second){
                HEAP_DECREASE_KEY(Q, G.V[edge.first]->id, u->d + edge.second);      // 更新最小距离的估计
                G.V[edge.first]->p = u;     // 更新前驱结点
            }
        }
    }
    return;
}

void JOHNSON(Graph G){
    int *h;
    Vertex *s = new Vertex;
    Graph G1 = G;
    s->num = G1.V.size();
    for (auto v : G1.V){
        s->edges.push_back(pair<int, int>(v->num, 0));   
    }
    G1.V.push_back(s);
    if (BELLMAN_FORD(G1, s) == false){
        cout << "The input graph contains a negative-weight cycle." << endl;
        no_negative_weight_cycle = false; 
	}
    else{
        h = (int *)malloc(sizeof(int)*G1.V.size()); 
        for(auto v : G1.V)
            h[v->num] = v->d;
        for(auto v : G1.V)  // 对于指针，这样引用即可，会改变G1里的具体值
            for (auto edge = v->edges.begin(); edge < v->edges.end(); edge++)
                (*edge).second = (*edge).second + h[v->num] - h[(*edge).first];
        for(auto u : G1.V){
            DIJKSTRA(G1, u);
            for(auto v : G1.V){
                D[u->num][v->num] = v->d + h[v->num] - h[u->num];       // 记录值
                if(v->p == nullptr){
                    Pre[u->num][v->num] = -1;       // 标记源或者到达不了
                }
                else{
                    Pre[u->num][v->num] = v->p->num;    // 标记前驱
                }
            }
        }
//       for(int i = 0; i < G.V.size(); i++){
//	    	for(int j = 0; j< G.V.size(); j++){
//	       		cout << Pre[i][j] << " ";
//	    	}
//	       	cout << endl;
//	    }
    }
    return;
}

void MAKE_SET(Graph &G, int N) {    // 生成N个顶点
    int i;
    Vertex *node;
    for (i = 0; i < N; i++) {
        node = new Vertex;
        node->num = i;
        node->id = i;       // 用于DIJKSTRA算法中, 记录次序 
        G.V.push_back(node);
    }
    return;
}

void Get_Edges(const char *path, Graph &G){
    int i;
    FILE *fp;
    pair<pair<int, int>, int>edge;    // 边
    if((fp=fopen(path,"r"))==NULL){
        printf("Fail to open file!\n");
        exit(0);
    }
    while(fscanf(fp, "%d\t%d\t%d", &edge.first.first, &edge.first.second, &edge.second) != EOF){
    	// cout << edge.first.first << "	" <<  edge.first.second << "	" << edge.second << endl;
        G.V[edge.first.first]->edges.push_back(pair<int, int>(edge.first.second, edge.second));
    }
    fclose(fp);
}

void Output(int N, const char *path){ //  N 个结点
    string route;
    int k;
    FILE *fp;
    if((fp=fopen(path,"w"))==NULL){
        printf("Fail to open file!\n");
        exit(0);
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j< N; j++){
            if(Pre[i][j] == -1 && j != i){      // 到达不了,或者是相同结点
                fprintf(fp, "( %d -> %d can't reach)\n", i, j);
            }
            else if(Pre[i][j] != -1){
                route = to_string(D[i][j]) + ")\n";
                k = j;
                while(Pre[i][k] != -1){
                    route = to_string(k) + ", " + route;
                    k = Pre[i][k];      // 得到前驱
                }
                route = "( " + to_string(k) + ", " + route;
                fprintf(fp,"%s", route.c_str());
            }
        }
    }
    fclose(fp);
}

int main(){
    int i;
    int N, base;    // 结点数和基底
    const char *path, *outpath;
    Graph G;	// 图 
    FILE *fp_time;

    double run_time, start, finish;
	_LARGE_INTEGER time_start;	//开始时间
	_LARGE_INTEGER time_over;	//结束时间
	double dqFreq;		//计时器频率
	LARGE_INTEGER f;	//计时器频率
    
    cout << "Please enter the number of vertices(N:0(quit), 27, 81, 243, 729) and base(5, 7):" << endl;
    cin >> N >> base;

    if((fp_time = fopen("../output/time.txt","w"))==NULL){
        printf("Fail to open time.txt!\n");
        exit(0);
    } 
	  
    while(N){
        switch (N){
        case 27:
            if(base == 5){
                path = "../input/input11.txt";
                outpath = "../output/result11.txt";
            }
            else if(base == 7){
                path = "../input/input12.txt";
                outpath = "../output/result12.txt";
            }
            else{
                cout << "Input Error!" << endl;
                cout << "Please enter the number of vertices(N:0(quit), 27, 81, 243, 729) and base(5, 7):" << endl;
                cin >> N >> base;
                continue;
            }
            break;
        case 81:
            if(base == 5){
                path = "../input/input21.txt";
                outpath = "../output/result21.txt";
            }
            else if(base == 7){
                path = "../input/input22.txt";
                outpath = "../output/result22.txt";
            }
            else{
                cout << "Input Error!" << endl;
                cout << "Please enter the number of vertices(N:0(quit), 27, 81, 243, 729) and base(5, 7):" << endl;
                cin >> N >> base;
                continue;
            }
            break;
        case 243:
            if(base == 5){
                path = "../input/input31.txt";
                outpath = "../output/result31.txt";
            }
            else if(base == 7){
                path = "../input/input32.txt";
                outpath = "../output/result32.txt";
            }
            else{
                cout << "Input Error!" << endl;
                cout << "Please enter the number of vertices(N:0(quit), 27, 81, 243, 729) and base(5, 7):" << endl;
                cin >> N >> base;
                continue;
            }
            break;
        case 729:
            if(base == 5){
                path = "../input/input41.txt";
                outpath = "../output/result41.txt";
            }
            else if(base == 7){
                path = "../input/input42.txt";
                outpath = "../output/result42.txt";
            }
            else{
                cout << "Input Error!" << endl;
                cout << "Please enter the number of vertices(N:0(quit), 27, 81, 243, 729) and base(5, 7):" << endl;
                cin >> N >> base;
                continue;
            }
            break;
        default:
        	cout << "Input Error!" << endl;
        	cout << "Please enter the number of vertices(N:0(quit), 27, 81, 243, 729) and base(5, 7):" << endl;
            cin >> N >> base;
            continue;
        }
        
        G.V.clear();	// 初始化

	 	MAKE_SET(G, N);     // 生成 N 个顶点
        Get_Edges(path, G);

        QueryPerformanceFrequency(&f);
		dqFreq=(double)f.QuadPart;
		QueryPerformanceCounter(&time_start);	//计时开始

        JOHNSON(G);
        
        QueryPerformanceCounter(&time_over);	//计时结束
        start = 1e6*time_start.QuadPart/dqFreq;
        finish = 1e6*time_over.QuadPart/dqFreq;
		run_time = 1e6*(time_over.QuadPart-time_start.QuadPart)/dqFreq;//乘以 1e6 把单位由秒化为微秒，精度为1000 000/（cpu主频）微秒
		
		if(no_negative_weight_cycle){
        	fprintf(fp_time, "N = %d; base = %d; start: %lf us; finish: %lf us; run_time: %lf us\n", N, base, start, finish, run_time); // time.txt
        	Output(N, outpath);     // 输出
        	cout << "Completed for size " << N << " and base " << base << endl;
    	}
		
		no_negative_weight_cycle = true; 
        cout << "Please enter the number of vertices(N:0(quit), 27, 81, 243, 729) and base(5, 7):" << endl;
        cin >> N >> base;
    }
    fclose(fp_time);
    return(0);
}
