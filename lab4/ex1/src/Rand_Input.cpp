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

int N;  // 结点数
vector<pair<int, int> >E;    // 边集合
vector<int> weights;

void Rand_Input(int n, const char *path){
    int i, j, k, x;
    int node1, node2, edge_number, edge;
    int weight;
    bool flag = true;      // 标志是不是所有顶点的关联边数是否还可以增加
    int count[n];   // 每个结点关联边数
    FILE* fp;
    pair<int, int> rand_edge;    // 随机生成的边
    pair<int, int> sym_edge;    // 随机生成的边的对称边
    for(i = 0; i < n; i++)      // 初始化 
        count[i] = 0;

    srand((unsigned)time(NULL));    // 用系统定时/计数器的值作为随机种子, 产生比较好的随机数
    for(i=0; i<n; i++){
        rand_edge.first = i;
        sym_edge.second = i;
        edge_number = rand() % (n/2) + 1;   // 对每一顶点随机生成的边数
        // cout << "number:	"<<edge_number << endl;
        edge = count[i];        // 目前已有边数
        // cout << "edge:	"<<edge << endl;
        while(edge < edge_number && flag){     // 边数是否达标, 所有顶点的边数是否还可以增加
            rand_edge.second = rand() % n;         // 选择另外一个顶点
            sym_edge.first = rand_edge.second;
            weight = rand() % 20 + 1;     // 边的权重为 (1, 20)
            if(find(E.begin(), E.end(), rand_edge) == E.end() && 
				find(E.begin(), E.end(), sym_edge) == E.end() && 
				count[rand_edge.second] < n/2)
			{  // 无重边且另一个顶点的相关边少于 N/2 条
                E.push_back(rand_edge);         // 加入这条边
                // cout << "size:	"<<E.size() << endl;
                weights.push_back(weight);
                edge += 1;
                count[i] += 1;
                if(rand_edge.second != i)   // 指向自身的环边只算一条
                    count[rand_edge.second] += 1;
            }
            else{
                continue;
            }
            for(j = 0; j < n; j++){     // 判断所有顶点的边数是否已达上界
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
    E.clear();	// 清空边
	weights.clear();	// 清空权重 
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
