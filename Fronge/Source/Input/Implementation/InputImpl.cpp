#include "froch.hpp"

#include "InputImpl.hpp"

namespace fro
{
	constexpr Key convertSDLKeyCode(SDL_Keycode const keyCode)
	{
		switch (keyCode)
		{
		case SDLK_RETURN:
		case SDLK_ESCAPE:
		case SDLK_BACKSPACE:
		case SDLK_TAB:
		case SDLK_SPACE:
		case SDLK_EXCLAIM:
		case SDLK_QUOTEDBL:
		case SDLK_HASH:
		case SDLK_PERCENT:
		case SDLK_DOLLAR:
		case SDLK_AMPERSAND:
		case SDLK_QUOTE:
		case SDLK_LEFTPAREN:
		case SDLK_RIGHTPAREN:
		case SDLK_ASTERISK:
		case SDLK_PLUS:
		case SDLK_COMMA:
		case SDLK_MINUS:
		case SDLK_PERIOD:
		case SDLK_SLASH:
		case SDLK_0:
		case SDLK_1:
		case SDLK_2:
		case SDLK_3:
		case SDLK_4:
		case SDLK_5:
		case SDLK_6:
		case SDLK_7:
		case SDLK_8:
		case SDLK_9:
		case SDLK_COLON:
		case SDLK_SEMICOLON:
		case SDLK_LESS:
		case SDLK_EQUALS:
		case SDLK_GREATER:
		case SDLK_QUESTION:
		case SDLK_AT:
		case SDLK_LEFTBRACKET:
		case SDLK_BACKSLASH:
		case SDLK_RIGHTBRACKET:
		case SDLK_CARET:
		case SDLK_UNDERSCORE:
		case SDLK_BACKQUOTE:
		case SDLK_a:
		case SDLK_b:
		case SDLK_c:
		case SDLK_d:
		case SDLK_e:
		case SDLK_f:
		case SDLK_g:
		case SDLK_h:
		case SDLK_i:
		case SDLK_j:
		case SDLK_k:
		case SDLK_l:
		case SDLK_m:
		case SDLK_n:
		case SDLK_o:
		case SDLK_p:
		case SDLK_q:
		case SDLK_r:
		case SDLK_s:
		case SDLK_t:
		case SDLK_u:
		case SDLK_v:
		case SDLK_w:
		case SDLK_x:
		case SDLK_y:
		case SDLK_z:
		case SDLK_DELETE:
			return static_cast<Key>(keyCode);
			break;

		case SDLK_CAPSLOCK:
			return Key::CAPS_LOCK;
			break;

		case SDLK_F1:
			return Key::F1;
			break;

		case SDLK_F2:
			return Key::F2;
			break;

		case SDLK_F3:
			return Key::F3;
			break;

		case SDLK_F4:
			return Key::F4;
			break;

		case SDLK_F5:
			return Key::F5;
			break;

		case SDLK_F6:
			return Key::F6;
			break;

		case SDLK_F7:
			return Key::F7;
			break;

		case SDLK_F8:
			return Key::F8;
			break;

		case SDLK_F9:
			return Key::F9;
			break;

		case SDLK_F10:
			return Key::F10;
			break;

		case SDLK_F11:
			return Key::F11;
			break;

		case SDLK_F12:
			return Key::F12;
			break;

		case SDLK_PRINTSCREEN:
			return Key::PRINT_SCREEN;
			break;

		case SDLK_SCROLLLOCK:
			return Key::SCROLL_LOCK;
			break;

		case SDLK_PAUSE:
			return Key::PAUSE;
			break;

		case SDLK_INSERT:
			return Key::INSERT;
			break;

		case SDLK_HOME:
			return Key::HOME;
			break;

		case SDLK_PAGEUP:
			return Key::PAGE_UP;
			break;

		case SDLK_END:
			return Key::END;
			break;

		case SDLK_PAGEDOWN:
			return Key::PAGE_DOWN;
			break;

		case SDLK_RIGHT:
			return Key::RIGHT;
			break;

		case SDLK_LEFT:
			return Key::LEFT;
			break;

		case SDLK_DOWN:
			return Key::DOWN;
			break;

		case SDLK_UP:
			return Key::UP;
			break;

		case SDLK_NUMLOCKCLEAR:
			return Key::NUM_LOCK_CLEAR;
			break;

		case SDLK_KP_DIVIDE:
			return Key::KEY_PAD_DIVIDE;
			break;

		case SDLK_KP_MULTIPLY:
			return Key::KEY_PAD_MULTIPLY;
			break;

		case SDLK_KP_MINUS:
			return Key::KEY_PAD_MINUS;
			break;

		case SDLK_KP_PLUS:
			return Key::KEY_PAD_PLUS;
			break;

		case SDLK_KP_ENTER:
			return Key::KEY_PAD_ENTER;
			break;

		case SDLK_KP_1:
			return Key::KEY_PAD_NUMBER_1;
			break;

		case SDLK_KP_2:
			return Key::KEY_PAD_NUMBER_2;
			break;

		case SDLK_KP_3:
			return Key::KEY_PAD_NUMBER_3;
			break;

		case SDLK_KP_4:
			return Key::KEY_PAD_NUMBER_4;
			break;

		case SDLK_KP_5:
			return Key::KEY_PAD_NUMBER_5;
			break;

		case SDLK_KP_6:
			return Key::KEY_PAD_NUMBER_6;
			break;

		case SDLK_KP_7:
			return Key::KEY_PAD_NUMBER_7;
			break;

		case SDLK_KP_8:
			return Key::KEY_PAD_NUMBER_8;
			break;

		case SDLK_KP_9:
			return Key::KEY_PAD_NUMBER_9;
			break;

		case SDLK_KP_0:
			return Key::KEY_PAD_NUMBER_0;
			break;

		case SDLK_KP_PERIOD:
			return Key::KEY_PAD_PERIOD;
			break;

		case SDLK_APPLICATION:
			return Key::APPLICATION;
			break;

		case SDLK_POWER:
			return Key::POWER;
			break;

		case SDLK_KP_EQUALS:
			return Key::KEY_PAD_EQUALS;
			break;

		case SDLK_F13:
			return Key::F13;
			break;

		case SDLK_F14:
			return Key::F14;
			break;

		case SDLK_F15:
			return Key::F15;
			break;

		case SDLK_F16:
			return Key::F16;
			break;

		case SDLK_F17:
			return Key::F17;
			break;

		case SDLK_F18:
			return Key::F18;
			break;

		case SDLK_F19:
			return Key::F19;
			break;

		case SDLK_F20:
			return Key::F20;
			break;

		case SDLK_F21:
			return Key::F21;
			break;

		case SDLK_F22:
			return Key::F22;
			break;

		case SDLK_F23:
			return Key::F23;
			break;

		case SDLK_F24:
			return Key::F24;
			break;

		case SDLK_EXECUTE:
			return Key::EXECUTE;
			break;

		case SDLK_HELP:
			return Key::HELP;
			break;

		case SDLK_MENU:
			return Key::MENU;
			break;

		case SDLK_SELECT:
			return Key::SELECT;
			break;

		case SDLK_STOP:
			return Key::STOP;
			break;

		case SDLK_AGAIN:
			return Key::AGAIN;
			break;

		case SDLK_UNDO:
			return Key::UNDO;
			break;

		case SDLK_CUT:
			return Key::CUT;
			break;

		case SDLK_COPY:
			return Key::COPY;
			break;

		case SDLK_PASTE:
			return Key::PASTE;
			break;

		case SDLK_FIND:
			return Key::FIND;
			break;

		case SDLK_MUTE:
			return Key::MUTE;
			break;

		case SDLK_VOLUMEUP:
			return Key::VOLUME_UP;
			break;

		case SDLK_VOLUMEDOWN:
			return Key::VOLUME_DOWN;
			break;

		case SDLK_KP_COMMA:
			return Key::KEY_PAD_COMMA;
			break;

		case SDLK_KP_EQUALSAS400:
			return Key::KEY_PAD_EQUALS_AS_400;
			break;

		case SDLK_ALTERASE:
			return Key::ALTER_ASE;
			break;

		case SDLK_SYSREQ:
			return Key::SYSTEM_REQUEST;
			break;

		case SDLK_CANCEL:
			return Key::CANCEL;
			break;

		case SDLK_CLEAR:
			return Key::CLEAR;
			break;

		case SDLK_PRIOR:
			return Key::PRIOR;
			break;

		case SDLK_RETURN2:
			return Key::RETURN_2;
			break;

		case SDLK_SEPARATOR:
			return Key::SEPARATOR;
			break;

		case SDLK_OUT:
			return Key::OUT;
			break;

		case SDLK_OPER:
			return Key::OPER;
			break;

		case SDLK_CLEARAGAIN:
			return Key::CLEAR_AGAIN;
			break;

		case SDLK_CRSEL:
			return Key::CR_SEL;
			break;

		case SDLK_EXSEL:
			return Key::EX_SEL;
			break;

		case SDLK_KP_00:
			return Key::KEY_PAD_NUMBER_00;
			break;

		case SDLK_KP_000:
			return Key::KEY_PAD_NUMBER_000;
			break;

		case SDLK_THOUSANDSSEPARATOR:
			return Key::THOUSANDS_SEPARATOR;
			break;

		case SDLK_DECIMALSEPARATOR:
			return Key::DECIMAL_SEPARATOR;
			break;

		case SDLK_CURRENCYUNIT:
			return Key::CURRENCY_UNIT;
			break;

		case SDLK_CURRENCYSUBUNIT:
			return Key::CURRENCY_SUB_UNIT;
			break;

		case SDLK_KP_LEFTPAREN:
			return Key::KEY_PAD_LEFT_PARENTHESIS;
			break;

		case SDLK_KP_RIGHTPAREN:
			return Key::KEY_PAD_RIGHT_PARENTHESIS;
			break;

		case SDLK_KP_LEFTBRACE:
			return Key::KEY_PAD_LEFT_BRACE;
			break;

		case SDLK_KP_RIGHTBRACE:
			return Key::KEY_PAD_RIGHT_BRACE;
			break;

		case SDLK_KP_TAB:
			return Key::KEY_PAD_TAB;
			break;

		case SDLK_KP_BACKSPACE:
			return Key::KEY_PAD_BACKSPACE;
			break;

		case SDLK_KP_A:
			return Key::KEY_PAD_A;
			break;

		case SDLK_KP_B:
			return Key::KEY_PAD_B;
			break;

		case SDLK_KP_C:
			return Key::KEY_PAD_C;
			break;

		case SDLK_KP_D:
			return Key::KEY_PAD_D;
			break;

		case SDLK_KP_E:
			return Key::KEY_PAD_E;
			break;

		case SDLK_KP_F:
			return Key::KEY_PAD_F;
			break;

		case SDLK_KP_XOR:
			return Key::KEY_PAD_XOR;
			break;

		case SDLK_KP_POWER:
			return Key::KEY_PAD_POWER;
			break;

		case SDLK_KP_PERCENT:
			return Key::KEY_PAD_PERCENT;
			break;

		case SDLK_KP_LESS:
			return Key::KEY_PAD_LESS;
			break;

		case SDLK_KP_GREATER:
			return Key::KEY_PAD_GREATER;
			break;

		case SDLK_KP_AMPERSAND:
			return Key::KEY_PAD_AMPERSAND;
			break;

		case SDLK_KP_DBLAMPERSAND:
			return Key::KEY_PAD_DOUBLE_AMPERSAND;
			break;

		case SDLK_KP_VERTICALBAR:
			return Key::KEY_PAD_VERTICALBAR;
			break;

		case SDLK_KP_DBLVERTICALBAR:
			return Key::KEY_PAD_DOUBLEB_VERTICAL_BAR;
			break;

		case SDLK_KP_COLON:
			return Key::KEY_PAD_COLON;
			break;

		case SDLK_KP_HASH:
			return Key::KEY_PAD_HASH;
			break;

		case SDLK_KP_SPACE:
			return Key::KEY_PAD_SPACE;
			break;

		case SDLK_KP_AT:
			return Key::KEY_PAD_AT;
			break;

		case SDLK_KP_EXCLAM:
			return Key::KEY_PAD_EXCLAMATION_MARK;
			break;

		case SDLK_KP_MEMSTORE:
			return Key::KEY_PAD_MEMORY_STORE;
			break;

		case SDLK_KP_MEMRECALL:
			return Key::KEY_PAD_MEMORY_RECALL;
			break;

		case SDLK_KP_MEMCLEAR:
			return Key::KEY_PAD_MEMORY_CLEAR;
			break;

		case SDLK_KP_MEMADD:
			return Key::KEY_PAD_MEMORY_ADD;
			break;

		case SDLK_KP_MEMSUBTRACT:
			return Key::KEY_PAD_MEMORY_SUBTRACT;
			break;

		case SDLK_KP_MEMMULTIPLY:
			return Key::KEY_PAD_MEMORY_MULTIPLY;
			break;

		case SDLK_KP_MEMDIVIDE:
			return Key::KEY_PAD_MEMORY_DIVIDE;
			break;

		case SDLK_KP_PLUSMINUS:
			return Key::KEY_PAD_PLUS_MINUS;
			break;

		case SDLK_KP_CLEAR:
			return Key::KEY_PAD_CLEAR;
			break;

		case SDLK_KP_CLEARENTRY:
			return Key::KEY_PAD_CLEAR_ENTRY;
			break;

		case SDLK_KP_BINARY:
			return Key::KEY_PAD_BINARY;
			break;

		case SDLK_KP_OCTAL:
			return Key::KEY_PAD_OCTAL;
			break;

		case SDLK_KP_DECIMAL:
			return Key::KEY_PAD_DECIMAL;
			break;

		case SDLK_KP_HEXADECIMAL:
			return Key::KEY_PAD_HEXADECIMAL;
			break;

		case SDLK_LCTRL:
			return Key::LEFT_CTRL;
			break;

		case SDLK_LSHIFT:
			return Key::LEFT_SHIFT;
			break;

		case SDLK_LALT:
			return Key::LEFT_ALT;
			break;

		case SDLK_LGUI:
			return Key::LEFT_GUI;
			break;

		case SDLK_RCTRL:
			return Key::RIGHT_CTRL;
			break;

		case SDLK_RSHIFT:
			return Key::RIGHT_SHIFT;
			break;

		case SDLK_RALT:
			return Key::RIGHT_ALT;
			break;

		case SDLK_RGUI:
			return Key::RIGHT_GUI;
			break;

		case SDLK_MODE:
			return Key::MODE;
			break;

		case SDLK_AUDIONEXT:
			return Key::AUDIO_NEXT;
			break;

		case SDLK_AUDIOPREV:
			return Key::AUDIO_PREV;
			break;

		case SDLK_AUDIOSTOP:
			return Key::AUDIO_STOP;
			break;

		case SDLK_AUDIOPLAY:
			return Key::AUDIO_PLAY;
			break;

		case SDLK_AUDIOMUTE:
			return Key::AUDIO_MUTE;
			break;

		case SDLK_MEDIASELECT:
			return Key::MEDIA_SELECT;
			break;

		case SDLK_WWW:
			return Key::WEB;
			break;

		case SDLK_MAIL:
			return Key::MAIL;
			break;

		case SDLK_CALCULATOR:
			return Key::CALCULATOR;
			break;

		case SDLK_COMPUTER:
			return Key::COMPUTER;
			break;

		case SDLK_AC_SEARCH:
			return Key::AC_SEARCH;
			break;

		case SDLK_AC_HOME:
			return Key::AC_HOME;
			break;

		case SDLK_AC_BACK:
			return Key::AC_BACK;
			break;

		case SDLK_AC_FORWARD:
			return Key::AC_FORWARD;
			break;

		case SDLK_AC_STOP:
			return Key::AC_STOP;
			break;

		case SDLK_AC_REFRESH:
			return Key::AC_REFRESH;
			break;

		case SDLK_AC_BOOKMARKS:
			return Key::AC_BOOKMARKS;
			break;

		case SDLK_BRIGHTNESSDOWN:
			return Key::BRIGHTNESS_DOWN;
			break;

		case SDLK_BRIGHTNESSUP:
			return Key::BRIGHTNESS_UP;
			break;

		case SDLK_DISPLAYSWITCH:
			return Key::DISPLAY_SWITCH;
			break;

		case SDLK_KBDILLUMTOGGLE:
			return Key::KEYBOARD_ILLUMINATION_TOGGLE;
			break;

		case SDLK_KBDILLUMDOWN:
			return Key::KEYBOARD_ILLUMINATION_DOWN;
			break;

		case SDLK_KBDILLUMUP:
			return Key::KEYBOARD_ILLUMINATION_UP;
			break;

		case SDLK_EJECT:
			return Key::EJECT;
			break;

		case SDLK_SLEEP:
			return Key::SLEEP;
			break;

		case SDLK_APP1:
			return Key::APP_1;
			break;

		case SDLK_APP2:
			return Key::APP_2;
			break;

		case SDLK_AUDIOREWIND:
			return Key::AUDIO_REWIND;
			break;

		case SDLK_AUDIOFASTFORWARD:
			return Key::AUDIO_FAST_FORWARD;
			break;

		case SDLK_SOFTLEFT:
			return Key::SOFT_LEFT;
			break;

		case SDLK_SOFTRIGHT:
			return Key::SOFT_RIGHT;
			break;

		case SDLK_CALL:
			return Key::CALL;
			break;

		case SDLK_ENDCALL:
			return Key::END_CALL;
			break;

		default:
			return Key::UNKNOWN;
		}
	}

