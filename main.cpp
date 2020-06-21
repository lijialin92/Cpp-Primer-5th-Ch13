#include <iostream>
#include <vector>
#include <memory>

/**exercise 13.1
 * 拷贝构造函数的第一个参数是自身类类型的引用，之后的参数都要有默认值。
 * 当需要拷贝初始化的时候用到它。以下5种行为会用到拷贝初始化：
 * 1. 用=定义变量（String a = "aaa"）
 * 2. 传递对象作为实参给函数的非引用类型的形参的时候
 * 3. 函数返回非引用类型的对象
 * 4. 用花括号列表初始化数组中的元素或这一个聚合类中的成员。
 * 5. 调用初始化标准库容器或者调用insert和push的时候。*/

/**exercise 13.2
 * 我认为这个是合法的构造函数，但是作为拷贝构造函数是不合法的。
 * 这个问题应该是在问：为什么拷贝构造函数要求参数是引用类型。
 * 举个例子：
 * 定义一个函数参数是非引用类型f(A a){使用a}， 假设实参是A aa，然后调用f(aa)。这是需要需要用到A类中定义的拷贝构造函数。
 * 如果A类中定义的拷贝构造函数是这个样子的：A:A(A a){使用a}。那么调用这个拷贝构造函数需要继续调用A类的拷贝构造函数，因为这是把一个
 * 对象传递给一个函数的形参作为参数。这样下去永无止境，是个死循环。*/

/**exercise 13.3
 * 因为StrBlob类中没有定义复制构造函数，所以用编译器自动合成的默认构造函数。合成的复制构造函数将需要复制的对象的每个非static成员复
 * 制到创建的对象中去。(例如StrBlob a2 = a1(一个之前已经创建号的StrBlob对象))每个成员的类型决定了如何拷贝进新的对象中去。内置类型
 * 直接拷贝；类类型，就是用自己类的拷贝构造函数拷贝。StrBlob中只有一个成员变量data，是shared_ptr类型的。所以拷贝这个成员的时候就
 * 调用shared_ptr的拷贝构造函数。
 * StrBlobPtr一样的，逐个拷贝自己类型中的成员变量。唯一需要注意的就是shared_ptr拷贝的时候引用计数会+1而StrBlobPtr中的weak_ptr拷
 * 贝的时候，这个weak_ptr依靠的shared_ptr的引用是不+1的。*/

/**exercise 13.4*/
/*Point global;
Point foo_bar(Point arg)//当需要传参的时候需要复制构造函数
{
    Piont local = arg, *//**这里是复制构造初始化，用到了复制构造函数*//* *heap = new Point(global);
    *heap = local; //这里应该是普通的复制，而不用到复制构造函数?
    Point pa[4] = {local, *heap}; //这里用到了复制构造函数，这里用花括号列表初始化数组中的元素
    return *heap; //这里然会的对象是Point类型的对象，不是引用。所以调用这个函数的那边，还是需要复制构造函数。
}*/

/**exercise 13.5*/
class HasPtr{
public:
    HasPtr(const std::string& s = std::string()) :
    ps(new std::string(s)), i(0) {}
    HasPtr(HasPtr& hasPtr) : ps(new std::string(*(hasPtr.ps))), i(hasPtr.i) {}
    HasPtr& operator=(const HasPtr(&));
    ~HasPtr(); //exercise 13.11

private:
    std::string* ps;
    int i;
};

/**exercise 13.6
 * 拷贝赋值运算符：
 * 本身是一个重载的赋值运算符，定义为类的成员函数。左侧运算对象绑定到隐含的this参数，右侧运算对象是与左侧相同类类型的，作为函数的参
 * 数，函数的返回指向其左侧对象的引用。
 * 类对象赋值给另外一个类对象的时候，会用到拷贝赋值运算符。例如 a2 = a1, a1, a2 是相同类类型的对象;
 * 将右侧对象的成员(非static成员)一个一个赋值给左侧对象的对应成员。
 * 如果一个类没有定义自己的拷贝赋值运算符，那么编译器就为这个类生成一个合成拷贝运算符。对于有些类，还可以定义自己的拷贝赋值运算符来
 * 起到禁止类对象赋值的效果。*/

