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
