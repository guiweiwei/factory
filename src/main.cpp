#include "templateFactory.h"
#include "test.h"

class F
{
public:
	F(int i) {}
};

void test(DEFAULT_FACTORY_HOLDER* factory)
{
	auto z = factory->createHeader(2.0);
}

int main()
{
	TemplateFactory<F, pool_mode>* factory = new TemplateFactory<F, pool_mode>;

	F *x = factory->createProduct(1);

	DEFAULT_FACTORY_HOLDER dFactory;

	auto y = dFactory.createItem(1);

	test(&dFactory);

	auto k = dFactory.createValue();
	auto k1 = dFactory.createValue(1);
	auto k2 = dFactory.createValue(2, 3);

	auto x2 = OBJECT_NEW(F, 1);

	TemplateFactory<SignalItemData, system_mode> heihei;
	heihei.createProduct();
	//DriveTest1<SignalValueInfo> tt;
	//tt.create(1, 2);
}