/**exercise 13.7
 * 由于两个类都未定义拷贝赋值运算符，所以编译器自动生成一个合成拷贝赋值运算符给这两个类。
 * 当StrBlob 对象相互赋值的时候，把data成员赋值。shared_ptr引用次数+1。StrBlobPtr赋值weakptr的时候，用weakptr的类内定义的拷贝赋
 * 值运算符赋值，curr直接进行内存复制。*/

/**exercise 13.8*/
HasPtr& HasPtr::operator=(const HasPtr& hasPtr) {
    if(!ps)
        delete ps;
    ps = new std::string(*(hasPtr.ps));
    i = hasPtr.i;
    return *this;
}

/**exercise 13.9
 * 析构函数用来释放对象所使用的资源，并销毁对象的非static成员。
 *
 * 一下情况会自动调用析构函数：
 * 1. 变量离开作用域。
 * 2. 销毁对象的成员的时候。
 * 3. 容器被销毁的时候，里面的元素被销毁。
 * 4. 销毁动态内存的对象。（delete指针，指向的对象被销毁，同样是调用这个对象的析构函数）
 * 5. 对于临时变量，当创建它的表达式结束的时候。
 *
 * 当一个类没有定义自己的析构函数的时候，编译器就为这个类生成一个合成析构函数。*/

/**exercise 13.10
 * 当strBlob对象被销毁的时候先执行析构函数的函数体，然后销毁它对象中的成员。data被销毁时候，需要调用shared_ptr的析构函数，如果引用
 * 次数变为0则释放动态内存空间。
 * 当StrblobPtr被销毁的时候先执行析构函数的函数体，然后依次销毁成员weakptr和curr.weakptr需要weakptr的类的析构函数，curr是内置类
 * 型，所以自动销毁。*/

/**exercise 13.11
 * look at exercise 13.5*/
HasPtr::~HasPtr() {delete ps;}

/**exercise 13.12
 * 三次，当item1和item2离开作用域的时候需要被销毁。调用两此Sales_data类的析构函数，用于销毁这两个对象。accum是被拷贝构造函数复制
 * 外面的对象，复制到函数里面来的。所以当函数结束的时候，accum也需要调用Sales_data的析构函数来销毁。*/

/**exercise 13.13*/
struct X{
    X(){std::cout << "X()";}
    X(const X& x) {std::cout << "X(const X&)";}
    ~X() {std::cout << "~X()";}
    X& operator=(const X& x1) {std::cout << "operator=";
        return *this;}
};

void f(X x) { }
void f1(X& x) { }
void exercise13_13_1(){
    //作为引用和非引用进行参数传递
    X x1;
    f(x1);
    std::cout << "now is reference: ";
    f1(x1);
}

void exercise13_13_2(){
    //动态分配
    X* x1 = new X;
    delete(x1);
}

void exercise13_13_3(){
    //容器中的元素
    X x;
    std::vector<X> x1(2);
    x1.push_back(x);
}

void exercise13_13_4(){
    //局部变量
    X x;
}

void exercise13_13_5(){
    //用一个对象初始化另外一个对象
    X x1;
    X x2(x1);
}

/**exercise 13.14
 * 输出相同的序列号*/

/**exercise 13.15
 * 改变了。使用拷贝构造函数，拷贝初始化b，c。但是输出的序列号并不是a.b,c本来的序列号。一位f()传递对象而不是引用，所以每次调用都需要拷贝构造函数，都会生成新的序列号，
 * 然后输出。*/

/**exercise 13.16
 * 会改变输出结果，因为调用f()的时候不再需要使用拷贝构造函数，因为f()的参数是引用。可以输出a，b，c本来的序列号。*/

/**exercise 13.17*/
struct numbered{

    inline static int seq;

    numbered() {mysn = seq++;}
    numbered(const numbered& a) {mysn = seq++;}
    int mysn;
};

void f(numbered a){std::cout << a.mysn << std::endl;}
void f1(const numbered& a){std::cout << a.mysn << std::endl;}

void exercise13_15(){
    numbered a, b = a, c = b;
    f(a);
    f(b);
    f(c);
}

