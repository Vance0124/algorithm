#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<time.h>
#include<algorithm>
#include<iostream>
#include<vector>
#include<string>
#include<utility>

using namespace std;

int N;  // �����
vector<pair<int, int> >E;    // �߼���
vector<int> weights;

void Rand_Input(int n, const char *path){
    int i, j, k, x;
    int node1, node2, edge_number, edge;
    int weight;
    bool flag = true;      // ��־�ǲ������ж���Ĺ��������Ƿ񻹿�������
    int count[n];   // ÿ������������
    FILE* fp;
    pair<int, int> rand_edge;    // ������ɵı�
    pair<int, int> sym_edge;    // ������ɵıߵĶԳƱ�
    for(i = 0; i < n; i++)      // ��ʼ�� 
        count[i] = 0;

    srand((unsigned)time(NULL));    // ��ϵͳ��ʱ/��������ֵ��Ϊ�������, �����ȽϺõ������
    for(i=0; i<n; i++){
        rand_edge.first = i;
        sym_edge.second = i;
        edge_number = rand() % (n/2) + 1;   // ��ÿһ����������ɵı���
        // cout << "number:	"<<edge_number << endl;
        edge = count[i];        // Ŀǰ���б���
        // cout << "edge:	"<<edge << endl;
        while(edge < edge_number && flag){     // �����Ƿ���, ���ж���ı����Ƿ񻹿�������
            rand_edge.second = rand() % n;         // ѡ������һ������
            sym_edge.first = rand_edge.second;
            weight = rand() % 20 + 1;     // �ߵ�Ȩ��Ϊ (1, 20)
            if(find(E.begin(), E.end(), rand_edge) == E.end() && 
				find(E.begin(), E.end(), sym_edge) == E.end() && 
				count[rand_edge.second] < n/2)
			{  // ���ر�����һ���������ر����� N/2 ��
                E.push_back(rand_edge);         // ����������
                // cout << "size:	"<<E.size() << endl;
                weights.push_back(weight);
                edge += 1;
                count[i] += 1;
                if(rand_edge.second != i)   // ָ������Ļ���ֻ��һ��
                    count[rand_edge.second] += 1;
            }
            else{
                continue;
            }
            for(j = 0; j < n; j++){     // �ж����ж���ı����Ƿ��Ѵ��Ͻ�
                if(count[j] < n/2)
                    break;
            }
            if(j >= n)
                flag = false;
        }
    }
    if((fp=fopen(path,"w"))==NULL){
        printf("Fail to open file!\n");
        exit(0);
    }
    for(k = 0; k < E.size(); k++ ){
        fprintf(fp,"%d\t%d\t%d\n",E[k].first, E[k].second, weights[k]);
        printf("%d\n",k+1);
    }
    E.clear();	// ��ձ�
	weights.clear();	// ���Ȩ�� 
    printf("OK!!!\n");
    fclose(fp);
    return;
}

int main(){
    const char *path;
    cout << "Please enter the number of vertices(N:0, 8, 64, 128, 512):" << endl;
    cin >> N;
    while(N){
        switch (N){
        case 8:
            path = "../input/input1.txt";
            break;
        case 64:
            path = "../input/input2.txt";
            break;
        case 128:
            path = "../input/input3.txt";
            break;
        case 512:
            path = "../input/input4.txt";
            break;
        default:
        	cout << "Input Error!" << endl;
        	cout << "Please enter the number of vertices(N:0, 8, 64, 128, 512):" << endl;
        	cin >> N;
            continue;
        }
        Rand_Input(N, path);
        cout << "Please enter the number of vertices(N:0, 8, 64, 128, 512):" << endl;
        cin >> N;
    }
    return 0;
}
