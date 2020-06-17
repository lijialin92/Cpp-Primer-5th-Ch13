#include <iostream>

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

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
