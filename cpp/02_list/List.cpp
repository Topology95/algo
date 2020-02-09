#include <iostream>

using namespace std;

template <class T> class List;
template <class T>
ostream& operator<<(ostream& output, const List<T>& L);
template <class T>
istream& operator>>(istream& input, List<T>& L);

/*列表节点模板类（以双向链表形式实现）*/
template <class T>
struct ListNode{
    T data;//数值
    ListNode<T>* pred;//前驱
    ListNode<T>* succ;//后继

    //构造函数
    ListNode(){}
    ListNode(T e, ListNode<T>* p = nullptr, ListNode<T>* s = nullptr) : data(e), pred(p), succ(s){}

    //操作接口
    ListNode<T>* insertAsPred(const T& e);//紧靠当前节点之前插入新节点
    ListNode<T>* insertAsSucc(const T& e);//紧靠当前节点之后插入新节点
};

template <class T>
ListNode<T>* ListNode<T>::insertAsPred(const T& e){
    ListNode<T>* x = new ListNode(e, pred, this);
    pred -> succ = x;
    pred = x;
    return x;
}

template <class T>
ListNode<T>* ListNode<T>::insertAsSucc(const T& e){
    ListNode<T>* x = new ListNode(e, this, succ);
    succ -> pred = x;
    succ = x;
    return x;
}

/*列表模板类*/
template <class T>
class List{
private:
    int size;//规模
    ListNode<T>* header;//头哨兵节点
    ListNode<T>* trailer;//尾哨兵节点
public:
    //构造函数
    List(){init();}

    //析构函数
    ~List(){clear(); delete header; delete trailer;};

    void init();//列表创建初始化
    friend ostream& operator<<<T>(ostream& output, const List<T>& L);//重载流插入运算符，打印列表
    friend istream& operator>><T>(istream& input, List<T>& L);//重载流提取运算符，输入列表
    int clear();//清除所有节点

    //只读访问接口
    int getSize() const {return size;}//规模
    bool isEmpty() const {return size <= 0;}//判空
    ListNode<T>* first() const {return header -> succ;}//首节点位置
    ListNode<T>* last() const {return trailer -> pred;}//末节点位置
    bool valid(ListNode<T>* p){return p && (header != p) && (trailer != p);}//判断位置p对外是否合法
    int disordered() const;//判断列表是否已排序，返回列表中相邻逆序节点对的总数
    ListNode<T>* find(const T& e, int n, ListNode<T>* p) const;//无序区间查找
    ListNode<T>* find(const T& e) const {return find(e, size, trailer);}//无序列表查找
    ListNode<T>* search(const T& e, int n, ListNode<T>* p) const;//有序区间查找
    ListNode<T>* search(const T& e) const {return search(e, size, trailer);}//有序列表查找
    ListNode<T>* selectMax(ListNode<T>* p, int n);//在p及其n-1个后继中选出最大者
    ListNode<T>* selectMax(){return selectMax(header -> succ, size);}//整体最大者

    //可写访问接口
    ListNode<T>* insertAsFirst(const T& e);//将e当作首节点插入
    ListNode<T>* insertAsLast(const T& e);//将e当作尾结点插入
    ListNode<T>* insertA(ListNode<T>* p, const T& e);//将e当作p的后继插入
    ListNode<T>* insertB(ListNode<T>* p, const T& e);//将e当作p的前驱插入
    T remove(ListNode<T>* p);//删除合法节点p，返回其数值
    int deduplicate();//对无序列表作唯一化处理
    int uniquify();//对有序列表作唯一化处理
    void reverse();//前后倒置
    void insertionSort(ListNode<T>* p, int n);//对从p开始连续的n个节点作插入排序
    void selectionSort(ListNode<T>* p, int n);//对从p开始连续的n个节点作选择排序
    void merge(ListNode<T>* & p, int n, List<T>& L, ListNode<T>* q, int m);//有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
    void mergeSort(ListNode<T>* & p, int n);//对从p开始连续的n个节点作归并排序

};

template <class T>
void List<T>::init(){
    header = new ListNode<T>;
    trailer = new ListNode<T>;
    header -> succ = trailer;
    header -> pred = nullptr;
    trailer -> pred = header;
    trailer -> succ = nullptr;
    size = 0;
}

template <class T>
ostream& operator<<(ostream& output, const List<T>& L){
    output << "当前列表：header " ;
    ListNode<T>* p = L.header -> succ;
    while(p != L.trailer){
        output << p -> data << " ";
        p = p -> succ;
    }
    output << "trailer" << endl;
    return output;
}

template <class T>
istream& operator>>(istream& input, List<T>& L){
    cout << "请输入列表节点的个数为：";
    int count;
    input >> count;
    cout << "请输入元素的数值：";
    T data;
    for(int i = 0; i < count; ++i){
        input >> data;
        L.insertAsLast(data);
    }
    return input;
}

template <class T>
int List<T>::clear(){
    int oldSize = size;
    while(0 < size) remove(header -> succ);
    return oldSize;
}

