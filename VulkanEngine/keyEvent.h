#pragma once



struct KeyEvent
{
	bool operator==(KeyEvent _event)
	{
		if (key == _event.key)
		{
			if (scancode == _event.scancode)
			{
				if (action == _event.action)
				{
					if (mods == _event.mods)
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	int key;
	int scancode;
	int action;
	int mods;

	enum ActionCode
	{
		KEY_PRESSED = 1,
		KEY_RELEASED = 0,
		KEY_HELD = 2
	};
	enum Mod
	{
		SHIFT = 0x0001,
		CONTROL = 0x0002,
		ALT = 0x0004,
		SUPER = 0x0008,
		CAPS_LOCK = 0x0010,
		NUM_LOCK = 0x0020
	};
};