	constexpr GamepadButton convertSDLControllerButton(Uint8 const button)
	{
		switch (button)
		{
		case SDL_CONTROLLER_BUTTON_A:
			return GamepadButton::A;

		case SDL_CONTROLLER_BUTTON_B:
			return GamepadButton::B;

		case SDL_CONTROLLER_BUTTON_X:
			return GamepadButton::X;

		case SDL_CONTROLLER_BUTTON_Y:
			return GamepadButton::Y;

		case SDL_CONTROLLER_BUTTON_BACK:
			return GamepadButton::BACK;

		case SDL_CONTROLLER_BUTTON_GUIDE:
			return GamepadButton::GUIDE;

		case SDL_CONTROLLER_BUTTON_START:
			return GamepadButton::START;

		case SDL_CONTROLLER_BUTTON_LEFTSTICK:
			return GamepadButton::LEFT_STICK;

		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
			return GamepadButton::RIGHT_STICK;

		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			return GamepadButton::LEFT_SHOULDER;

		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			return GamepadButton::RIGHT_SHOULDER;

		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			return GamepadButton::UP;

		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			return GamepadButton::DOWN;

		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			return GamepadButton::LEFT;

		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			return GamepadButton::RIGHT;

		case SDL_CONTROLLER_BUTTON_MISC1:
			return GamepadButton::MISCELLANEOUS;

		case SDL_CONTROLLER_BUTTON_PADDLE1:
			return GamepadButton::PADDLE_1;

		case SDL_CONTROLLER_BUTTON_PADDLE2:
			return GamepadButton::PADDLE_2;

		case SDL_CONTROLLER_BUTTON_PADDLE3:
			return GamepadButton::PADDLE_3;

		case SDL_CONTROLLER_BUTTON_PADDLE4:
			return GamepadButton::PADDLE_4;

		case SDL_CONTROLLER_BUTTON_TOUCHPAD:
			return GamepadButton::TOUCHPAD;

		default:
			return GamepadButton::UNKNOWN;
		}
	}

