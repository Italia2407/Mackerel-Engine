#pragma once

#include <cstdint>
#include "InputSetRanges.h"

namespace MCK
{
	/**
	 * This enum class contains an enum for detectible keyboard inputs; it is
	 *     based on the GLFW key constants.
	 */
	namespace Key
	{
		const int32_t SPACE = MKC_KEY_OFFSET + 32;
		const int32_t APOSTROPHE = MKC_KEY_OFFSET + 39 /* ' */;
		const int32_t COMMA = MKC_KEY_OFFSET + 44 /* ; */;
		const int32_t MINUS = MKC_KEY_OFFSET + 45 /* - */;
		const int32_t PERIOD = MKC_KEY_OFFSET + 46 /* . */;
		const int32_t SLASH = MKC_KEY_OFFSET + 47 /* / */;
		const int32_t _0 = MKC_KEY_OFFSET + 48;
		const int32_t _1 = MKC_KEY_OFFSET + 49;
		const int32_t _2 = MKC_KEY_OFFSET + 50;
		const int32_t _3 = MKC_KEY_OFFSET + 51;
		const int32_t _4 = MKC_KEY_OFFSET + 52;
		const int32_t _5 = MKC_KEY_OFFSET + 53;
		const int32_t _6 = MKC_KEY_OFFSET + 54;
		const int32_t _7 = MKC_KEY_OFFSET + 55;
		const int32_t _8 = MKC_KEY_OFFSET + 56;
		const int32_t _9 = MKC_KEY_OFFSET + 57;
		const int32_t SEMICOLON = MKC_KEY_OFFSET + 59 /* ; */;
		const int32_t EQUAL = MKC_KEY_OFFSET + 61 /* = keyOffset + */;
		const int32_t A = MKC_KEY_OFFSET + 65;
		const int32_t B = MKC_KEY_OFFSET + 66;
		const int32_t C = MKC_KEY_OFFSET + 67;
		const int32_t D = MKC_KEY_OFFSET + 68;
		const int32_t E = MKC_KEY_OFFSET + 69;
		const int32_t F = MKC_KEY_OFFSET + 70;
		const int32_t G = MKC_KEY_OFFSET + 71;
		const int32_t H = MKC_KEY_OFFSET + 72;
		const int32_t I = MKC_KEY_OFFSET + 73;
		const int32_t J = MKC_KEY_OFFSET + 74;
		const int32_t K = MKC_KEY_OFFSET + 75;
		const int32_t L = MKC_KEY_OFFSET + 76;
		const int32_t M = MKC_KEY_OFFSET + 77;
		const int32_t N = MKC_KEY_OFFSET + 78;
		const int32_t O = MKC_KEY_OFFSET + 79;
		const int32_t P = MKC_KEY_OFFSET + 80;
		const int32_t Q = MKC_KEY_OFFSET + 81;
		const int32_t R = MKC_KEY_OFFSET + 82;
		const int32_t S = MKC_KEY_OFFSET + 83;
		const int32_t T = MKC_KEY_OFFSET + 84;
		const int32_t U = MKC_KEY_OFFSET + 85;
		const int32_t V = MKC_KEY_OFFSET + 86;
		const int32_t W = MKC_KEY_OFFSET + 87;
		const int32_t X = MKC_KEY_OFFSET + 88;
		const int32_t Y = MKC_KEY_OFFSET + 89;
		const int32_t Z = MKC_KEY_OFFSET + 90;
		const int32_t LEFT_BRACKET = MKC_KEY_OFFSET + 91 /* [ */;
		const int32_t BACKSLASH = MKC_KEY_OFFSET + 92 /* \ */;
		const int32_t RIGHT_BRACKET = MKC_KEY_OFFSET + 93 /* ] */;
		const int32_t GRAVE_ACCENT = MKC_KEY_OFFSET + 96 /* ` */;
		const int32_t WORLD_1 = MKC_KEY_OFFSET + 161 /* non-US #1 */;
		const int32_t WORLD_2 = MKC_KEY_OFFSET + 162 /* non-US #2 */;
		const int32_t ESCAPE = MKC_KEY_OFFSET + 256;
		const int32_t ENTER = MKC_KEY_OFFSET + 257;
		const int32_t TAB = MKC_KEY_OFFSET + 258;
		const int32_t BACKSPACE = MKC_KEY_OFFSET + 259;
		const int32_t INSERT = MKC_KEY_OFFSET + 260;
		const int32_t _DELETE = MKC_KEY_OFFSET + 261;
		const int32_t RIGHT = MKC_KEY_OFFSET + 262;
		const int32_t LEFT = MKC_KEY_OFFSET + 263;
		const int32_t DOWN = MKC_KEY_OFFSET + 264;
		const int32_t UP = MKC_KEY_OFFSET + 265;
		const int32_t PAGE_UP = MKC_KEY_OFFSET + 266;
		const int32_t PAGE_DOWN = MKC_KEY_OFFSET + 267;
		const int32_t HOME = MKC_KEY_OFFSET + 268;
		const int32_t END = MKC_KEY_OFFSET + 269;
		const int32_t CAPS_LOCK = MKC_KEY_OFFSET + 280;
		const int32_t SCROLL_LOCK = MKC_KEY_OFFSET + 281;
		const int32_t NUM_LOCK = MKC_KEY_OFFSET + 282;
		const int32_t PRINT_SCREEN = MKC_KEY_OFFSET + 283;
		const int32_t PAUSE = MKC_KEY_OFFSET + 284;
		const int32_t F1 = MKC_KEY_OFFSET + 290;
		const int32_t F2 = MKC_KEY_OFFSET + 291;
		const int32_t F3 = MKC_KEY_OFFSET + 292;
		const int32_t F4 = MKC_KEY_OFFSET + 293;
		const int32_t F5 = MKC_KEY_OFFSET + 294;
		const int32_t F6 = MKC_KEY_OFFSET + 295;
		const int32_t F7 = MKC_KEY_OFFSET + 296;
		const int32_t F8 = MKC_KEY_OFFSET + 297;
		const int32_t F9 = MKC_KEY_OFFSET + 298;
		const int32_t F10 = MKC_KEY_OFFSET + 299;
		const int32_t F11 = MKC_KEY_OFFSET + 300;
		const int32_t F12 = MKC_KEY_OFFSET + 301;
		const int32_t F13 = MKC_KEY_OFFSET + 302;
		const int32_t F14 = MKC_KEY_OFFSET + 303;
		const int32_t F15 = MKC_KEY_OFFSET + 304;
		const int32_t F16 = MKC_KEY_OFFSET + 305;
		const int32_t F17 = MKC_KEY_OFFSET + 306;
		const int32_t F18 = MKC_KEY_OFFSET + 307;
		const int32_t F19 = MKC_KEY_OFFSET + 308;
		const int32_t F20 = MKC_KEY_OFFSET + 309;
		const int32_t F21 = MKC_KEY_OFFSET + 310;
		const int32_t F22 = MKC_KEY_OFFSET + 311;
		const int32_t F23 = MKC_KEY_OFFSET + 312;
		const int32_t F24 = MKC_KEY_OFFSET + 313;
		const int32_t F25 = MKC_KEY_OFFSET + 314;
		const int32_t KP_0 = MKC_KEY_OFFSET + 320;
		const int32_t KP_1 = MKC_KEY_OFFSET + 321;
		const int32_t KP_2 = MKC_KEY_OFFSET + 322;
		const int32_t KP_3 = MKC_KEY_OFFSET + 323;
		const int32_t KP_4 = MKC_KEY_OFFSET + 324;
		const int32_t KP_5 = MKC_KEY_OFFSET + 325;
		const int32_t KP_6 = MKC_KEY_OFFSET + 326;
		const int32_t KP_7 = MKC_KEY_OFFSET + 327;
		const int32_t KP_8 = MKC_KEY_OFFSET + 328;
		const int32_t KP_9 = MKC_KEY_OFFSET + 329;
		const int32_t KP_DECIMAL = MKC_KEY_OFFSET + 330;
		const int32_t KP_DIVIDE = MKC_KEY_OFFSET + 331;
		const int32_t KP_MULTIPLY = MKC_KEY_OFFSET + 332;
		const int32_t KP_SUBTRACT = MKC_KEY_OFFSET + 333;
		const int32_t KP_ADD = MKC_KEY_OFFSET + 334;
		const int32_t KP_ENTER = MKC_KEY_OFFSET + 335;
		const int32_t KP_EQUAL = MKC_KEY_OFFSET + 336;
		const int32_t LEFT_SHIFT = MKC_KEY_OFFSET + 340;
		const int32_t LEFT_CONTROL = MKC_KEY_OFFSET + 341;
		const int32_t LEFT_ALT = MKC_KEY_OFFSET + 342;
		const int32_t LEFT_SUPER = MKC_KEY_OFFSET + 343;
		const int32_t RIGHT_SHIFT = MKC_KEY_OFFSET + 344;
		const int32_t RIGHT_CONTROL = MKC_KEY_OFFSET + 345;
		const int32_t RIGHT_ALT = MKC_KEY_OFFSET + 346;
		const int32_t RIGHT_SUPER = MKC_KEY_OFFSET + 347;
		const int32_t MENU = MKC_KEY_OFFSET + 348;
	}
}
