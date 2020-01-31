#include <iostream>
#include <stdlib.h>
using namespace std;

template <class T> class Array;
template <class T>
ostream& operator<<(ostream& output, const Array<T>& A);
template <class T>
void mergeTwoOrderedArray(const Array<T>& A, const Array<T>& B, Array<T> &R);

template <class T>
class Array{
private:
    T* elems;//数据区
    int size;//规模
    int capacity;//容量
    void expand();//扩容
    void shrink();//缩容
public:
    //构造函数
    Array();//默认构造函数，容量为5
    Array(int n);//指定数组容量的构造函数
    //析构函数
    ~Array(){delete [] elems;}
    //只读访问接口
    int getSize() const {return size;}//返回数组规模
    int getCapacity() const {return capacity;}//返回数组容量
    int find(const T& e) const {return find(e, 0, size);}//无序数组整体查找
    int find(const T& e, int lo, int hi) const;//无序数组区间查找，返回等于e的元素的最大位置
    int search(const T& e) const {return size <= 0 ? -1 : search(e, 0, size);}//有序数组整体查找
    int search(const T& e, int lo, int hi) const;//有序数组区间查找，返回不大于e的元素的最大位置
    bool disordered() const;//判断数组是否已经有序
    //可写访问接口
    T& operator[](int r) const {return elems[r];}//重载下标运算符，直接引用数组元素
    Array<T>& operator=(const Array<T>& A);//重载复制运算符，直接复制数组
    friend ostream& operator<<<T>(ostream& output, const Array<T>& A);//重载流插入运算符，打印数组信息

    int insert(int r, const T& e);//将e插入到位置为r处，返回元素插入位置
    int insert(const T& e){return insert(size, e);}//默认在数组末尾插入元素e，返回元素插入位置
    T remove(int r);//删除位置为r的元素，返回被删除元素
    int remove(int lo, int hi);//删除在区间[lo,hi)之内的元素，返回被删除元素个数
    T put(int r, const T& e);//用e替换位置为r的元素，返回被替换的元素
    int deduplicate();//无序数组去重，返回被删除元素数
    int uniquify();//有序数组去重，返回被删除元素数
    void sort(){sort(0, size);}//数组整体排序
    void sort(int lo, int hi);//对区间段[lo,hi)排序，采用冒泡排序法
    void unsort(){unsort(0, size);}//整体置乱
    void unsort(int lo, int hi);//区间[lo, hi)置乱

    friend void mergeTwoOrderedArray<T>(const Array<T>& A, const Array<T>& B, Array<T> &R);//将有序数组A、B合并为一个新的有序数组R
};

template <class T>
Array<T>::Array(){
    elems = new T[capacity = 5];
    size = 0;
}

template <class T>
Array<T>::Array(int n){
    elems = new T[capacity = n];
    size = 0;
}

template <class T>
int Array<T>::find(const T& e, int lo, int hi) const{//查找等于e且index最大的元素，返回其index；若查找失败，返回-1
    try{
        if(lo < 0 || lo >= size || hi < 0 || hi > size || lo >= hi) throw "Find(e, lo, hi) index error! Index should be ";
        while((lo < hi--) && (e != elems[hi]));
        if(hi >= lo) return hi;
    }catch(const char* msg){
        cerr << msg << "0 <= lo < hi <= " << size << endl;
    }
    return -1;//hi < lo，查找失败
}

template <class T>
int Array<T>::search(const T& e, int lo, int hi) const{
    try{
        if(lo < 0 || lo >= size || hi < 0 || hi > size || lo >= hi) throw "Search(e, lo, hi) index error! Index should be ";
        while(lo < hi){
            int mi = (lo + hi) >> 1;
            (e < elems[mi]) ? hi = mi : lo = mi + 1;
        }
    }catch(const char* msg){
        cerr << msg << "0 <= lo < hi <= " << size << endl;
        return -1;
    }
    return --lo;
}

template <class T>
bool Array<T>::disordered() const{
    int n = 0;
    for(int i = 1; i < size; ++i){
        if(elems[i - 1] > elems[i]) ++n;//逆序则计数
    }
    return !n;//有序当且仅当n=0
}

template <class T>
Array<T>& Array<T>::operator=(const Array<T>& A){
    if(elems) delete [] elems;
    elems = new T[capacity = A.capacity];
    size = 0;
    int lo = 0;
    while(lo < A.size) elems[size++] = A.elems[lo++];
    return *this;
}

