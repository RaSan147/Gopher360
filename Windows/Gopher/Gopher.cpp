#include "Gopher.h"
#include "ConfigFile.h"

// Description:
//   Send a keyboard input to the system based on the key value
//     and its event type.
//
// Params:
//   cmd    The value of the key to send(see http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx)
//   flag   The KEYEVENT for the key
void inputKeyboard(WORD cmd, DWORD flag)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = cmd;
	input.ki.dwFlags = flag;
	SendInput(1, &input, sizeof(INPUT));
}

// Description:
//   Send a keyboard input based on the key value with the "pressed down" event.
//
// Params:
//   cmd    The value of the key to send
void inputKeyboardDown(WORD cmd)
{
	inputKeyboard(cmd, 0);
}

// Description:
//   Send a keyboard input based on the key value with the "released" event
//
// Params:
//   cmd    The value of the key to send
void inputKeyboardUp(WORD cmd)
{
	inputKeyboard(cmd, KEYEVENTF_KEYUP);
}

// Description:
//   Send a mouse input based on a mouse event type.
//   See https://msdn.microsoft.com/en-us/library/windows/desktop/ms646310(v=vs.85).aspx
//
// Params:
//   dwFlags    The mouse event to send
//   mouseData  Additional information needed for certain mouse events (Optional)
void mouseEvent(DWORD dwFlags, DWORD mouseData = 0)
{
	INPUT input;
	input.type = INPUT_MOUSE;

	// Only set mouseData when using a supported dwFlags type
	if (dwFlags == MOUSEEVENTF_WHEEL ||
		dwFlags == MOUSEEVENTF_XUP ||
		dwFlags == MOUSEEVENTF_XDOWN ||
		dwFlags == MOUSEEVENTF_HWHEEL)
	{
		input.mi.mouseData = mouseData;
	}
	else
	{
		input.mi.mouseData = 0;
	}

	input.mi.dwFlags = dwFlags;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));
}

Gopher::Gopher(CXBOXController* controller)
	: _controller(controller)
{
}