	constexpr std::pair<GamepadAxis, double> convertSDLControllerAxis(Uint8 const axis, Sint16 const value)
	{
		GamepadAxis convertedAxis;

		switch (axis)
		{
		case SDL_CONTROLLER_AXIS_LEFTX:
			value >= 0 ?
				convertedAxis = GamepadAxis::LEFT_STICK_RIGHT :
				convertedAxis = GamepadAxis::LEFT_STICK_LEFT;
			break;

		case SDL_CONTROLLER_AXIS_LEFTY:
			value >= 0 ?
				convertedAxis = GamepadAxis::LEFT_STICK_DOWN :
				convertedAxis = GamepadAxis::LEFT_STICK_UP;
			break;

		case SDL_CONTROLLER_AXIS_RIGHTX:
			value >= 0 ?
				convertedAxis = GamepadAxis::RIGHT_STICK_RIGHT :
				convertedAxis = GamepadAxis::RIGHT_STICK_LEFT;
			break;

		case SDL_CONTROLLER_AXIS_RIGHTY:
			value >= 0 ?
				convertedAxis = GamepadAxis::RIGHT_STICK_DOWN :
				convertedAxis = GamepadAxis::RIGHT_STICK_UP;
			break;

		case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
			convertedAxis = GamepadAxis::LEFT_TRIGGER;
			break;

		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
			convertedAxis = GamepadAxis::RIGHT_TRIGGER;
			break;

		default:
			return { GamepadAxis::UNKNOWN, 0.0 };
		}

		auto constexpr highestAxisValue{ std::numeric_limits<decltype(value)>::max() };
		auto constexpr lowestAxisValue{ std::numeric_limits<decltype(value)>::lowest() };
		return
		{
			convertedAxis,
			static_cast<double>(value) / (value >= 0 ? highestAxisValue : lowestAxisValue)
		};
	}
}