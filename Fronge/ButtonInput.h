#pragma once

#include <SDL2/SDL_Scancode.h>
#include <SDL2/SDL_gamecontroller.h>
#include <bitset>
#include <variant>
#include <optional>

namespace fro
{
	struct ButtonInput final
	{
	public:
        enum class Key
        {
            Unknown,

            Return,
            Escape,
            Backspace,
            Tab,
            Space,
            ExclamationMark,
            DoubleQuote,
            Hash,
            Percent,
            Dollar,
            Ampersand,
            Quote,
            LeftParenthesis,
            RightParenthesis,
            Asterisk,
            Plus,
            Comma,
            Minus,
            Period,
            Slash,
            N0,
            N1,
            N2,
            N3,
            N4,
            N5,
            N6,
            N7,
            N8,
            N9,
            Colon,
            SemiColon,
            Less,
            Equals,
            Greater,
            QuestionMark,
            At,

            LeftBracket,
            BackSlash,
            RighBracket,
            Caret,
            Underscore,
            Backquote,
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,

            CapsLock,

            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,

            PrintScreen,
            ScrollLock,
            Pause,
            Insert,
            Home,
            PageUp,
            Delete,
            End,
            PageDown,
            Right,
            Left,
            Down,
            Up,

            NumLockClear,
            KeyPadDivide,
            KeyPadMultiply,
            KeyPadMinus,
            KeyPadPlus,
            KeyPadEnter,
            KeyPadN1,
            KeyPadN2,
            KeyPadN3,
            KeyPadN4,
            KeyPadN5,
            KeyPadN6,
            KeyPadN7,
            KeyPadN8,
            KeyPadN9,
            KeyPadN0,
            KeyPadPeriod,

            Application,
            Power,
            KeyPadEquals,
            F13,
            F14,
            F15,
            F16,
            F17,
            F18,
            F19,
            F20,
            F21,
            F22,
            F23,
            F24,
            Execute,
            Help,
            Menu,
            Select,
            Stop,
            Again,
            Undo,
            Cut,
            Copy,
            Paste,
            Find,
            Mute,
            VolumeUp,
            VolumeDown,
            KeyPadComma,
            KeyPadEqualsAs400,

            AlterAse,
            SystemRequest,
            Cancel,
            Clear,
            Prior,
            Return2,
            Separator,
            Out,
            Oper,
            ClearAgain,
            CrSel,
            ExSel,

            KeyPadN00,
            KeyPadN000,
            ThousandsSeparator,
            DecimalSeperator,
            CurrencyUnit,
            CurrencySubUnit,
            KeyPadLeftParenthesis,
            KeyPadRightParenthesis,
            KeyPadLeftBrace,
            KeyPadRightBrace,
            KeyPadTab,
            KeyPadBackspace,
            KeyPadA,
            KeyPadB,
            KeyPadC,
            KeyPadD,
            KeyPadE,
            KeyPadF,
            KeyPadXOR,
            KeyPadPower,
            KeyPadPercent,
            KeyPadLess,
            KeyPadGreater,
            KeyPadAmpersand,
            KeyPadDoubleAmpersand,
            KeyPadVerticalBar,
            KeyPadDoubleVerticalBar,
            KeyPadColon,
            KeyPadHash,
            KeyPadSpace,
            KeyPadAt,
            KeyPadExclamationMark,
            KeyPadMemoryStore,
            KeyPadMemoryRecall,
            KeyPadMemoryClear,
            KeyPadMemoryAdd,
            KeyPadMemorySubtract,
            KeyPadMemoryMultiply,
            KeyPadMemoryDivide,
            KeyPadPlusMinus,
            KeyPadClear,
            KeyPadClearEntry,
            KeyPadBinary,
            KeyPadOctal,
            KeyPadDecimal,
            KeyPadHexadecimal,

            LeftControl,
            LeftShift,
            LeftAlt,
            LeftGUI,
            RightControl,
            RightShift,
            RightAlt,
            RightGUI,

            Mode,

            AudioNext,
            AudioPrevious,
            AudioStop,
            AudioPlay,
            AudioMute,
            MediaSelect,
            Web,
            Mail,
            Calculator,
            Computer,
            ACSearch,
            ACHome,
            ACBack,
            ACForward,
            ACStop,
            ACRefresh,
            ACBookmarks,

            BrightnessDown,
            BrightnessUp,
            DisplaySwitch,
            KeyBoardIlluminationToggle,
            KeyBoardIlluminationDown,
            KeyBoardIlluminationUp,
            Eject,
            Sleep,
            App1,
            App2,

            AudioRewind,
            AudioFastForward,

            SoftLeft,
            SoftRight,
            Call,
            EndCall
        };
        enum class GamePadButton
        {
            A,
            B,
            X,
            Y,

            Back,
            Start,
            LeftStick,
            RightStick,

            LeftShoulder,
            RightShoulder,

            Up,
            Down,
            Left,
            Right
        };

		using Button = std::variant<Key, GamePadButton>;

		enum class State
		{
			pressed = 0b01,
			released = 0b10,
			down = 0b11
		};

		ButtonInput(Button button, State state);
		ButtonInput(const ButtonInput&) = default;
		ButtonInput(ButtonInput&&) noexcept = default;

		~ButtonInput() = default;

		ButtonInput& operator=(const ButtonInput&) = default;
		ButtonInput& operator=(ButtonInput&&) noexcept = default;

		bool operator<(const ButtonInput& otherButtonInput) const;

		//template<typename ButtonType>
		//requires std::same_as<ButtonType, SDL_Scancode> or std::same_as<ButtonType, SDL_GameControllerButton>
		//std::optional<ButtonType> getButton() const
		//{
		//	if (std::holds_alternative<ButtonType>(m_Button))
		//		return std::get<ButtonType>(m_Button);

		//	return std::nullopt;
		//};

		const Button m_Button;
		const State m_State;
	};
}