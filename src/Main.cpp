 
#include "../includes/Engine/Engine.h"
#include <iostream>
int main()
{
	Engine &engine = Engine::instance();
	engine.init( 1920, 1080, "My engine" );
	//char cc;
	//std::cin >> cc;
	return 0;
}
