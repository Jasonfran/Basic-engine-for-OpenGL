 
#include "../includes/Engine/Engine.h"
#include <iostream>
int main()
{
	Engine &engine = Engine::instance();
	engine.init( 800, 800, "My engine" );
	//char cc;
	//std::cin >> cc;
	return 0;
}
