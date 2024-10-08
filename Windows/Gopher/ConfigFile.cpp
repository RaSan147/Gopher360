#include "ConfigFile.h"
#include <iostream>
#include <fstream> 
#include <windows.h>

void ConfigFile::removeComment(std::string& line) const
{
	if (line.find('#') != line.npos)
	{
		line.erase(line.find('#'));
	}
}

bool ConfigFile::onlyWhitespace(const std::string& line) const
{
	return (line.find_first_not_of(' ') == line.npos);
}

bool ConfigFile::validLine(const std::string& line) const
{
	std::string temp = line;
	temp.erase(0, temp.find_first_not_of("\t "));
	if (temp[0] == '=')
	{
		return false;
	}

	for (size_t i = temp.find('=') + 1; i < temp.length(); i++)
	{
		if (temp[i] != ' ')
		{
			return true;
		}
	}

	return false;
}

void ConfigFile::extractKey(std::string& key, size_t const& sepPos, const std::string& line) const
{
	key = line.substr(0, sepPos);
	if (key.find('\t') != line.npos || key.find(' ') != line.npos)
	{
		key.erase(key.find_first_of("\t "));
	}
}
void ConfigFile::extractValue(std::string& value, size_t const& sepPos, const std::string& line) const
{
	value = line.substr(sepPos + 1);
	value.erase(0, value.find_first_not_of("\t "));
	value.erase(value.find_last_not_of("\t ") + 1);
}

void ConfigFile::extractContents(const std::string& line)
{
	std::string temp = line;
	temp.erase(0, temp.find_first_not_of("\t "));
	size_t sepPos = temp.find('=');

	std::string key, value;
	extractKey(key, sepPos, temp);
	extractValue(value, sepPos, temp);

	if (!keyExists(key))
	{
		contents.insert(std::pair<std::string, std::string>(key, value));
	}
	else
	{
		exitWithError("CFG: Can only have unique key names!\n");
	}
}

void ConfigFile::parseLine(const std::string& line, size_t const lineNo)
{
	if (line.find('=') == line.npos)
	{
		exitWithError("CFG: Couldn't find separator on line: " + Convert::T_to_string(lineNo) + "\n");
	}

	if (!validLine(line))
	{
		exitWithError("CFG: Bad format for line: " + Convert::T_to_string(lineNo) + "\n");
	}

	extractContents(line);
}