// Description:
//   Reads and parses the configuration file, assigning values to the 
//     configuration variables.
void Gopher::loadConfigFile()
{
	ConfigFile cfg("config.ini");

	//--------------------------------
	// Configuration bindings
	//--------------------------------
	CONFIG_MOUSE_LEFT = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_LEFT").c_str(), 0, 0);
	CONFIG_MOUSE_RIGHT = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_RIGHT").c_str(), 0, 0);
	CONFIG_MOUSE_MIDDLE = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_MIDDLE").c_str(), 0, 0);
	CONFIG_HIDE = strtol(cfg.getValueOfKey<std::string>("CONFIG_HIDE").c_str(), 0, 0);
	CONFIG_DISABLE = strtol(cfg.getValueOfKey<std::string>("CONFIG_DISABLE").c_str(), 0, 0);
	CONFIG_DISABLE_VIBRATION = strtol(cfg.getValueOfKey<std::string>("CONFIG_DISABLE_VIBRATION").c_str(), 0, 0);
	CONFIG_SCROLL_SPEED_CHANGE = strtol(cfg.getValueOfKey<std::string>("CONFIG_SCROLL_SPEED_CHANGE").c_str(), 0, 0);
	CONFIG_SPEED_CHANGE = strtol(cfg.getValueOfKey<std::string>("CONFIG_SPEED_CHANGE").c_str(), 0, 0);
	CONFIG_OSK = strtol(cfg.getValueOfKey<std::string>("CONFIG_OSK").c_str(), 0, 0);

	//--------------------------------
	// Controller bindings
	//--------------------------------
	GAMEPAD_DPAD_UP = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_UP").c_str(), 0, 0);
	GAMEPAD_DPAD_DOWN = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_DOWN").c_str(), 0, 0);
	GAMEPAD_DPAD_LEFT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_LEFT").c_str(), 0, 0);
	GAMEPAD_DPAD_RIGHT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_RIGHT").c_str(), 0, 0);
	GAMEPAD_START = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_START").c_str(), 0, 0);
	GAMEPAD_BACK = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_BACK").c_str(), 0, 0);
	GAMEPAD_LEFT_THUMB = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_THUMB").c_str(), 0, 0);
	GAMEPAD_RIGHT_THUMB = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_THUMB").c_str(), 0, 0);
	GAMEPAD_LEFT_SHOULDER = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_SHOULDER").c_str(), 0, 0);
	GAMEPAD_LEFT_SHOULDER__LT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_SHOULDER__LT").c_str(), 0, 0);
	GAMEPAD_LEFT_SHOULDER__RT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_SHOULDER__RT").c_str(), 0, 0);
	GAMEPAD_RIGHT_SHOULDER = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_SHOULDER").c_str(), 0, 0);
	GAMEPAD_RIGHT_SHOULDER__LT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_SHOULDER__LT").c_str(), 0, 0);
	GAMEPAD_RIGHT_SHOULDER__RT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_SHOULDER__RT").c_str(), 0, 0);
	GAMEPAD_A = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_A").c_str(), 0, 0);
	GAMEPAD_B = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_B").c_str(), 0, 0);
	GAMEPAD_X = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_X").c_str(), 0, 0);
	GAMEPAD_Y = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_Y").c_str(), 0, 0);
	GAMEPAD_TRIGGER_LEFT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_LEFT").c_str(), 0, 0);
	GAMEPAD_TRIGGER_RIGHT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_RIGHT").c_str(), 0, 0);

	//--------------------------------
	// Advanced settings
	//--------------------------------

	// Acceleration factor
	acceleration_factor = strtof(cfg.getValueOfKey<std::string>("ACCELERATION_FACTOR").c_str(), 0);

	// Dead zones
	DEAD_ZONE = strtol(cfg.getValueOfKey<std::string>("DEAD_ZONE").c_str(), 0, 0);
	if (DEAD_ZONE == 0)
	{
		DEAD_ZONE = 6000;
	}

	SCROLL_DEAD_ZONE = strtol(cfg.getValueOfKey<std::string>("SCROLL_DEAD_ZONE").c_str(), 0, 0);
	if (SCROLL_DEAD_ZONE == 0)
	{
		SCROLL_DEAD_ZONE = 5000;
	}

	/*scroll_speed = strtof(cfg.getValueOfKey<std::string>("SCROLL_SPEED").c_str(), 0);
	if (scroll_speed < 0.00001f)
	{
		scroll_speed = 0.1f;
	}*/

	// Variable scroll speeds
	std::istringstream scroll_speeds_str = std::istringstream(cfg.getValueOfKey<std::string>("SCROLL_SPEEDS"));
	
	scroll_speed_idx = 1;

	const float SCROLL_SPEED_MIN = 0.0001f;
	const float SCROLL_SPEED_MAX = 1.0f;
	for (std::string scrl_speed; std::getline(scroll_speeds_str, scrl_speed, ',');)
	{
		std::istringstream speed_entry = std::istringstream(scrl_speed);
		std::string speed_name, speed_value_s;
		// Check to see if we are at the string that includes the equals sign.
		if (scrl_speed.find_first_of('=') != std::string::npos)
		{
			std::getline(speed_entry, speed_name, '=');
		}
		else
		{
			std::ostringstream tmp_name;
			tmp_name << scroll_speed_idx++;
			speed_name = tmp_name.str();
		}
		std::getline(speed_entry, speed_value_s);
		float speed_value = strtof(speed_value_s.c_str(), 0);
		// Ignore speeds that are not within the allowed range.
		if (speed_value > SCROLL_SPEED_MIN && speed_value <= SCROLL_SPEED_MAX)
		{
			scroll_speeds.push_back(speed_value);
			scroll_speed_names.push_back(speed_name);
		}
	}

	// If no scroll speeds were defined, add a set of default speeds.
	std::cout << "scroll_speeds.size(): " << scroll_speeds.size() << std::endl;
	if (scroll_speeds.size() == 0)
	{
		scroll_speeds.push_back(SCROLL_SPEED_ULTRALOW);
		scroll_speeds.push_back(SCROLL_SPEED_LOW);
		scroll_speeds.push_back(SCROLL_SPEED_MED);
		scroll_speeds.push_back(SCROLL_SPEED_HIGH);
		scroll_speed_names.push_back("ULTRALOW");
		scroll_speed_names.push_back("LOW");
		scroll_speed_names.push_back("MED");
		scroll_speed_names.push_back("HIGH");

		scroll_speed = scroll_speeds[2];
		scroll_speed_idx = 2;
	}
	else {
		scroll_speed_idx = 0;
		// Initialize the scroll speed to the first speed stored in the configuration file.
		scroll_speed_idx = strtol(cfg.getValueOfKey<std::string>("SCROLL_SPEED_ID").c_str(), 0, 0);
		scroll_speed = scroll_speeds[scroll_speed_idx];  // Initialize the scroll speed to the first speed stored.
		std::cout << "Current scroll_speed_idx: " << scroll_speed_idx << " \t With scroll_speed: " << scroll_speed << std::endl;
	}
	std::istringstream cursor_speeds_str = std::istringstream(cfg.getValueOfKey<std::string>("CURSOR_SPEEDS"));
	cursor_speed_idx = 1;
	const float CUR_SPEED_MIN = 0.0001f;
	const float CUR_SPEED_MAX = 1.0f;
	for (std::string cur_speed; std::getline(cursor_speeds_str, cur_speed, ',');)
	{
		std::istringstream cursor_speed_entry = std::istringstream(cur_speed);
		std::string cur_name, cur_speed_s;
		// Check to see if we are at the string that includes the equals sign.
		if (cur_speed.find_first_of('=') != std::string::npos)
		{
			std::getline(cursor_speed_entry, cur_name, '=');
		}
		else
		{
			std::ostringstream tmp_name;
			tmp_name << cursor_speed_idx++;
			cur_name = tmp_name.str();
		}
		std::getline(cursor_speed_entry, cur_speed_s);
		float cur_speedf = strtof(cur_speed_s.c_str(), 0);
		// Ignore speeds that are not within the allowed range.
		if (cur_speedf > CUR_SPEED_MIN && cur_speedf <= CUR_SPEED_MAX)
		{
			cursor_speeds.push_back(cur_speedf);
			cursor_speed_names.push_back(cur_name);
		}
	}

	// If no cursor speeds were defined, add a set of default speeds.
	if (cursor_speeds.size() == 0)
	{
		cursor_speeds.push_back(CURSOR_SPEED_ULTRALOW);
		cursor_speeds.push_back(CURSOR_SPEED_LOW);
		cursor_speeds.push_back(CURSOR_SPEED_MED);
		cursor_speeds.push_back(CURSOR_SPEED_HIGH);
		cursor_speed_names.push_back("ULTRALOW");
		cursor_speed_names.push_back("LOW");
		cursor_speed_names.push_back("MED");
		cursor_speed_names.push_back("HIGH");

		cursor_speed = cursor_speeds[2];
	}
	else {
		cursor_speed_idx = 0;
		// Initialize the speed to the first speed stored in the configuration file.
		cursor_speed_idx = strtol(cfg.getValueOfKey<std::string>("CURSOR_SPEED_ID").c_str(), 0, 0);
		cursor_speed = cursor_speeds[cursor_speed_idx];  // Initialize the speed to the first speed stored.
		std::cout << "Current cursor_speed_idx: " << cursor_speed_idx << " \t With cursor_speed: " << cursor_speed << std::endl;
	}
	// Swap stick functions
	SWAP_THUMBSTICKS = strtol(cfg.getValueOfKey<std::string>("SWAP_THUMBSTICKS").c_str(), 0, 0);

	// Set the initial window visibility
	setWindowVisibility(_hidden);
}

