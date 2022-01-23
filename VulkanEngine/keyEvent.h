#pragma once



struct KeyEvent
{
	int key;
	int scancode;
	int action;
	int mods;

	enum ActionCode
	{
		KEY_PRESSED = 0,
		KEY_RELEASED = 1
	};
};