void ConfigFile::ExtractKeys()
{
	std::ifstream file;
	file.open(fName.c_str());

	if (!file)
	{
		printf("%s not found! Building a fresh one... ", fName.c_str());

		std::ofstream outfile("config.ini");

		// Begin config dump to file
		outfile << "#	GOPHER DEFAULT CONFIGURATION rev1.0 - Auto generated by Gopher360." << std::endl;
		outfile << "#	If you want a fresh one, just DELETE THIS FILE and re-run Gopher360." << std::endl;
		outfile << "#	Set which controller buttons will activate the configuration events." << std::endl;
		outfile << "#	SET 0 FOR NO FUNCTION." << std::endl;
		outfile << "" << std::endl;

		outfile << "# ADVANCED CONFIGURATION SETTINGS" << std::endl;
		outfile << "#  ALLOWED CURSOR SPEEDS, FIRST WILL BE CHOSEN BY DEFAULT.  VALUES > 1.0 WILL BE IGNORED.  NO SPACES." << std::endl;
		outfile << "SCROLL_SPEEDS = ULTRASLOW=0.03,LOW=0.07,MED=0.1,HIGH=0.2" << std::endl;
		outfile << "CURSOR_SPEEDS = ULTRALOW=0.005,LOW=0.015,MED=0.025,HIGH=0.04" << std::endl;

		outfile << "#  SET ACCELERATION FACTOR FOR NON-LINEAR CURSOR SPEED" << std::endl;
		outfile << "ACCELERATION_FACTOR = 3" << std::endl;
		outfile << "#  Swaps the function of the thumbsticks. Set to 0 for default behavior or set to 1 to have the mouse movement on the right stick and scrolling on the left stick." << std::endl;
		outfile << "SWAP_THUMBSTICKS = 0" << std::endl;
		outfile << "DEAD_ZONE = 6000 # Thumbstick dead zone to use for mouse movement. Absolute maximum thumbstick value is 32768." << std::endl;
		outfile << "SCROLL_DEAD_ZONE = 6000 # Thumbstick dead zone to use for scroll wheel movement. Absolute maximum thumbstick value is 32768." << std::endl;
		outfile << "TRIGGER_DEAD_ZONE = 0 # Dead zone for the left and right triggers to detect a trigger press. Range from 0 (accept all input) to 255 (ignore all input)." << std::endl << std::endl;

		outfile << "SCROLL_SPEED_ID = 2" << std::endl;
		outfile << "# Speed preset index at which you scroll" << std::endl;
		outfile << "CURSOR_SPEED_ID = 2" << std::endl;
		outfile << "# Speed preset index at which you move the cursor" << std::endl << std::endl;


		outfile << "#	AVAILABLE VALUES AT https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad(v=vs.85).aspx" << std::endl;
		outfile << "#	TIP: Sum the hex value for double button shortcuts eg. 0x0010(START) 0x0020(BACK) so 0x0030(START+BACK) will trigger the event only when both are pressed." << std::endl;
		outfile << "" << std::endl;
		outfile << "" << std::endl;
		outfile << "#CONFIG_OSK = 0x0020                # Toggle on-screen keyboard" << std::endl;
		outfile << "" << std::endl;
		outfile << "" << std::endl;
		outfile << "#	KEYBOARD SHORTCUTS ON CONTROLLER BUTTONS" << std::endl;
		outfile << "#	SET 0 FOR NO FUNCTION" << std::endl;
		outfile << "#	AVAILABLE VALUES AT> https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731" << std::endl;
		outfile << "" << std::endl;
		outfile << "" << std::endl;
		outfile << "CONFIG_MOUSE_LEFT = 0x2000          # Left mouse button          #B" << std::endl;
		outfile << "CONFIG_MOUSE_RIGHT = 0x4000         # Right mouse button         #X" << std::endl;
		outfile << "CONFIG_MOUSE_MIDDLE = 0             # Middle mouse button" << std::endl;
		outfile << "CONFIG_HIDE = 0                     # Hides the terminal" << std::endl;
		outfile << "CONFIG_DISABLE = 0x0030             # Disables the Gopher        #Select+Start              " << std::endl;
		outfile << "CONFIG_DISABLE_VIBRATION = 0x0021   # Disables Gopher Vibrations #Select+Up" << std::endl;
		outfile << "CONFIG_SCROLL_SPEED_CHANGE = 0x0028 # Change scroll speed        #Select+Right" << std::endl;
		outfile << "CONFIG_SPEED_CHANGE =  0x0024       # Change speed               #Select+Left" << std::endl;
		outfile << "GAMEPAD_DPAD_UP = 0x26              # UP" << std::endl;
		outfile << "GAMEPAD_DPAD_DOWN = 0x28            # DOWN" << std::endl;
		outfile << "GAMEPAD_DPAD_LEFT = 0x25            # LEFT" << std::endl;
		outfile << "GAMEPAD_DPAD_RIGHT = 0x27           # RIGHT" << std::endl;
		outfile << "GAMEPAD_START = 0x5B                # WINDOW" << std::endl;
		outfile << "GAMEPAD_BACK = 0x09                 # TAB" << std::endl;
		outfile << "GAMEPAD_LEFT_THUMB = 0" << std::endl;
		outfile << "GAMEPAD_RIGHT_THUMB = 0" << std::endl;
		outfile << "GAMEPAD_LEFT_SHOULDER = 0xA6        # BACK" << std::endl;
		outfile << "GAMEPAD_RIGHT_SHOULDER = 0xA7       # FORWARD" << std::endl;
		outfile << "GAMEPAD_A = 0x0D                    # ENTER" << std::endl;
		outfile << "GAMEPAD_B = 0                       # Default left click" << std::endl;
		outfile << "GAMEPAD_X = 0                       # Default right click" << std::endl;
		outfile << "GAMEPAD_Y = 0x08                    # BACKSPACE" << std::endl;
		outfile << "" << std::endl;
		outfile << "GAMEPAD_TRIGGER_LEFT = 0x10         # shift key" << std::endl;
		outfile << "GAMEPAD_TRIGGER_RIGHT = 0x11        # control key" << std::endl;
		outfile << "CTRL_GAMEPAD_LEFT_SHOULDER = 0xBD   # ctrl + -" << std::endl;
		outfile << "CTRL_GAMEPAD_RIGHT_SHOULDER = 0xBB  # ctrl + +" << std::endl;
		outfile << "" << std::endl;
		// End config dump

		outfile.close();

		file.open(fName.c_str());

		if (!file)
		{

			exitWithError("\nERROR! Configuration file " + fName + " still couldn't be found!\n");
		}
		else
		{

			printf("Success!\nNow using %s.\n", fName.c_str());
		}
	}

	 // print the text file

	std::cout << "CONFIG FILE: " << fName << std::endl;
	std::cout << "--------------------------------" << std::endl;


	//exitWithError("\nSafety exit!\n");

	std::string line;
	size_t lineNo = 0;
	while (std::getline(file, line))
	{
		lineNo++;
		std::string temp = line;

		if (temp.empty())
		{
			continue;
		}

		removeComment(temp);
		if (onlyWhitespace(temp))
		{
			continue;
		}

		// print the line
		std::cout << line << std::endl;

		parseLine(temp, lineNo);
	}

	file.close();
}

ConfigFile::ConfigFile(const std::string& fName)
{
	this->fName = fName;
	ExtractKeys();
}

bool ConfigFile::keyExists(const std::string& key) const
{
	return contents.find(key) != contents.end();
}

void ConfigFile::exitWithError(const std::string& error)
{
	std::cout << error;
	std::cin.ignore();
	std::cin.get();

	exit(EXIT_FAILURE);
}