// Description:
//   The main program loop. Handles the gamepad inputs and converts them
//     to system inputs based on the mapping provided by the configuration
//     file.
void Gopher::loop()
{
	Sleep(SLEEP_AMOUNT);

	_currentState = _controller->GetState();

	// Disable Gopher
	handleDisableButton();
	if (_disabled)
	{
		return;
	}

	// Vibration
	handleVibrationButton();

	// Mouse functions
	handleMouseMovement();
	handleScrolling();

	if (CONFIG_MOUSE_LEFT)
	{
		mapMouseClick(CONFIG_MOUSE_LEFT, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
	}
	if (CONFIG_MOUSE_RIGHT)
	{
		mapMouseClick(CONFIG_MOUSE_RIGHT, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
	}
	if (CONFIG_MOUSE_MIDDLE)
	{
		mapMouseClick(CONFIG_MOUSE_MIDDLE, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP);
	}

	// Hides the console
	if (CONFIG_HIDE)
	{
		setXboxClickState(CONFIG_HIDE);
		if (_xboxClickIsDown[CONFIG_HIDE])
		{
			toggleWindowVisibility();
		}
	}

	// Toggle the on-screen keyboard
	if (CONFIG_OSK)
	{
		setXboxClickState(CONFIG_OSK);
		if (_xboxClickIsDown[CONFIG_OSK])
		{
			// Get the otk window
			HWND otk_win = getOskWindow();
			if (otk_win == NULL)
			{
				openOnScreenKeyboard();	
				otk_win = getOskWindow();
				if (otk_win == NULL)
				{
					printf("Please start the On-screen keyboard first\n");
					return;
				}
			}
			else if (IsIconic(otk_win))
			{
				ShowWindow(otk_win, SW_RESTORE);
			}
			else
			{
				ShowWindow(otk_win, SW_MINIMIZE);
			}
		}
	}

	// Change Scroll Speed
	setXboxClickState(CONFIG_SCROLL_SPEED_CHANGE);
	if (_xboxClickIsDown[CONFIG_SCROLL_SPEED_CHANGE])
	{
		const int CHANGE_SCROLL_SPEED_VIBRATION_INTENSITY = 6500;   // Speed of the vibration motors when changing scroll speed.
		const int CHANGE_SCROLL_SPEED_VIBRATION_DURATION = 450;      // Duration of the scroll speed change vibration in milliseconds.

		std::cout << "Existing scroll speeds are: " << std::endl;
		for (unsigned int i = 0; i < scroll_speeds.size(); i++)
		{
			std::cout << scroll_speed_names[i] << " = " << scroll_speeds[i] << std::endl;
		}

		//std::cout << "Existing scroll speed index is: " << scroll_speed_idx << std::endl;
		scroll_speed_idx++;
		//std::cout << "New scroll speed index is: " << scroll_speed_idx << std::endl;
		scroll_speed_idx = scroll_speed_idx % scroll_speeds.size();

		scroll_speed = scroll_speeds[scroll_speed_idx];

		// Write the new scroll speed to the configuration file (using find and replace)(if not present, add it)
		std::string scroll_speed_id_name = "SCROLL_SPEED_ID";
		std::string scroll_speed_id_s = std::to_string(scroll_speed_idx);
		// open the file
		std::ifstream file("config.ini");
		// check if scroll_speed_id is in the file
		std::string line;
		bool found = false;
		while (std::getline(file, line))
		{
			if (line.find(scroll_speed_id_name) != std::string::npos)
			{
				found = true;
				break;
			}
		}
		// if found, replace the value
		if (found)
		{
			std::string temp_text = "";
			std::string search = scroll_speed_id_name + " = ";
			std::string replace = scroll_speed_id_name + " = " + scroll_speed_id_s;
			std::ifstream file("config.ini");
			//std::ofstream temp;
			//temp.open("temp.ini");
			while (std::getline(file, line))
			{
				if (line.find(search) != std::string::npos)
				{
					line = replace;
				}
				temp_text += line + "\n";
			}
			//temp.close();
			file.close();
			std::ofstream OutFile("config.ini");
			OutFile << temp_text;
			OutFile.close();
		}
		// if not found, add it
		else
		{
			std::ofstream file("config.ini", std::ios_base::app);
			// add the new line to the end of the
			file << std::endl;
			file << scroll_speed_id_name << "=" << scroll_speed_id_s << std::endl;
			file.close();
		}
		


		printf("Setting scroll speed to %f (%s)...\n", scroll_speed, scroll_speed_names[scroll_speed_idx].c_str());
		pulseVibrate(CHANGE_SCROLL_SPEED_VIBRATION_DURATION, CHANGE_SCROLL_SPEED_VIBRATION_INTENSITY, CHANGE_SCROLL_SPEED_VIBRATION_INTENSITY);
	}

	// Will change between the current speed values
	setXboxClickState(CONFIG_SPEED_CHANGE);
	if (_xboxClickIsDown[CONFIG_SPEED_CHANGE])
	{
		const int CHANGE_SPEED_VIBRATION_INTENSITY = 6500;   // Speed of the vibration motors when changing cursor speed.
		const int CHANGE_SPEED_VIBRATION_DURATION = 450;      // Duration of the cursor speed change vibration in milliseconds.

		//std::cout << "Existing cursor speed index " << cursor_speed_idx << " is: " << cursor_speed << std::endl;
		cursor_speed_idx++;
		//std::cout << "Changing cursor speed index to " << cursor_speed_idx << std::endl;
		cursor_speed_idx = cursor_speed_idx % cursor_speeds.size();

		cursor_speed = cursor_speeds[cursor_speed_idx];

		// Write the new cursor speed to the configuration file (using find and replace)(if not present, add it)
		std::string cursor_speed_id_name = "CURSOR_SPEED_ID";
		std::string cursor_speed_id_s = std::to_string(cursor_speed_idx);

		// open the file
		std::ifstream file("config.ini");
		// check if cursor_speed_id is in the file
		std::string line;
		bool found = false;

		while (std::getline(file, line))
		{
			if (line.find(cursor_speed_id_name) != std::string::npos)
			{
				found = true;
				break;
			}
		}

		// if found, replace the value
		if (found)
		{
			std::string temp_text = "";
			std::string search = cursor_speed_id_name + " = ";
			std::string replace = cursor_speed_id_name + " = " + cursor_speed_id_s;
			std::ifstream file("config.ini");
			//std::ofstream temp;
			//temp.open("temp.ini");
			while (std::getline(file, line))
			{
				if (line.find(search) != std::string::npos)
				{
					line = replace;
				}
				temp_text += line + "\n";
			}
			//temp.close();
			file.close();
			std::ofstream OutFile("config.ini");
			OutFile << temp_text;
			OutFile.close();
		}
		// if not found, add it
		else
		{
			std::ofstream file("config.ini", std::ios_base::app);
			// add the new line to the end of the
			file << std::endl;
			file << cursor_speed_id_name << "=" << cursor_speed_id_s << std::endl;
			file.close();
		}


		printf("Setting speed to %f (%s)...\n", cursor_speed, cursor_speed_names[cursor_speed_idx].c_str());
		pulseVibrate(CHANGE_SPEED_VIBRATION_DURATION, CHANGE_SPEED_VIBRATION_INTENSITY, CHANGE_SPEED_VIBRATION_INTENSITY);
	}

	// Update all controller keys.
	handleTriggers(GAMEPAD_TRIGGER_LEFT, GAMEPAD_TRIGGER_RIGHT);
	if (GAMEPAD_DPAD_UP)
	{
		mapKeyboard(XINPUT_GAMEPAD_DPAD_UP, GAMEPAD_DPAD_UP);
	}
	if (GAMEPAD_DPAD_DOWN)
	{
		mapKeyboard(XINPUT_GAMEPAD_DPAD_DOWN, GAMEPAD_DPAD_DOWN);
	}
	if (GAMEPAD_DPAD_LEFT)
	{
		mapKeyboard(XINPUT_GAMEPAD_DPAD_LEFT, GAMEPAD_DPAD_LEFT);
	}
	if (GAMEPAD_DPAD_RIGHT)
	{
		mapKeyboard(XINPUT_GAMEPAD_DPAD_RIGHT, GAMEPAD_DPAD_RIGHT);
	}
	if (GAMEPAD_START)
	{
		mapKeyboard(XINPUT_GAMEPAD_START, GAMEPAD_START);
	}
	if (GAMEPAD_BACK)
	{
		mapKeyboard(XINPUT_GAMEPAD_BACK, GAMEPAD_BACK);
	}
	if (GAMEPAD_LEFT_THUMB)
	{
		mapKeyboard(XINPUT_GAMEPAD_LEFT_THUMB, GAMEPAD_LEFT_THUMB);
	}
	if (GAMEPAD_RIGHT_THUMB)
	{
		mapKeyboard(XINPUT_GAMEPAD_RIGHT_THUMB, GAMEPAD_RIGHT_THUMB);
	}
	if (GAMEPAD_LEFT_SHOULDER)
	{
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			mapKeyboard(XINPUT_GAMEPAD_LEFT_SHOULDER, GAMEPAD_LEFT_SHOULDER__LT);
		}
		else if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			mapKeyboard(XINPUT_GAMEPAD_LEFT_SHOULDER, GAMEPAD_LEFT_SHOULDER__RT);
		}
		else {
			mapKeyboard(XINPUT_GAMEPAD_LEFT_SHOULDER, GAMEPAD_LEFT_SHOULDER);
		}
	}
	if (GAMEPAD_RIGHT_SHOULDER)
	{
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			mapKeyboard(XINPUT_GAMEPAD_RIGHT_SHOULDER, GAMEPAD_RIGHT_SHOULDER__LT);
		}
		else if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			mapKeyboard(XINPUT_GAMEPAD_RIGHT_SHOULDER, GAMEPAD_RIGHT_SHOULDER__RT);
		}
		else {
			mapKeyboard(XINPUT_GAMEPAD_RIGHT_SHOULDER, GAMEPAD_RIGHT_SHOULDER);
		}
	}
	if (GAMEPAD_A)
	{
		mapKeyboard(XINPUT_GAMEPAD_A, GAMEPAD_A);
	}
	if (GAMEPAD_B)
	{
		mapKeyboard(XINPUT_GAMEPAD_B, GAMEPAD_B);
	}
	if (GAMEPAD_X)
	{
		mapKeyboard(XINPUT_GAMEPAD_X, GAMEPAD_X);
	}
	if (GAMEPAD_Y)
	{
		mapKeyboard(XINPUT_GAMEPAD_Y, GAMEPAD_Y);
	}
}

