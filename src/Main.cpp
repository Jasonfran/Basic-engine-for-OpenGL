 
#include "../includes/Engine.h"

int main()
{
	Engine &engine = Engine::instance();
	engine.init( 800, 600, "My engine" );
	_CrtDumpMemoryLeaks();
	return 0;
}