void exercise13_16(){
    numbered a, b = a, c = b;
    f1(a);
    f1(b);
    f1(c);
}

/**exercise 13.18*/
class Employee{
private:
    static int seq;
public:
    std::string name;
public:
    Employee() {seq++;}
    Employee(const std::string& s) {name = s; seq++;}
    Employee(const Employee& e) {name = e.name; seq++;}
    Employee& operator=(Employee& e) = default;
};

/**exercise 13.19
 * see exercise 13.18;
 * 需要定义自己的拷贝构造函数，因为拷贝初始化时候序列号需要变化 Emloyee a(b)。a，b的序列号不是一样的。
 * 但是我们希望赋值的时候 =左侧和右侧的对象有相同的序列号。c = b，b，c的序列号是一样的。*/

/**exercise 13.20
 * 因为没有定义自己的拷贝构造函数和拷贝赋值运算符，所以用编译器合成的。
 * 拷贝：把file成员拷贝给新的对象，拷贝的时候会调用shared_ptr的拷贝构造函数。两个shared_ptr指向相同的vector<string>。并且这个shared_ptr的引用计数+1。
 * 把wm同样拷贝给新的对象，拷贝的时候会调用map的拷贝构造函数，拷贝map中的每个元素。string将调用string的拷贝构造函数，shared_ptr调用自己构造函数。
 * 对于QueryResult, sought,file都调用自己的类的拷贝构造函数，lines是int的类型的，直接复制内存。
 * 赋值：
 * 销毁：*/

/**exercise 13.21
 * 不需要，因为这两个类的成员类型都是标准库中的（vector，shared_ptr，string，map，set）。如果两个对象需要共享，智能指针可以很好的
 * 实现，并不会出现new时候出现的问题。*/

/**exercise 13.22
 * see later exercise*/

/**exercise 13.23
 * no difference*/

/**exercise 13.24
 * 默认构造函数销毁一个对象的时候，只会销毁ps指针，而不会销毁ps指向的内存。造成内存泄漏。
 *
 * 如果未定义拷贝构造函数，两个对象会有相同的ps指针，指向相同的内存。当析构函数销毁其中一个对象的时候，会释放这个ps指向的内存。另外
 * 和这个ps指向相同对象的ps就成了空悬指针。*/

/**exercise 13.25
 * 拷贝构造函数：生成一个新的vector<string>,然后用一个StrBlob对象的ptr指向它。
 * 拷贝复制运算符：让对象可以自己给自己复制，如果出翔异常，不会产生问题。
 * 析构函数不需要的，因为shared_ptr如果引用为0，会自动销毁所指向的内存空间。不用手动delete销毁。*/

/**exercise 13.26*/
class StrBlob{
public:
    StrBlob() = default;
    StrBlob(std::vector<std::string>*);
    StrBlob(const StrBlob&);
    StrBlob& operator=(const StrBlob&);
    void push_back(const std::string& s) {data->push_back(s);}

public:
    std::shared_ptr<std::vector<std::string>> data;
};

StrBlob::StrBlob(std::vector<std::string>* p){
    data = std::make_shared<std::vector<std::string>>(*p);
    delete p;
    std::cout << "use of normal constructor." << std::endl;
}

inline StrBlob::StrBlob(const StrBlob& sb){
    data = std::make_shared<std::vector<std::string>>(*sb.data);
    std::cout << "use of normal copy constructor." << std::endl;
}

inline StrBlob& StrBlob::operator=(const StrBlob& sb) {
    data = std::make_shared<std::vector<std::string>>(*sb.data);
    std::cout << "use of operator= constructor." << std::endl;
    return *this;
}

void exercise13_26(){
    using namespace std;
    auto vecPtr = new vector<string>();
    vecPtr->push_back("aaa");
    vecPtr->push_back("bbb");
    vecPtr->push_back("ccc");
    StrBlob b1(vecPtr);
    for(const auto& i : *b1.data)
        cout << i << endl;
    StrBlob b2(b1);
    for(const auto& i : *b2.data)
        cout << i << endl;
    StrBlob b3;
    b3 = b2;
    for(const auto& i : *b3.data)
        cout << i << endl;
}

int main() {
    exercise13_26()
    ;
    return 0;
}