// Description:
//   Sends a vibration pulse to the controller for a duration of time.
//     This is a BLOCKING call. Any inputs during the vibration will be IGNORED.
//
// Params:
//   duration   The length of time in milliseconds to vibrate for
//   l          The speed (intensity) of the left vibration motor
//   r          The speed (intensity) of the right vibration motor
void Gopher::pulseVibrate(const int duration, const int l, const int r) const
{
	if (!_vibrationDisabled)
	{
		_controller->Vibrate(l, r);
		Sleep(duration);
		_controller->Vibrate(0, 0);
	}
}

// Description:
//   Toggles the controller mapping after checking for the disable configuration command.
void Gopher::handleDisableButton()
{
	setXboxClickState(CONFIG_DISABLE);
	if (_xboxClickIsDown[CONFIG_DISABLE])
	{
		int duration = 0;   // milliseconds
		int intensity = 0;  // vibration intensity

		_disabled = !_disabled;

		if (_disabled)
		{
			// Transition to a disabled state.
			duration = 400;
			intensity = 10000;

			// Release all keys currently pressed by the Gopher mapping.
			while (_pressedKeys.size() != 0)
			{
				std::list<WORD>::iterator it = _pressedKeys.begin();

				// Handle mouse buttons
				if (*it == VK_LBUTTON)
				{
					mouseEvent(MOUSEEVENTF_LEFTUP);
				}
				else if (*it == VK_RBUTTON)
				{
					mouseEvent(MOUSEEVENTF_RIGHTUP);
				}
				else if (*it == VK_MBUTTON)
				{
					mouseEvent(MOUSEEVENTF_MIDDLEUP);
				}
				// Handle keys (TODO: support mouse X1 and X2 buttons)
				else
				{
					inputKeyboardUp(*it);
				}

				_pressedKeys.erase(it);
			}
		}
		else
		{
			duration = 400;
			intensity = 65000;
		}

		pulseVibrate(duration, intensity, intensity);
	}
}

