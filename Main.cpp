#include "Engine.h"

int main()
{
	Engine &engine = Engine::instance();
	engine.init();
	engine.createWindow( 800, 600, "My Engine" );
	engine.run();

	return 0;
}