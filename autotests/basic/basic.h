#include <QObject>

class BasicTest : public QObject
{
private Q_SLOTS:
    void modelTest();
    void changedTest();
};
