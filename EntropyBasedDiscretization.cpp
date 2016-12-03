#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;
//计算一个数据集的entropy
double entropy( double S[][2],int from,int to,int class_types ){
    //只有一个元素，entropy=0
    if( from>=to )
        return 0.0;
    double ent=0.0;
    //计算每个类的概率
    int N=to-from+1;
    int* c_count=new int[class_types];
    memset(c_count,0,class_types*sizeof(int));
    for( int i=from;i<=to;i++ ){
        c_count[(int)S[i][1]]+=1;
    }
    double pro_i=0.0;
    //计算entropy, 以2为底
    for( int i=0;i<class_types;i++ ){
        if( c_count[i]>0 ){
            pro_i=(double)c_count[i]/N;
            ent-=(pro_i*log2(pro_i));
        }
    }
    delete[] c_count;
    return ent;
}
//计算一个划分的entropy
double entropyWithT( double S[][2],int from,int to,int T,int class_types){
    double ent1=entropy(S,from,T,class_types);
    double ent2=entropy(S,T+1,to,class_types);
    int N=to-from+1;
    double entST=(double)(T-from+1)/N*ent1+(double)(to-(T+1)+1)/N*ent2;
    //如果T==to，返回的是entropy(S)
    return entST;
}
//计算二分划分的最佳划分点
int bestCutPoint( double S[][2],int from,int to,int class_types ){
    int T=-1;double min_entST;double entST;
    for( int i=from;i<=to;i++ ){
        //找到相同值的右边界
        while( i<to && S[i]==S[i+1] )
            i++;
        entST=entropyWithT(S,from,to,i,class_types);
        //minimize Entropy(S,T), means maximize information gain
        if( T==-1 ){
            T=i;min_entST=entST;
        }else if( entST<min_entST ){
            T=i;min_entST=entST;
        }
    }
    return T;
}
//计算不同的类标数目
int uniqueClass( double S[][2],int from,int to,int class_types ){
    if( from==to )
        return 1;
    int* c_count=new int[class_types];
    memset(c_count,0,class_types*sizeof(int));
    for( int i=from;i<=to;i++ ){
        c_count[(int)S[i][1]]+=1;
    }
    int uniqueClass=0;
    for( int i=0;i<class_types;i++ ){
        if( c_count[i]!=0 )
            uniqueClass+=1;
    }
    return uniqueClass;
}
//Recursive Minimal Entropy Partioning
void partition(double S[][2],int from,int to,vector<int> &Ts,int class_types){
    //只有一个元素，没有划分的必要
    if( from>=to )
        return ;
    int N=to-from+1;
    int T=bestCutPoint(S,from,to,class_types);
    if( T==to )//最佳划分点是末尾元素，相当于没有划分
        return ;
    //检查Minimal Description Length Principle
    double gainST=entropy(S,from,to,class_types)-entropyWithT(S,from,to,T,class_types);
    //统计每部分中类标的数目,和entropy
    int k=uniqueClass(S,from,to,class_types);
    int k1=uniqueClass(S,from,T,class_types);
    int k2=uniqueClass(S,T+1,to,class_types);
    double entS=entropy(S,from,to,class_types);
    double entS1=entropy(S,from,T,class_types);
    double entS2=entropy(S,T+1,to,class_types);
    //计算下界
    double delta=log2(pow(3,k)-2)-(k*entS-k1*entS1-k2*entS2);
    double threshold=log2(N-1)/N+delta/N;
    if( gainST<= threshold ){
        return ;
    }else{
        //分割点记录
        Ts.push_back(T);
        //递归处理左右两部分
        partition(S,from,T,Ts,class_types);
        partition(S,T+1,to,Ts,class_types);
    }
}
//基于Entropy和Minimal Description Length的离散化方法
int discretization(double S[][2],int from,int to,int class_types){
    vector<int> Ts;
    //调用划分
    partition(S,from,to,Ts,class_types);
    //划分点排序
    sort(Ts.begin(),Ts.end());
    cout<< "-----划分点有" << Ts.size() << "个" <<endl;
    for( int i=0;i<Ts.size();i++ ){
        cout<< Ts[i] <<endl;
    }
    cout<< "-----离散化结果："<<endl;
    for( int i=0;i<Ts.size();i++ ){
        int po_left=(i==0)?from:Ts[i-1]+1;
        int po_right=Ts[i];
        for( int j=po_left;j<=po_right;j++ ){
            //离散化结果
            cout<< i <<endl;
        }
    }
    for( int i=Ts[Ts.size()-1]+1;i<=to;i++ ){
        //离散化结果
        cout<< Ts.size() <<endl;
    }
}
int main()
{
    //double S[7][2]={{1,0},{2,0},{3,1},{4,1},{5,2},{6,2},{7,2}};
    //double S[7][2]={{1,0},{2,0},{3,1},{4,1},{5,2},{6,2},{7,2}};
    double S[15][2]={{1,0},{2,0},{3,0},{4,0},{5,1},{6,1},{7,1},{8,1},{9,1},{10,2},{11,2},{12,2},{13,1},{14,2},{15,2}};
    //double entS=entropy(S,0,6,2);
    //cout<< entS <<endl;
    //cout<< infoGain(S,0,6,2,2,entS) <<endl;
    //cout<< bestCutPoint(S,0,6,2) <<endl;

    //vector<int> Ts;
    //partition(S,0,6,Ts,3);
    //cout<< "-----划分点有" << Ts.size() << "个" <<endl;
    //for( int i=0;i<Ts.size();i++ ){
    //    cout<< Ts[i] <<endl;
    //}

    discretization(S,0,14,3);
    return 0;
}
