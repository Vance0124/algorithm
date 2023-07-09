#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<Windows.h>
#include<time.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include<utility>

using namespace std;

int N;  // �����

typedef struct node{
    int key;
    int rank;
    node* p;
}Node;

typedef struct graph{
    Node **V;   // ���㼯��
    vector<pair<pair<int, int>, int> > E;    // �߼���
}Graph, *GraphPtr;

Node *MAKE_SET(int key){
    Node* x = (Node *)malloc(sizeof(Node));
    x->key = key;
    x->p = x;
    x->rank = 0;
    return x;
}

Node *FIND_SET(Node* x){
    if (x->p!=x)
        x->p = FIND_SET(x->p);
    return x->p;
}

void LINK(Node* x, Node* y)
{
    if(x->rank > y->rank)
        y->p = x;
    else{
        x->p = y;
        if(x->rank==y->rank){
            y->rank += 1;
        }
    }
}


void UNION(Node* x, Node* y){
    LINK (FIND_SET(x), FIND_SET(y));
}

bool compare(pair<pair<int, int>, int> x, pair<pair<int, int>, int> y){     // ���򷽷�
    return(x.second < y.second);
}

vector<pair<pair<int, int>, int> > MST_KRUSKAL(GraphPtr G){
    vector<pair<pair<int, int>, int> > A; // A
    for (int i = 0; i < N; i++){
        G->V[i] = MAKE_SET(i);
    }
    //  cout << "before sort: " <<endl;
    //  for (int i = 0; i < G->E.size(); i++){
	//  	cout << G->E[i].first.first << "\t" << G->E[i].first.second <<"\t" << G->E[i].second << endl;
	//  }
    sort(G->E.begin(), G->E.end(), compare);
    //  cout << "after sort: " <<endl;
    //  for (int i = 0; i < G->E.size(); i++){
	//  	cout << G->E[i].first.first << "\t" << G->E[i].first.second <<"\t" << G->E[i].second << endl;
	//  } 
    for (int i = 0; i < G->E.size(); i++){
        if (FIND_SET(G->V[G->E[i].first.first]) != FIND_SET(G->V[G->E[i].first.second])){
            A.push_back(G->E[i]);
            // cout << A.size() <<endl;
            UNION(G->V[G->E[i].first.first], G->V[G->E[i].first.second]);
        }
    }
    return A;
}

void Get_Edges(const char *path, GraphPtr G){
    int i;
    FILE *fp;
    pair<pair<int, int>, int>edge;    // ��
    if((fp=fopen(path,"r"))==NULL){
        printf("Fail to open file!\n");
        exit(0);
    }
    while(fscanf(fp, "%d\t%d\t%d", &edge.first.first, &edge.first.second, &edge.second) != EOF){
        G->E.push_back(edge);
    }
    G->V = (Node **)malloc(sizeof(Node *)*N);	// Ϊ��㼯�Ϸ���ռ� 
    fclose(fp);
}

int main(){
    int i, tree_weight;
    const char *path, *outpath;
    Graph G;	// ͼ 
    FILE *fp_time, *fp_result;
    vector<pair<pair<int, int>, int> > A;   // MST�ı߼�

    double run_time, start, finish;
	_LARGE_INTEGER time_start;	//��ʼʱ��
	_LARGE_INTEGER time_over;	//����ʱ��
	double dqFreq;		//��ʱ��Ƶ��
	LARGE_INTEGER f;	//��ʱ��Ƶ��
    
    cout << "Please enter the number of vertices(N:0, 8, 64, 128, 512):" << endl;
    cin >> N;

    if((fp_time = fopen("../output/time.txt","w"))==NULL){
        printf("Fail to open time.txt!\n");
        exit(0);
    } 
	  
    while(N){
        switch (N){
        case 8:
            path = "../input/input1.txt";
            outpath = "../output/result1.txt";
            break;
        case 64:
            path = "../input/input2.txt";
            outpath = "../output/result2.txt";
            break;
        case 128:
            path = "../input/input3.txt";
            outpath = "../output/result3.txt";
            break;
        case 512:
            path = "../input/input4.txt";
            outpath = "../output/result4.txt";
            break;
        default:
        	cout << "Input Error!" << endl;
        	cout << "Please enter the number of vertices(N:0, 8, 64, 128, 512):" << endl;
        	cin >> N;
            continue;
        }
        
        G.E.clear();	// ��ʼ��
	 	A.clear();
	 	
        Get_Edges(path, &G);
        QueryPerformanceFrequency(&f);
		dqFreq=(double)f.QuadPart;
		QueryPerformanceCounter(&time_start);	//��ʱ��ʼ

        A = MST_KRUSKAL(&G);
        
        QueryPerformanceCounter(&time_over);	//��ʱ����
        start = 1e6*time_start.QuadPart/dqFreq;
        finish = 1e6*time_over.QuadPart/dqFreq;
		run_time = 1e6*(time_over.QuadPart-time_start.QuadPart)/dqFreq;//���� 1e6 �ѵ�λ���뻯Ϊ΢�룬����Ϊ1000 000/��cpu��Ƶ��΢��

        fprintf(fp_time, "N = %d; start: %lf us; finish: %lf us; run_time: %lf us\n", N, start, finish, run_time); // time.txt

        if((fp_result = fopen(outpath,"w"))==NULL){
            printf("Fail to open result.txt!\n");
            exit(0);
        }
        tree_weight = 0;
        for (i = 0; i < A.size(); i++){
            fprintf(fp_result, "(%d\t%d\t%d)\n", A[i].first.first, A[i].first.second, A[i].second);
            tree_weight += A[i].second;
        }
        fprintf(fp_result, "%d", tree_weight);
        fclose(fp_result);
		
		cout << "Completed for size " << N << endl;
        cout << "Please enter the number of vertices(N:0, 8, 64, 128, 512):" << endl;
        cin >> N;
    }
    return(0);
}
