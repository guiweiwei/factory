#include "templateFactory.h"
#include "test.h"

class F
{
public:
	F(int i) {}
};

void test(DEFAULT_FACTORY* factory)
{
	auto z = factory->createHeader(2.0);
}

int main()
{
	templateFactory<F, pool_mode>* factory = new templateFactory<F, pool_mode>;

	F *x = factory->createFactory(1);

	DEFAULT_FACTORY dFactory;

	auto y = dFactory.createItem(1);

	test(&dFactory);

	auto k = dFactory.createValue();
	auto k1 = dFactory.createValue(1);
	auto k2 = dFactory.createValue(2, 3);

	auto x2 = OBJECT_NEW(F, 1);

	//DriveTest1<SignalValueInfo> tt;
	//tt.create(1, 2);
}
