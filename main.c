#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXSIZE 10000 /*参加排序元素的最大个数*/
#define INF 0x3f
typedef	int	KeyType;
typedef int InfoType;
typedef struct {
    KeyType	key;
    InfoType weight;	// 其他字段（自己设计）
}RedType;
typedef struct
{
    RedType	r[MAXSIZE+1];
    int	length; /*参加排序元素的实际个数*/
}SqList;

void init(SqList* L,int size);
void print_L(SqList* L);//打印L
int SelectMinKey(SqList* L,int i);//选择[i,length]里的最小值并返回下标
void Swap(SqList* L, int i, int index);//交换下标为i和index的数据
void SelectSort(SqList* L);//选择排序
void InsertSort(SqList* L);//插入排序
void BubbleSort(SqList* L);//冒泡排序
void BInsertSort(SqList* L);//折半插入排序
void QuickSort_Recursion(SqList* L,int low,int high);//快速排序——递归,low为指向第一个数据的指针，high为指向最后一个指针的下标
int Partition(SqList* L,int low,int high);//将数列L按枢轴划分开，使枢轴左边的数都比他小，右边都比他大
void HeapSort(SqList* L);//堆排序
void Heapify(SqList* L,int i);//将节点i以及他的左右孩子结点化成一个小堆
void BuildHeap(SqList* L);
void MergeSort(SqList* L,int l,int r);//归并排序
void MergingSort(SqList* L,int l,int m,int r);//将两个有序序列合并为一个有序序列

