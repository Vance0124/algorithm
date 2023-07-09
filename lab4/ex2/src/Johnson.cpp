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
#define PARENT(i) (i-1)/2       //���ڵ�    ������
#define LEFT(i) 2*i+1         //����
#define RIGHT(i) 2*i+2      //�Һ���

using namespace std;

int D[MAX][MAX];    // ���н��Ե����·������
int Pre[MAX][MAX];  // ��¼ǰ��
bool no_negative_weight_cycle = true;	// ��־�Ƿ��и��� 

typedef struct vertex{ 
    int num;        // num�洢������
    int d;          // ���·������ֵ
    int id;       // ����DIJKSTRA�㷨��, ��¼���� 
    vertex *p;      // ǰ������
    vector<pair<int, int> >edges; // �ڽӱ�,��һ�� int Ϊ������, ͬʱҲ��Ӧ����ͼG.V�����еĴ洢�±�;�ڶ��� int Ϊ�ߵ�Ȩֵ
}Vertex;

typedef struct graph{   // ����ͼ
    vector<Vertex *>V;  // ͼ�ṹ�У��洢��������
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
        for (auto u : G.V)             // ����ڿα����Զ���һ��for, ������Ϊ�����ڽӱ�洢, Ҫ�������ж�����ܻ�����б�
            for (auto edge : u->edges)
                RELAX(u, G.V[edge.first], edge.second);
    for (auto u : G.V)
        for (auto edge : u->edges)
            if (G.V[edge.first]->d > u->d + edge.second)
                return false;
    return true;
}

// struct compare{		// �ȽϺ��� 
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
    	v->id = i++;        // ����С������±�
        Q.push_back(v);
    }
    BUILD_MIN_HEAP(Q, G.V.size());
    while (!Q.empty()){
        u = HEAP_EXTRACT_MIN(Q, Q.size());
        Q.pop_back();   //�������һ��Ԫ���Ը���ά��Q.size()
        for (auto edge : u->edges){
            if(G.V[edge.first]->d > u->d + edge.second){
                HEAP_DECREASE_KEY(Q, G.V[edge.first]->id, u->d + edge.second);      // ������С����Ĺ���
                G.V[edge.first]->p = u;     // ����ǰ�����
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
        for(auto v : G1.V)  // ����ָ�룬�������ü��ɣ���ı�G1��ľ���ֵ
            for (auto edge = v->edges.begin(); edge < v->edges.end(); edge++)
                (*edge).second = (*edge).second + h[v->num] - h[(*edge).first];
        for(auto u : G1.V){
            DIJKSTRA(G1, u);
            for(auto v : G1.V){
                D[u->num][v->num] = v->d + h[v->num] - h[u->num];       // ��¼ֵ
                if(v->p == nullptr){
                    Pre[u->num][v->num] = -1;       // ���Դ���ߵ��ﲻ��
                }
                else{
                    Pre[u->num][v->num] = v->p->num;    // ���ǰ��
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

void MAKE_SET(Graph &G, int N) {    // ����N������
    int i;
    Vertex *node;
    for (i = 0; i < N; i++) {
        node = new Vertex;
        node->num = i;
        node->id = i;       // ����DIJKSTRA�㷨��, ��¼���� 
        G.V.push_back(node);
    }
    return;
}

void Get_Edges(const char *path, Graph &G){
    int i;
    FILE *fp;
    pair<pair<int, int>, int>edge;    // ��
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

void Output(int N, const char *path){ //  N �����
    string route;
    int k;
    FILE *fp;
    if((fp=fopen(path,"w"))==NULL){
        printf("Fail to open file!\n");
        exit(0);
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j< N; j++){
            if(Pre[i][j] == -1 && j != i){      // ���ﲻ��,��������ͬ���
                fprintf(fp, "( %d -> %d can't reach)\n", i, j);
            }
            else if(Pre[i][j] != -1){
                route = to_string(D[i][j]) + ")\n";
                k = j;
                while(Pre[i][k] != -1){
                    route = to_string(k) + ", " + route;
                    k = Pre[i][k];      // �õ�ǰ��
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
    int N, base;    // ������ͻ���
    const char *path, *outpath;
    Graph G;	// ͼ 
    FILE *fp_time;

    double run_time, start, finish;
	_LARGE_INTEGER time_start;	//��ʼʱ��
	_LARGE_INTEGER time_over;	//����ʱ��
	double dqFreq;		//��ʱ��Ƶ��
	LARGE_INTEGER f;	//��ʱ��Ƶ��
    
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
        
        G.V.clear();	// ��ʼ��

	 	MAKE_SET(G, N);     // ���� N ������
        Get_Edges(path, G);

        QueryPerformanceFrequency(&f);
		dqFreq=(double)f.QuadPart;
		QueryPerformanceCounter(&time_start);	//��ʱ��ʼ

        JOHNSON(G);
        
        QueryPerformanceCounter(&time_over);	//��ʱ����
        start = 1e6*time_start.QuadPart/dqFreq;
        finish = 1e6*time_over.QuadPart/dqFreq;
		run_time = 1e6*(time_over.QuadPart-time_start.QuadPart)/dqFreq;//���� 1e6 �ѵ�λ���뻯Ϊ΢�룬����Ϊ1000 000/��cpu��Ƶ��΢��
		
		if(no_negative_weight_cycle){
        	fprintf(fp_time, "N = %d; base = %d; start: %lf us; finish: %lf us; run_time: %lf us\n", N, base, start, finish, run_time); // time.txt
        	Output(N, outpath);     // ���
        	cout << "Completed for size " << N << " and base " << base << endl;
    	}
		
		no_negative_weight_cycle = true; 
        cout << "Please enter the number of vertices(N:0(quit), 27, 81, 243, 729) and base(5, 7):" << endl;
        cin >> N >> base;
    }
    fclose(fp_time);
    return(0);
}
