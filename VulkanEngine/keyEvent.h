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
		KEY_PRESSED = 0,
		KEY_RELEASED = 1,
		KEY_HELD
	};
};