int main() {
    SqList* L = (SqList*)malloc(sizeof(SqList));
    clock_t start,end;

    init(L,10000);
    printf("Before sorting:\n");
    print_L(L);


    start = clock();
    QuickSort_Recursion(L,0,L->length);
    //HeapSort(L);
    end = clock();

    printf("Sorted:\n");
    print_L(L);
    double time = (double)(end - start);
    printf("excuted time: %.4f ms",time);
    free(L);
    return 0;
}
//直接初始化一个数组，方便调试
void init(SqList* L,int size){
    L->length=size;
    for (int i = 0; i < L->length; ++i) {
        L->r[i].key = rand() % (1000 - 0 + 1);
        L->r[i].weight = i;
    }
}
//打印L
void print_L(SqList* L){
    for (int i = 0; i < L->length; ++i) {
        printf("key:%d weight:%d\n",L->r[i].key,L->r[i].weight);
    }
}
//选择最小值
int SelectMinKey(SqList* L,int i){
    int min=INF;
    int index;
    for (i; i < L->length; ++i) {
        if (L->r[i].key<min){
            min = L->r[i].key;
            index=i;
        }
    }return index;
}
//值交换
void Swap(SqList* L, int i, int index){
    RedType temp;
    if (L->r[i].key != L->r[index].key){
        temp = L->r[i];
        L->r[i] = L->r[index];
        L->r[index] = temp;
    }
}
//选择排序
void SelectSort(SqList* L){
    int index;
    for (int i = 0; i < L->length; ++i) {
        index = SelectMinKey(L,i);
        Swap(L, i, index);
    }
}
//插入排序
void InsertSort(SqList* L){
    int j;
    RedType R; //暂存位置
    for (int i = 1; i < L->length; ++i) {
        if (L->r[i].key < L->r[i-1].key){
            R = L->r[i];
            for (j = i-1;  L->r[j].key > R.key ; j--)
                L->r[j+1] = L->r[j];
            L->r[j+1] = R;
        }
    }
}
//冒泡排序
void BubbleSort(SqList* L){
    for (int i = 1; i < L->length; ++i) {
        for (int j = 0; j < L->length-i; ++j) {
            if (L->r[j].key > L->r[j+1].key){
                Swap(L,j,j+1);
            }
        }
    }
}
//折半插入排序
void BInsertSort(SqList* L){
    RedType R; //暂存位置
    int low,high,mid,i,j;
    for (i = 1; i < L->length; ++i) {
        R = L->r[i];
        low = 0;
        high = i-1;
        while(low<=high){
            mid = (low+high)/2;
            if(R.key < L->r[mid].key)   high = mid-1;
            else    low = mid+1;
        }
        for (j = i-1; j >= high+1 ; --j)    L->r[j+1] = L->r[j];
        L->r[high+1] = R;
    }
}
//快速排序
void QuickSort_Recursion(SqList* L,int low,int high){
    int pivotloc;
    if (low<high){
        pivotloc = Partition(L,low,high);
        QuickSort_Recursion(L,low,pivotloc-1);
        QuickSort_Recursion(L,pivotloc+1,high);
    }
}
//将数列L按枢轴划分开，使枢轴左边的数都比他小，右边都比他大
int Partition(SqList* L,int low,int high){
    RedType R = L->r[low];//将最左边low位得数据先暂存在R，该数据为枢轴
    int pivotkey = L->r[low].key;
    while(low<high){
        //如果high位得key大于pivotkey，则high指针向左移，知道high位得key<pivotkey时，结束循环，然后将high位得值赋给low位
        while (low<high && L->r[high].key>=pivotkey)     --high;
        L->r[low] = L->r[high];
        //如果low位得key<pivotkey，则咯哇指针右移，直至low位得key>pivotkey时，结束循环，然后将该low位得值赋给high位
        while(low<high && L->r[low].key<=pivotkey)   ++low;
        L->r[high] = L->r[low];
    }
    //low==high 时循环结束，此时low（或high）位没有数据，将枢轴得数据赋给low位
    L->r[low] = R;
    return low;
}
//堆排序
void HeapSort(SqList* L){
    BuildHeap(L);
    int n = L->length;
    for (int i = n-1; i >=0 ; --i) {
        Swap(L,i,0);
        L->length=i;
        BuildHeap(L);
    }
    //循环结束后，将L->length变为原始的长度，因为后边得print_L函数在打印时还要用到L->length，如果
    //不变为原始长度，那么L->length循环过后为0，之后在执行print_L函数时，不会打印任何出任何数据。
    L->length = n;
}
//将节点i以及他的左右孩子结点化成一个小堆
void Heapify(SqList* L,int i){
    if(i>=L->length) return;//递归出口，经测试可有可无。
    int left = 2*i+1;
    int right = 2*i+2;
    //找到三个节点中数据最大得节点
    int max = i;
    if (left < L->length && L->r[left].key > L->r[max].key)
        max = left;
    if (right < L->length && L->r[right].key > L->r[max].key)
        max = right;
    //如果数据最大的节点不是i，则将i和max节点交换
    if (max != i){
        Swap(L,max,i);
        Heapify(L,max);
    }
}
//建堆
void BuildHeap(SqList* L){
    int last_node = (L->length - 1) / 2;
    for (int i = last_node; i >= 0 ; --i) {
        Heapify(L,i);
    }
}
//两个有序序列归并排序
//l为最左边元素下标，r为最右边元素下标,m为第二个数组的第一个下标
void MergingSort(SqList* L,int l,int m,int r){
    int LEFT_SIZE = m-l;
    int RIGHT_SIZE = r-m+1;
    RedType left[LEFT_SIZE];
    RedType right[RIGHT_SIZE];
    int i,j,k=l;
    for (i = 0; i < LEFT_SIZE; ++i) {
        left[i] = L->r[k];
        k++;
    }
    for (j = m; j <=r ; ++j) {
        right[j-m] = L->r[j];
    }
//    for (int n = 0; n < RIGHT_SIZE; ++n) {
//        printf("%d ",right[n].key);
//    }
//    printf("\n");
    i=0;    j=0;    k=l;
    while(i<LEFT_SIZE && j<RIGHT_SIZE){
        if(left[i].key > right[j].key){
            L->r[k] = left[i];
            i++;
            k++;
        }
        else{
            L->r[k] = right[j];
            j++;
            k++;
        }
    }
    while(i < LEFT_SIZE){
        L->r[k] = left[i];
        i++;
        k++;
    }
    while(j < RIGHT_SIZE){
        L->r[k] = right[j];
        j++;
        k++;
    }
}
//完全无序的序列归并排序
//l为最左边元素下标，r为最右边元素下标
void MergeSort(SqList* L,int l,int r){
    if(l==r) return;
    else{
        int m = (l+r)/2;
        MergeSort(L,l,m);
        MergeSort(L,m+1,r);
        MergingSort(L,l,m+1,r);
    }
}