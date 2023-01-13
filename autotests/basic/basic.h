#include <QObject>

class BasicTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void modelTest();
    void changedTest();
    void className();
    void checkExpressions();
};
