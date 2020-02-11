#include <iostream>

using namespace std;

template <class T> class SingleList;
template <class T>
ostream& operator<<(ostream& output, const SingleList<T>& L);
template <class T>
istream& operator>>(istream& input, SingleList<T>& L);

/*列表节点模板类（以单向链表形式实现）*/
template <class T>
struct ListNode{
    T data;//数值
    ListNode<T>* next;//后继

    //构造函数
    ListNode(){}
    ListNode(T e, ListNode<T>* n = nullptr):data(e),next(n){}

    //操作接口
    ListNode<T>* insertAsNext(const T& e){//将e紧靠当前节点之后插入当前节点所属列表
        ListNode<T>* x = new ListNode(e);
        x -> next = this -> next;
        this -> next = x;
        return x;
    }
};

/*列表模板类*/
template <class T>
class SingleList{
private:
    int size;//规模
    ListNode<T>* head;//头哨兵节点
public:
    //构造函数
    SingleList(){init();}

    //析构函数
    ~SingleList(){clear(); delete head;}

    void init();//列表创建初始化
    int clear();//清除所有节点
    friend ostream& operator<<<T>(ostream& output, const SingleList<T>& L);//重载流插入运算符，打印列表
    friend istream& operator>><T>(istream& input, SingleList<T>& L);//重载流提取运算符，输入列表

    //只读访问接口
    int getSize() const {return size;}//规模
    bool isEmpty() const {return size <= 0;}//判空
    ListNode<T>* first() const {return head -> next;}//首节点位置
    ListNode<T>* locate(int pos);//返回第pos个节点的位置
    T getPos(int pos) const;//返回第pos个节点的数值

    //可写访问接口
    ListNode<T>* insertAsFirst(const T& e);//将e当作首节点插入
    ListNode<T>* insertAsLast(const T& e);//将e当作末节点插入
    ListNode<T>* insertToPos(const T& e,int pos);//将e插入到列表的第pos个节点处，pos合法区间[0, size]
    T remove(ListNode<T>* p);//删除节点p并返回其数值
    T remove(int pos);//将第pos个节点删除，pos合法区间[0,size)
    void reverse();//前后倒置
};

template <class T>
void SingleList<T>::init(){
    head = new ListNode<T>;
    head -> next = nullptr;
    size = 0;
}

template <class T>
int SingleList<T>::clear(){
    int oldSize = size;
    while(0 < size) remove(head -> next);
    return oldSize;
}

template <class T>
ostream& operator<<(ostream& output, const SingleList<T>& L){
    output << "当前列表的规模为 " << L.size <<",具体信息如下" << endl;
    output << "head";
    ListNode<T>* p = L.head -> next;
    while(p != nullptr){
        output << ", " << p -> data;
        p = p -> next;
    }
    output << endl;
    return output;
}

template <class T>
istream& operator>>(istream& input, SingleList<T>& L){
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
ListNode<T>* SingleList<T>::locate(int pos){
    if(size == 0 && pos == -1) return head;
    if(size > 0){
        if(pos == -1) return head;
        if(pos < size){
            int i = 0;
            ListNode<T>* p = head -> next;
            while(i < pos) {p = p -> next; ++i;}
            return p;
        }
    }
    return nullptr;
}

template <class T>
T SingleList<T>::getPos(int pos) const{
    try{
        if(pos < 0 || pos >= size) throw "访问的pos不合法，";
        ListNode<T>* p = first();
        while(0 < pos--) p = p -> next;
        return p -> data;
    }catch(const char *msg){
        cerr << msg << "0 <= pos < " << size << endl;
    }
    return -1;
}

template <class T>
ListNode<T>* SingleList<T>::insertAsFirst(const T& e){
    ++size;
    return head -> insertAsNext(e);
}

template <class T>
ListNode<T>* SingleList<T>::insertAsLast(const T& e){
    return insertToPos(e, size);
}

template <class T>
ListNode<T>* SingleList<T>::insertToPos(const T& e,int pos){
    try{
        ListNode<T>* p = locate(--pos);
        if(p == nullptr) throw "插入的pos不合法，";
        ++size;
        return p -> insertAsNext(e);
    }catch(const char *msg){
        cerr << msg << "0 <= pos <= " << size << endl;
    }
    return nullptr;
}

template <class T>
T SingleList<T>::remove(ListNode<T>* p){
    try{
        if(p == nullptr) throw "删除位置不合法！";
        T e = p -> data;
        ListNode<T>* q = head;
        while(q -> next != p) q = q -> next;
        q -> next = p -> next;
        delete p;
        --size;
        return e;
    }catch(const char *msg){
        cerr << msg << endl;
    }
    return -1;
}

template <class T>
T SingleList<T>::remove(int pos){
    try{
        if(pos < 0 || pos >= size) throw "删除的pos不合法，";
        ListNode<T>* p = locate(--pos);
        return remove(p -> next);
    }catch(const char *msg){
        cerr << msg << "0 <= pos < " << size << endl;
    }
    return -1;
}

template <class T>
void SingleList<T>::reverse(){
    try{
        if(size == 0) throw "当前列表规模为0，不能对链表进行前后倒置！";
        ListNode<T> *p, *q, *r;
        p = head -> next;
        q = p -> next;
        r = nullptr;
        p -> next = nullptr;
        while(q){
            r = q -> next;
            q -> next = p;
            p = q;
            q = r;
        }
        head -> next = p;
    }catch(const char *msg){
        cerr << msg << endl;
    }
}