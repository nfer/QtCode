
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