template <class T>
ostream& operator<<(ostream& output, const Array<T>& A){
    output << "数组信息，";
    output <<  "规模：" << A.size << "，容量：" << A.capacity << endl;
    for(int i = 0; i < A.size; ++i) output << "elems[" << i << "]:  " << A.elems[i] << endl;
    return output;
}

template <class T>
void Array<T>::expand(){
    if(size == capacity){
        T* oldElems = elems;
        elems = new T[capacity <<= 1];
        for(int i = 0; i < size; ++i) elems[i] = oldElems[i];
        delete [] oldElems;
    }
}

template <class T>
void Array<T>::shrink(){
    if((size << 2) < capacity){//若(size / capacity) < 0.25,将capacity缩小一半
        T* oldElems = elems;
        elems = new T[capacity >>= 1];
        for(int i = 0; i < size; ++i) elems[i] = oldElems[i];
        delete [] oldElems;
    }
}

template <class T>
int Array<T>::insert(int r, const T& e){
    try{
        if(r < 0 || r > size) throw "Insert(r, e) index error! Index should be ";
        expand();
        for(int i = size; i > r; --i) elems[i] = elems[i - 1];
        elems[r] = e;
        size++;
    }catch(const char* msg){
        cerr << msg << "0 <= r <= " << size << endl;
    }
    return r;
}

template <class T>
T Array<T>::remove(int r){
    T e;
    try{
        if(r < 0 || r >= size) throw "Remove(r) index error! Index should be ";
        e = elems[r];
        remove(r, r + 1);
    }catch(const char* msg){
        cerr << msg << "0 <= r < " << size << endl;
    }
    return e;
}

template <class T>
int Array<T>::remove(int lo, int hi){
    try{
        if(lo < 0 || lo >= size || hi < 0 || hi > size || lo >= hi) throw "Remove(lo, hi) index error! Index should be ";
        while(hi < size) elems[lo++] = elems[hi++];
        size = lo;
        shrink();
    }catch(const char* msg){
        cerr << msg << "0 <= lo < hi <= " << size << endl;
    }
    return hi - lo;
}

template <class T>
T Array<T>::put(int r, const T& e){
    T o;
    try{
        if(r < 0 || r >= size) throw "Put(r, e) index error! Index should be ";
        o = elems[r];
        elems[r] = e;
    }catch(const char* msg){
        cerr << msg << "0 <= r < " << size << endl;
    }
    return o;
}

template <class T>
int Array<T>::deduplicate(){
    int oldSize = size;
    int i = 1;
    while(i < size){
        (find(elems[i], 0, i) < 0) ? ++i : remove(i);
    }
    shrink();
    return oldSize - size;
}

template <class T>
int Array<T>::uniquify(){
    int i = 0, j = 0;
    while(++j < size){
        if(elems[i] != elems[j]) elems[++i] = elems[j];
    }
    size = ++i;
    shrink();
    return j - i;
}

template <class T>
void Array<T>::sort(int lo, int hi){
    try{
        if(lo < 0 || lo >= size || hi < 0 || hi > size || lo >= hi) throw "Sort(lo, hi) index error! Index should be ";
        bool sorted = false;
        while(!sorted){
            sorted = true;
            for(int i = lo + 1; i < hi; ++i){
                if(elems[i - 1] > elems[i]){
                    swap(elems[i], elems[i - 1]);
                    sorted = false;
                }
            }
            --hi;
        }
    }catch(const char* msg){
        cerr << msg << "0 <= lo < hi <= " << size << endl;
    }
}

template <class T>
void Array<T>::unsort(int lo, int hi){
    try{
        if(lo < 0 || lo >= size || hi < 0 || hi > size || lo >= hi) throw "Unsort(lo, hi) index error! Index should be ";
        T* A = elems + lo;
        for(int i = hi - lo; i > 0; --i)
            swap(A[i - 1], A[rand() % i]);
    }catch(const char* msg){
        cerr << msg << "0 <= lo < hi <= " << size << endl;
    }
}

template <class T>
void mergeTwoOrderedArray(const Array<T>& A, const Array<T>& B, Array<T> &R){
    for(int i = 0, j = 0; (i < A.size) || (j < B.size);){
        if((i < A.size) && (!(j < B.size) || (A.elems[i] <= B.elems[j]))) R.insert(A.elems[i++]);
        if((j < B.size) && (!(i < A.size) || (B.elems[j] < A.elems[i]))) R.insert(B.elems[j++]);
    }
}