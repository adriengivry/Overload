/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <iostream>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

#define COLOR_BLUE OvDebug::blue
#define COLOR_RED OvDebug::red
#define COLOR_GREEN OvDebug::green
#define COLOR_YELLOW OvDebug::yellow
#define COLOR_WHITE OvDebug::white
#define COLOR_DEFAULT OvDebug::grey

namespace OvDebug
{
#ifdef PLATFORM_WINDOWS
	inline std::ostream& blue(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_BLUE | FOREGROUND_GREEN |
			FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& red(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_ERROR_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& green(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& yellow(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_ERROR_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_GREEN | FOREGROUND_RED |
			FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& white(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_GREEN |
			FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		return s;
	}

	inline std::ostream& grey(std::ostream& s)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout,
			FOREGROUND_RED | FOREGROUND_GREEN |
			FOREGROUND_BLUE);
		return s;
	}

	struct color
	{
		color(WORD attribute) : m_color(attribute)
		{
		}

		WORD m_color;
	};

	template <class _Elem, class _Traits>
	std::basic_ostream<_Elem, _Traits>&
		operator<<(std::basic_ostream<_Elem, _Traits>& i, color& c)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout, c.m_color);
		return i;
	}
#else
	inline std::ostream& blue(std::ostream& s)
	{
		s << "\033[34m";
		return s;
	}

	inline std::ostream& red(std::ostream& s)
	{
		s << "\033[31m";
		return s;
	}

	inline std::ostream& green(std::ostream& s)
	{
		s << "\033[32m";
		return s;
	}

	inline std::ostream& yellow(std::ostream& s)
	{
		s << "\033[43m";
		return s;
	}

	inline std::ostream& white(std::ostream& s)
	{
		s << "\033[37m";
		return s;
	}

	inline std::ostream& grey(std::ostream& s)
	{
		s << "\033[30m";
		return s;
	}
#endif // PLATFORM_WINDOWS
}