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
vector<pair<int, int> >E;    // 边集合
vector<int> weights;

void Rand_Input(int N, const char *path, int base){  
     pair<int, int> rand_edge;    // 随机生成的边
     int weight;
     FILE* fp;
    srand((unsigned)time(NULL));    // 用系统定时/计数器的值作为随机种子, 产生比较好的随机数
    for(int i=0; i<N; i++){    // 为每个结点生成指定的边
        rand_edge.first = i;
        int edge_number = int(log(N) / log(base));    // 计算边的数量
        int j = 0;
        while (j < edge_number){     // 为有向图，因此生成 edge_number 条边即可
            rand_edge.second = rand() % N;     // 连结的另外一个结点 
            if(find(E.begin(), E.end(), rand_edge) == E.end()){  // 查看是否与以前生成的边重复
                weight = rand() % 51;      // 边的权重为 (0, 50)
                E.push_back(rand_edge);         // 加入这条边
                // cout << "size:	"<<E.size() << endl;
                weights.push_back(weight);
                j++;
            }
        }
    }
    if((fp=fopen(path,"w"))==NULL){
        printf("Fail to open file!\n");
        exit(0);
    }
    for(int k = 0; k < E.size(); k++ ){
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
    int n;
    int N;  // 结点数
    int base;   // 边的基底
    const char *path;
    cout << "Please enter the number of input file(0(quit)  11  12  21  22  31  32  41  42):" << endl;
    cin >> n;
    while(n){
        switch (n){
        case 11:
            path = "../input/input11.txt";
            N = 27;
            base = 5;
            break;
        case 12:
            path = "../input/input12.txt";
            N = 27;
            base = 7;
            break;
        case 21:
            path = "../input/input21.txt";
            N = 81;
            base = 5;
            break;
        case 22:
            path = "../input/input22.txt";
            N = 81;
            base = 7;
            break;
        case 31:
            path = "../input/input31.txt";
            N = 243;
            base = 5;
            break;
        case 32:
            path = "../input/input32.txt";
            N = 243;
            base = 7;
            break;
        case 41:
            path = "../input/input41.txt";
            N = 729;
            base = 5;
            break;
        case 42:
            path = "../input/input42.txt";
            N = 729;
            base = 7;
            break;
        default:
        	cout << "Input Error!" << endl;
        	cout << "Please enter the number of input file(0(quit)  11  12  21  22  31  32  41  42):" << endl;
            cin >> n;
            continue;
        }
        Rand_Input(N, path, base);
        cout << "Please enter the number of input file(0(quit)  11  12  21  22  31  32  41  42):" << endl;
    	cin >> n;
    }
    return 0;
}
