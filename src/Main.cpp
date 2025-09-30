#include "./BrickBuilderApp.h"
#include "DX12Engine/Launcher.h"

int main() 
{
	int windowSize[2] = { 1920, 1080 };
	BrickBuilder::BrickBuilder app;
	DX12Engine::Launcher::Launch(&app, windowSize, "Brick Builder");
}