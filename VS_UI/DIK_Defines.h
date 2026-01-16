/*=============================================================================

	DIK_Defines.h

	DirectInput Key Code Definitions for cross-platform compatibility.
	These codes map to DirectInput DIK_* constants.

=============================================================================*/

#ifndef __DIK_DEFINES_H__
#define __DIK_DEFINES_H__

/* ============================================================================
 * DirectInput Key Codes (DIK_* constants)
 * ============================================================================ */

/* Letters A-Z */
#define DIK_A			0x1E
#define DIK_B			0x30
#define DIK_C			0x2E
#define DIK_D			0x20
#define DIK_E			0x12
#define DIK_F			0x21
#define DIK_G			0x22
#define DIK_H			0x23
#define DIK_I			0x17
#define DIK_J			0x24
#define DIK_K			0x25
#define DIK_L			0x26
#define DIK_M			0x32
#define DIK_N			0x31
#define DIK_O			0x18
#define DIK_P			0x19
#define DIK_Q			0x10
#define DIK_R			0x13
#define DIK_S			0x1F
#define DIK_T			0x14
#define DIK_U			0x16
#define DIK_V			0x2F
#define DIK_W			0x11
#define DIK_X			0x2D
#define DIK_Y			0x15
#define DIK_Z			0x2C

/* Numbers 0-9 */
#define DIK_0			0x0B
#define DIK_1			0x02
#define DIK_2			0x03
#define DIK_3			0x04
#define DIK_4			0x05
#define DIK_5			0x06
#define DIK_6			0x07
#define DIK_7			0x08
#define DIK_8			0x09
#define DIK_9			0x0A

/* Number pad */
#define DIK_NUMPAD0		0x52
#define DIK_NUMPAD1		0x4F
#define DIK_NUMPAD2		0x50
#define DIK_NUMPAD3		0x51
#define DIK_NUMPAD4		0x4B
#define DIK_NUMPAD5		0x4C
#define DIK_NUMPAD6		0x4D
#define DIK_NUMPAD7		0x47
#define DIK_NUMPAD8		0x48
#define DIK_NUMPAD9		0x49
#define DIK_NUMPADSLASH		0xB5
#define DIK_NUMPADSTAR		0x37
#define DIK_NUMPADMINUS		0x4A
#define DIK_NUMPADPLUS		0x4E
#define DIK_NUMPADPERIOD	0x53
#define DIK_NUMPADENTER		0x9C

/* Function keys */
#define DIK_F1			0x3B
#define DIK_F2			0x3C
#define DIK_F3			0x3D
#define DIK_F4			0x3E
#define DIK_F5			0x3F
#define DIK_F6			0x40
#define DIK_F7			0x41
#define DIK_F8			0x42
#define DIK_F9			0x43
#define DIK_F10			0x44
#define DIK_F11			0x57
#define DIK_F12			0x58
#define DIK_F13			0x64
#define DIK_F14			0x65
#define DIK_F15			0x66

/* Special keys */
#define DIK_ESCAPE		0x01
#define DIK_TAB			0x0F
#define DIK_RETURN		0x1C
#define DIK_BACK		0x0E
#define DIK_SPACE		0x39
#define DIK_CAPITAL		0x3A

/* Modifier keys */
#define DIK_LCONTROL		0x1D
#define DIK_RCONTROL		0x9D
#define DIK_LSHIFT		0x2A
#define DIK_RSHIFT		0x36
#define DIK_LMENU		0x38	/* Left ALT */
#define DIK_RMENU		0xB8	/* Right ALT */
#define DIK_LALT		DIK_LMENU
#define DIK_RALT		DIK_RMENU
#define DIK_LWIN		0x5B	/* Left Windows key */
#define DIK_RWIN		0x5C	/* Right Windows key */
#define DIK_APPS		0x5D	/* Application key */

/* Punctuation */
#define DIK_GRAVE		0x29	/* ` ~ */
#define DIK_MINUS		0x0C	/* - _ */
#define DIK_EQUALS		0x0D	/* = + */
#define DIK_LBRACKET		0x1A	/* [ { */
#define DIK_RBRACKET		0x1B	/* ] } */
#define DIK_BACKSLASH		0x2B	/* \ | */
#define DIK_SEMICOLON		0x27	/* ; : */
#define DIK_APOSTROPHE		0x28	/* ' " */
#define DIK_COMMA		0x33	/* , < */
#define DIK_PERIOD		0x34	/* . > */
#define DIK_SLASH		0x35	/* / ? */

/* Navigation keys */
#define DIK_UP			0xC8	/* Up arrow */
#define DIK_DOWN		0xD0	/* Down arrow */
#define DIK_LEFT		0xCB	/* Left arrow */
#define DIK_RIGHT		0xCD	/* Right arrow */
#define DIK_HOME		0xC7	/* Home */
#define DIK_END			0xCF	/* End */
#define DIK_PRIOR		0xC9	/* Page Up */
#define DIK_NEXT		0xD1	/* Page Down */
#define DIK_INSERT		0xD2	/* Insert */
#define DIK_DELETE		0xD3	/* Delete */

/* Print screen / scroll lock / pause */
#define DIK_PRINT		0xD2	/* Print Screen */
#define DIK_SCROLL		0x46	/* Scroll Lock */
#define DIK_PAUSE		0x45	/* Pause */

/* Other special keys */
#define DIK_SYSRQ		0x54
#define DIK_BREAK		0xD3
#define DIK_CANCEL		0x4B	/* Control + Break */
#define DIK_HELP		0x63

/* Japanese/Korean keys */
#define DIK_CONVERT		0x79	/* Japanese conversion */
#define DIK_NOCONVERT		0x7B	/* Japanese non-conversion */
#define DIK_KANA		0x70	/* Japanese Katakana */
#define DIK_KANJI		0x71	/* Japanese Kanji */
#define DIK_HANGUL		0xF2	/* Korean Hangul */
#define DIK_HANJA		0xF1	/* Korean Hanja */

/* OEM specific */
#define DIK_OEM_1		0x27	/* ; : */
#define DIK_OEM_2		0x28	/* ' " */
#define DIK_OEM_3		0x29	/* ` ~ */
#define DIK_OEM_4		0x1A	/* [ { */
#define DIK_OEM_5		0x2B	/* \ | */
#define DIK_OEM_6		0x1B	/* ] } */
#define DIK_OEM_7		0x33	/* , < */
#define DIK_OEM_8		0x34	/* . > */
#define DIK_OEM_9		0x35	/* / ? */
#define DIK_OEM_102		0x56	/* Additional key on non-US keyboards */
#define DIK_OEM_PLUS		0x0D	/* = + */
#define DIK_OEM_COMMA		0x33	/* , < */
#define DIK_OEM_MINUS		0x0C	/* - _ */
#define DIK_OEM_PERIOD		0x34	/* . > */

#endif /* __DIK_DEFINES_H__ */
