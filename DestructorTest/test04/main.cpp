#include <qDebug>

class A: public QObject {
public:
    A(QString _name) {
        this->setObjectName(_name);
        qDebug() << "create A with obj name: " << this->objectName();
    }
    ~A() { qDebug() << "destruct A with obj name: " << this->objectName();}
};

int main()
{
    A *pa1 = new A("pa1");
    A *pa2 = new A("pa2");

    pa1->setParent(pa2);
    pa2->dumpObjectTree();

    delete pa1;
    pa2->dumpObjectTree();

    delete pa2;
}