// Description:
//   Toggles the vibration support after checking for the diable vibration command. 
//   This function will BLOCK to prevent rapidly toggling the vibration.
void Gopher::handleVibrationButton()
{
	setXboxClickState(CONFIG_DISABLE_VIBRATION);
	if (_xboxClickIsDown[CONFIG_DISABLE_VIBRATION])
	{
		_vibrationDisabled = !_vibrationDisabled;
		printf("Vibration %s\n", _vibrationDisabled ? "Disabled" : "Enabled");
		Sleep(1000);
	}
}

// Description:
//   Toggles the visibility of the window.
void Gopher::toggleWindowVisibility()
{
	_hidden = !_hidden;
	printf("Window %s\n", _hidden ? "hidden" : "unhidden");
	setWindowVisibility(_hidden);
}

// Description:
//   Either hides or shows the window.
// 
// Params:
//   hidden   Hides the window when true
void Gopher::setWindowVisibility(const bool& hidden) const
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, _hidden ? SW_HIDE : SW_SHOW);
}

template <typename T>
int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

// Description:
//   Determines if the thumbstick value is valid and converts it to a float.
//
// Params:
//   t  Analog thumbstick value to check and convert
//
// Returns:
//   If the value is valid, t will be returned as-is as a float. If the value is 
//     invalid, 0 will be returned.
short Gopher::getDelta(short t)
{
	//filter non-32768 and 32767, wireless ones can glitch sometimes and send it to the edge of the screen, it'll toss out some HUGE integer even when it's centered
	if (t > 32767) t = 0;
	if (t < -32768) t = 0;

	return t;
}

