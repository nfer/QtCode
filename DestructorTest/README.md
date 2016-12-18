# Qt中对象析构的一些测试验证

## 基本的C++对象创建和析构
测试代码1：
```C++
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

## Qt基于QObject的对象创建和析构
测试代码2：
```C++
#include <qDebug>

class A: public QObject {
public:
    A(QString _name) : name(_name) { qDebug() << "create A with name: " << this->name;}
    ~A() { qDebug() << "destruct A with name: " << this->name;}
private:
    QString name;
};

int main()
{
    A *pa1 = new A("pa1");
    A *pa2 = new A("pa2");

    pa1->setParent(pa2);
    delete pa2;
}
```

我们通过关键字`new`创建了两个类`A`的实例，分别是`pa1`和`pa2`。因为类`A`继承于Qt的內建类`QObject`，所以可以使用接口`void setParent(QObject *parent)`将对象`pa2`设置为对象`pa1`的父元素。

我们先看一下程序的运行结果：
```
create A with name:  "pa1"
create A with name:  "pa2"
destruct A with name:  "pa2"
destruct A with name:  "pa1"
```
注意，我们通过`delete`关键字只是删除了对象`pa2`，并没有删除对象`pa1`，为什么输出显示系统也对`pa1`进行了析构呢？？？

> QObjects organize themselves in object trees. When you create a QObject with another object as parent, it's added to the parent's children() list, and is deleted when the parent is.                      -- [Object Trees & Ownership](http://doc.qt.io/qt-5/objecttrees.html)

通过上面的官方说明，我们了解到，Qt会根据父子关系进行自动的资源回收，当父元素被析构的时候会自动调用子元素的析构。对应到我们的测试代码就是：
 - 通过关键字`delete`删除对象`pa2`
 - 调用`pa2`的析构函数
 - 检查发现`pa2`存在子元素`pa1`
 - 自动的进行`pa1`的资源释放(`delete pa1 ???`)
