#pragma once

namespace CMD_3D_ENGINE
{
	enum Key
	{
		W = 'W', A = 'A', S = 'S', D = 'D',
		Q = 'Q', E = 'E'
	};

	class KeyboardHandler
	{
	private:
		static constexpr unsigned short MASK = 0x8000;
	public:
		KeyboardHandler();
		bool isKeyPressed(Key k) const;
	};

}