// Description:
//   Calculates a multiplier for an analog thumbstick based on the update rate.
//
// Params:
//   magnitude  The thumbstick magnitude in XY-plane, which is sqrt(deltaX*deltaX + deltaY*deltaY). Must be larger than deadzone.
//   deadzone   The dead zone to use for this thumbstick
//   accel      An exponent to use to create an input curve (Optional). 0 to use a linear input
//   
// Returns:
//   Multiplier used to properly scale the given thumbstick value.
float Gopher::getMult(float magnitude, float deadzone, float accel = 0.0f)
{
	// Normalize the thumbstick value (result is in range 0 to 1).
	// Note that the smallest accepted thumbstick distance is deadzone.
	// Thus, 0% would be deadzone (and below) and 100% would be (MAXSHORT - deadzone)
	float mult = (magnitude - deadzone) / (MAXSHORT - deadzone);

	// Apply a curve to the normalized thumbstick value.
	if (accel > 0.0001f)
	{
		mult = pow(mult, accel);
	}
	return mult / FPS;
}

// Description:
//   Controls the mouse cursor movement by reading the left thumbstick.
void Gopher::handleMouseMovement()
{
	POINT cursor;
	GetCursorPos(&cursor);

	short tx;
	short ty;

	if (SWAP_THUMBSTICKS == 0)
	{
		// Use left stick
		tx = getDelta(_currentState.Gamepad.sThumbLX);
		ty = getDelta(_currentState.Gamepad.sThumbLY);
	}
	else
	{
		// Use right stick
		tx = getDelta(_currentState.Gamepad.sThumbRX);
		ty = getDelta(_currentState.Gamepad.sThumbRY);
	}

	float x = cursor.x + _xRest;
	float y = cursor.y + _yRest;

	float dx = 0;
	float dy = 0;

	// Handle dead zone
	float lengthsq = tx * tx + ty * ty;
	if (lengthsq > DEAD_ZONE * DEAD_ZONE)
	{
		float mult = cursor_speed * getMult(sqrt(lengthsq), DEAD_ZONE, acceleration_factor);

		dx = tx * mult;
		dy = ty * mult;
	}

	x += dx;
	_xRest = x - (float)((int)x);

	y -= dy;
	_yRest = y - (float)((int)y);

	SetCursorPos((int)x, (int)y); //after all click input processing
}

