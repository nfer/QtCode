# Qt中对象析构的一些测试验证

## 基本的C++对象创建和析构
测试代码1：
```
#include <qDebug>

class A {
public:
    A(QString _name) : name(_name) { qDebug() << "create A with name: " << this->name;}
    ~A() { qDebug() << "destruct A with name: " << this->name;}
private:
    QString name;
};

int main()
{
    A a("a");
    A *pa = new A("pa");
    A *pa2 = new A("pa2");
    delete pa2;
}
```

 - 对象`a`在**栈**上创建，在离开**变量作用域**时会自动进行内存释放
 - 对象`pa`通过`new`关键字在**堆**上创建，在离开**变量作用域**后并不会自动进行内存释放
 - 对象`pa2`也是在**堆**上创建的，但是在离开**变量作用域**时手动通过`delete`关键字进行内存释放

上述的代码运行结果如下:
```
create A with name:  "a"
create A with name:  "pa"
create A with name:  "pa2"
destruct A with name:  "pa2"
destruct A with name:  "a"
```

说明：

 1. 严格的来讲，`pa`和`pa2`并不是对象，而是指向对象的指针
 2. `pa`指向的对象在程序退出的时候也会被系统进行内存释放
 3. 注意，手动释放的`pa2`早于`a`的析构，原因在于**栈**上的空间是在退出**变量作用域**的时候才进行的内存释放操作