template <class T>
int List<T>::disordered() const{
    int count = 0;
    ListNode<T>* p = first() -> succ;
    while(p != trailer){
        if(p -> pred -> data > p -> data) ++count;
        p = p -> succ;
    }
    return count;
}

template <class T>//在无序列表内节点p的n个前驱中，找到等于e的最后者
ListNode<T>* List<T>::find(const T& e, int n, ListNode<T>* p) const{
    while(0 < n--){
        if(e == (p = p -> pred) -> data) return p;
    }
    return nullptr;
}

template <class T>//在有序列表内节点p的n个前驱中，找到不大于e的最后者
ListNode<T>* List<T>::search(const T& e, int n, ListNode<T>* p) const{
    while(0 <= n--){
        if((p = p -> pred) -> data <= e) break;
    }
    return p;
}

template <class T>
ListNode<T>* List<T>::selectMax(ListNode<T>* p, int n){
    ListNode<T>* max = p;
    for(ListNode<T>* cur = p; 1 < n; --n){
        if(max -> data <= (cur = cur -> succ) -> data) max = cur;
    }
    return max;
}

template <class T>
ListNode<T>* List<T>::insertAsFirst(const T& e){
    ++size;
    return header -> insertAsSucc(e);
}

template <class T>
ListNode<T>* List<T>::insertAsLast(const T& e){
    ++size;
    return trailer -> insertAsPred(e);
}

template <class T>
ListNode<T>* List<T>::insertA(ListNode<T>* p, const T& e){
    ++size;
    return p -> insertAsSucc(e);
}

template <class T>
ListNode<T>* List<T>::insertB(ListNode<T>* p, const T& e){
    ++size;
    return p -> insertAsPred(e);
}

template <class T>
T List<T>::remove(ListNode<T>* p){
    try{
        if(!valid(p)) throw "删除位置不合法";
        T e = p -> data;
        p -> pred -> succ = p -> succ;
        p -> succ -> pred = p -> pred;
        delete p;
        --size;
        return e;
    }catch(const char* msg){
        cerr << msg << endl;
    }
    return -1;
}

template <class T>
int List<T>::deduplicate(){
    if(size < 2) return 0;
    int oldSize = size;
    ListNode<T>* p = header;
    int r = 0;
    while(trailer != (p = p -> succ)){
        ListNode<T>* q = find(p -> data, r, p);
        q ? remove(q) : r++;
    }
    return oldSize - size;
}

template <class T>
int List<T>::uniquify(){
    if(size < 2) return 0;
    int oldSize = size;
    ListNode<T>* p = header -> succ;
    ListNode<T>* q;
    while(trailer != (q =  p -> succ)){
        if(p -> data != q -> data) p = q;
        else remove(q);
    }
    return oldSize - size;
}

template <class T>
void List<T>::reverse(){
    ListNode<T>* p = last();
    while(p != first()) insertA(p, remove(first()));
}

template <class T>
void List<T>::insertionSort(ListNode<T>* p, int n){
    try{
        if(!valid(p)) throw "节点位置不合法";
        if(n < 2) return;
        for(int r = 0; r < n; ++r){
            insertA(search(p -> data, r, p), p -> data);
            p = p -> succ;
            remove(p -> pred);
        }
    }catch(const char* msg){
        cerr << msg << endl;
    }
}

template <class T>
void List<T>::selectionSort(ListNode<T>* p, int n){
    try{
        if(!valid(p)) throw "节点位置不合法";
        if(n < 2) return;
        ListNode<T>* head = p -> pred;
        ListNode<T>* tail = p;
        for(int i = 0; i < n; ++i) tail = tail -> succ;//待排序区间为(head, tail)
        while(1 < n){
            ListNode<T>* max = selectMax(head -> succ, n);//找出最大者，有重复元素优先选后者
            insertB(tail, remove(max));
            tail = tail -> pred;
            --n;
        }
    }catch(const char* msg){
        cerr << msg << endl;
    }
}

template <class T>
void List<T>::merge(ListNode<T>* & p, int n, List<T>& L, ListNode<T>* q, int m){
    ListNode<T>* pp = p -> pred;
    while(0 < m){//在q尚未移出区间之前
        if((0 < n) && (p -> data <= q -> data)){//若p仍在区间内且v(p)<=v(q)
            if(q == (p = p -> succ)) break;
            --n;
        }
        else{//若p已超出右界，或者v(q)<v(p)
            insertB(p, L.remove((q = q -> succ) -> pred));//将q转移至p之前
            --m;
        }
    }
    p = pp -> succ;//确定归并后区间的新起点
}

template <class T>
void List<T>::mergeSort(ListNode<T>* & p, int n){
    try{
        if(!valid(p)) throw "节点位置不合法";
        if(n < 2) return;
        int m = n >> 1;
        ListNode<T>* q = p;
        for(int i = 0; i < m; ++i) q = q -> succ;
        mergeSort(p, m);
        mergeSort(q, n - m);
        merge(p, m, *this, q, n - m);//注意：排序后，p依然指向归并后区间的新起点
    }catch(const char* msg){
        cerr << msg << endl;
    }
}