// Description:
//   Controls the scroll wheel movement by reading the right thumbstick.
void Gopher::handleScrolling()
{
	float current_SCROLL_SPEED = scroll_speed;

	float tx;
	float ty;

	if (SWAP_THUMBSTICKS == 0)
	{
		// Use right stick
		tx = getDelta(_currentState.Gamepad.sThumbRX);
		ty = getDelta(_currentState.Gamepad.sThumbRY);
	}
	else
	{
		// Use left stick
		tx = getDelta(_currentState.Gamepad.sThumbLX);
		ty = getDelta(_currentState.Gamepad.sThumbLY);
	}

	// Handle dead zone
	float magnitudeX = abs(tx);
	float magnitudeY = abs(ty);

	// if both, only scroll in the direction with the most magnitude
	if (magnitudeX > SCROLL_DEAD_ZONE && magnitudeY > SCROLL_DEAD_ZONE)
	{
		if (magnitudeX > magnitudeY)
		{
			magnitudeY = 0;
		}
		else
		{
			magnitudeX = 0;
		}
	}

	if (magnitudeX > SCROLL_DEAD_ZONE)
	{
		float scrollX = tx * getMult(magnitudeX, SCROLL_DEAD_ZONE) * current_SCROLL_SPEED;
		mouseEvent(MOUSEEVENTF_HWHEEL, scrollX);
	}
	if (magnitudeY > SCROLL_DEAD_ZONE)
	{
		float scrollY = ty * getMult(magnitudeY, SCROLL_DEAD_ZONE) * current_SCROLL_SPEED;
		mouseEvent(MOUSEEVENTF_WHEEL, scrollY);
	}
}

// Description:
//   Handles the trigger-to-key mapping. The triggers are handled separately since
//     they are analog instead of a simple button press.
//
// Params:
//   lKey   The mapped key for the left trigger
//   rKey   The mapped key for the right trigger
void Gopher::handleTriggers(WORD lKey, WORD rKey)
{
	bool lTriggerIsDown = _currentState.Gamepad.bLeftTrigger > TRIGGER_DEAD_ZONE;
	bool rTriggerIsDown = _currentState.Gamepad.bRightTrigger > TRIGGER_DEAD_ZONE;

	// Handle left trigger
	if (lTriggerIsDown != _lTriggerPrevious)
	{
		_lTriggerPrevious = lTriggerIsDown;
		if (lTriggerIsDown)
		{
			inputKeyboardDown(lKey);
		}
		else
		{
			inputKeyboardUp(lKey);
		}
	}

	// Handle right trigger
	if (rTriggerIsDown != _rTriggerPrevious)
	{
		_rTriggerPrevious = rTriggerIsDown;
		if (rTriggerIsDown)
		{
			inputKeyboardDown(rKey);
		}
		else
		{
			inputKeyboardUp(rKey);
		}
	}
}

// Description:
//   Handles the state of a controller button press.
//
// Params:
//   STATE  The Gopher state, or command, to update
void Gopher::setXboxClickState(DWORD STATE)
{
	_xboxClickIsDown[STATE] = false;
	_xboxClickIsUp[STATE] = false;

	if (!this->xboxClickStateExists(STATE))
	{
		_xboxClickStateLastIteration[STATE] = false;
	}

	bool isDown = (_currentState.Gamepad.wButtons & STATE) == STATE;

	// Detect if the button has been pressed.
	if (isDown && !_xboxClickStateLastIteration[STATE])
	{
		_xboxClickStateLastIteration[STATE] = true;
		_xboxClickIsDown[STATE] = true;
		_xboxClickDownLength[STATE] = 0;
		_xboxClickIsDownLong[STATE] = false;
	}

	// Detect if the button has been held as a long press.
	if (isDown && _xboxClickStateLastIteration[STATE])
	{
		const int LONG_PRESS_TIME = 200;  // milliseconds

		++_xboxClickDownLength[STATE];
		if (_xboxClickDownLength[STATE] * SLEEP_AMOUNT > LONG_PRESS_TIME)
		{
			_xboxClickIsDownLong[STATE] = true;
		}
	}

	// Detect if the button has been released.
	if (!isDown && _xboxClickStateLastIteration[STATE])
	{
		_xboxClickStateLastIteration[STATE] = false;
		_xboxClickIsUp[STATE] = true;
		_xboxClickIsDownLong[STATE] = false;
	}

	_xboxClickStateLastIteration[STATE] = isDown;
}

// Description:
//   Check to see if a controller state exists in Gopher's button map.
//
// Params:
//   xinput   The Gopher state, or command, to search for
//
// Returns:
//   true if the state is present in the map.
bool Gopher::xboxClickStateExists(DWORD STATE)
{
	auto it = _xboxClickStateLastIteration.find(STATE);
	if (it == _xboxClickStateLastIteration.end())
	{
		return false;
	}

	return true;
}

// Description:
//   Presses or releases a key based on a mapped Gopher state.
//
// Params:
//   STATE  The Gopher state, or command, to trigger a key event
//   key    The key value to input to the system
void Gopher::mapKeyboard(DWORD STATE, WORD key)
{
	setXboxClickState(STATE);
	if (_xboxClickIsDown[STATE])
	{
		inputKeyboardDown(key);

		// Add key to the list of pressed keys.
		_pressedKeys.push_back(key);
	}

	if (_xboxClickIsUp[STATE])
	{
		inputKeyboardUp(key);

		// Remove key from the list of pressed keys.
		erasePressedKey(key);
	}
}

