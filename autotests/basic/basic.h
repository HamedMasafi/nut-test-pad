#include <QObject>

class BasicTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void checkDeclartions();
    void checkPrimary();
    void insert();
    void insertWithChilds();
    void modelTest();
    void changedTest();
    void className();
    void checkExpressions();
    void join();
};