// Description:
//   Presses or releases a mouse button based on a mapped Gopher state
//
// Params:
//   STATE    The Gopher state, or command, to trigger a mouse event
//   keyDown  The button down event for a mouse event
//   keyUp    The button up event for a mouse event
void Gopher::mapMouseClick(DWORD STATE, DWORD keyDown, DWORD keyUp)
{
	setXboxClickState(STATE);
	if (_xboxClickIsDown[STATE])
	{
		mouseEvent(keyDown);

		// Add key to the list of pressed keys.
		if (keyDown == MOUSEEVENTF_LEFTDOWN)
		{
			_pressedKeys.push_back(VK_LBUTTON);
		}
		else if (keyDown == MOUSEEVENTF_RIGHTDOWN)
		{
			_pressedKeys.push_back(VK_RBUTTON);
		}
		else if (keyDown == MOUSEEVENTF_MIDDLEDOWN)
		{
			_pressedKeys.push_back(VK_MBUTTON);
		}
	}

	if (_xboxClickIsUp[STATE])
	{
		mouseEvent(keyUp);

		// Remove key from the list of pressed keys.
		if (keyUp == MOUSEEVENTF_LEFTUP)
		{
			erasePressedKey(VK_LBUTTON);
		}
		else if (keyUp == MOUSEEVENTF_RIGHTUP)
		{
			erasePressedKey(VK_RBUTTON);
		}
		else if (keyUp == MOUSEEVENTF_MIDDLEUP)
		{
			erasePressedKey(VK_MBUTTON);
		}
	}

	/*if (_xboxClickIsDownLong[STATE])
	{
	  mouseEvent(keyDown | keyUp);
	  mouseEvent(keyDown | keyUp);
	}*/
}

// Description:
//   Callback function used for the EnumWindows call to determine if we
//     have found the On-Screen Keyboard window.
//
// Params:
//   curWnd   The current window to check
//   lParam   A callback parameter used to store the window if it is found
//
// Returns:
//   FALSE when the the desired window is found.
static BOOL CALLBACK EnumWindowsProc(HWND curWnd, LPARAM lParam)
{
	TCHAR title[256];
	// Check to see if the window title matches what we are looking for.
	if (GetWindowText(curWnd, title, 256) && !_tcscmp(title, _T("On-Screen Keyboard")))
	{
		*(HWND*)lParam = curWnd;
		return FALSE;  // Correct window found, stop enumerating through windows.
	}

	return TRUE;
}

// Description:
//   Finds the On-Screen Keyboard if it is open.
//
// Returns:
//   If found, the handle to the On-Screen Keyboard handle. Otherwise, returns NULL.
HWND Gopher::getOskWindow()
{
	HWND ret = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)&ret);
	return ret;
}

bool Gopher::openOnScreenKeyboard()
{
	typedef BOOL(WINAPI* Wow64DisableWow64FsRedirectionFunc)(PVOID*);
	typedef BOOL(WINAPI* Wow64RevertWow64FsRedirectionFunc)(PVOID);
	// Disable file system redirection
	PVOID oldValue;
	Wow64DisableWow64FsRedirectionFunc disableWow64FsRedirection =
		(Wow64DisableWow64FsRedirectionFunc)GetProcAddress(
			GetModuleHandle(TEXT("kernel32")), "Wow64DisableWow64FsRedirection");
	Wow64RevertWow64FsRedirectionFunc revertWow64FsRedirection =
		(Wow64RevertWow64FsRedirectionFunc)GetProcAddress(
			GetModuleHandle(TEXT("kernel32")), "Wow64RevertWow64FsRedirection");

	if (disableWow64FsRedirection && revertWow64FsRedirection) {
		disableWow64FsRedirection(&oldValue);
	}

	// Now try launching
	HINSTANCE result = ShellExecuteW(
		NULL,
		L"runas", // "runas" triggers UAC prompt
		L"C:\\Windows\\System32\\osk.exe",
		NULL,
		NULL,
		SW_SHOWNORMAL
	);

	if (revertWow64FsRedirection) {
		revertWow64FsRedirection(oldValue);
	}

	if ((int)result <= 32) {
		std::cerr << "Failed to start OSK. Error code: " << (int)result << std::endl;
		return false;
	}

	return true;
}

// Description:
//   Removes an entry for a pressed key from the list.
//
// Params:
//   key  The key value to remove from the pressed key list. 
//
// Returns:
//   True if the given key was found and removed from the list.
bool Gopher::erasePressedKey(WORD key)
{
	for (std::list<WORD>::iterator it = _pressedKeys.begin();
		it != _pressedKeys.end();
		++it)
	{
		if (*it == key)
		{
			_pressedKeys.erase(it);
			return true;
		}
	}

	return false;
}

