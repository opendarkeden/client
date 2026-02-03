/*-----------------------------------------------------------------------------

	DXLibBackendSDL.cpp

	SDL2 backend implementation for DXLib.
	Provides SDL2-based input and audio for cross-platform support.

	2025.01.14

-----------------------------------------------------------------------------*/

#define DXLIB_BACKEND_SDL_IMPL

/* Include CDirectInput.h first to get DIK constants */
#include "CDirectInput.h"
#include "DXLibBackend.h"

#ifdef DXLIB_BACKEND_SDL

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Include input focus manager */
#include "../../VS_UI/src/InputFocusManager.h"

/* For MP3/OGG support */
#ifdef SDL_MIXER_MAJOR_VERSION
	#include <SDL_mixer.h>
#endif

/* Fallback: If DIK constants are not defined, define them here */
#ifndef DIK_ESCAPE
	#define DIK_ESCAPE          0x01
	#define DIK_1               0x02
	#define DIK_2               0x03
	#define DIK_3               0x04
	#define DIK_4               0x05
	#define DIK_5               0x06
	#define DIK_6               0x07
	#define DIK_7               0x08
	#define DIK_8               0x09
	#define DIK_9               0x0A
	#define DIK_0               0x0B
	#define DIK_MINUS           0x0C
	#define DIK_EQUALS          0x0D
	#define DIK_BACK            0x0E
	#define DIK_TAB             0x0F
	#define DIK_Q               0x10
	#define DIK_W               0x11
	#define DIK_E               0x12
	#define DIK_R               0x13
	#define DIK_T               0x14
	#define DIK_Y               0x15
	#define DIK_U               0x16
	#define DIK_I               0x17
	#define DIK_O               0x18
	#define DIK_P               0x19
	#define DIK_LBRACKET        0x1A
	#define DIK_RBRACKET        0x1B
	#define DIK_RETURN          0x1C
	#define DIK_LCONTROL        0x1D
	#define DIK_A               0x1E
	#define DIK_S               0x1F
	#define DIK_D               0x20
	#define DIK_F               0x21
	#define DIK_G               0x22
	#define DIK_H               0x23
	#define DIK_J               0x24
	#define DIK_K               0x25
	#define DIK_L               0x26
	#define DIK_SEMICOLON       0x27
	#define DIK_APOSTROPHE      0x28
	#define DIK_GRAVE           0x29
	#define DIK_LSHIFT          0x2A
	#define DIK_BACKSLASH       0x2B
	#define DIK_Z               0x2C
	#define DIK_X               0x2D
	#define DIK_C               0x2E
	#define DIK_V               0x2F
	#define DIK_B               0x30
	#define DIK_N               0x31
	#define DIK_M               0x32
	#define DIK_COMMA           0x33
	#define DIK_PERIOD          0x34
	#define DIK_SLASH           0x35
	#define DIK_RSHIFT          0x36
	#define DIK_MULTIPLY        0x37
	#define DIK_LMENU           0x38
	#define DIK_SPACE           0x39
	#define DIK_CAPITAL         0x3A
	#define DIK_F1              0x3B
	#define DIK_F2              0x3C
	#define DIK_F3              0x3D
	#define DIK_F4              0x3E
	#define DIK_F5              0x3F
	#define DIK_F6              0x40
	#define DIK_F7              0x41
	#define DIK_F8              0x42
	#define DIK_F9              0x43
	#define DIK_F10             0x44
	#define DIK_NUMLOCK         0x45
	#define DIK_SCROLL          0x46
	#define DIK_NUMPAD7         0x47
	#define DIK_NUMPAD8         0x48
	#define DIK_NUMPAD9         0x49
	#define DIK_SUBTRACT        0x4A
	#define DIK_NUMPAD4         0x4B
	#define DIK_NUMPAD5         0x4C
	#define DIK_NUMPAD6         0x4D
	#define DIK_ADD             0x4E
	#define DIK_NUMPAD1         0x4F
	#define DIK_NUMPAD2         0x50
	#define DIK_NUMPAD3         0x51
	#define DIK_NUMPAD0         0x52
	#define DIK_DECIMAL         0x53
	#define DIK_F11             0x57
	#define DIK_F12             0x58
	#define DIK_F13             0x64
	#define DIK_F14             0x65
	#define DIK_F15             0x66
	#define DIK_KANA            0x70
	#define DIK_ABNT_C1         0x73
	#define DIK_CONVERT         0x79
	#define DIK_NOCONVERT       0x7B
	#define DIK_YEN             0x7D
	#define DIK_ABNT_C2         0x7E
	#define DIK_NUMPADEQUALS    0x8D
	#define DIK_PREVTRACK       0x90
	#define DIK_AT              0x91
	#define DIK_COLON           0x92
	#define DIK_UNDERLINE       0x93
	#define DIK_KANJI           0x94
	#define DIK_STOP            0x95
	#define DIK_AX              0x96
	#define DIK_UNLABELED       0x97
	#define DIK_NEXTTRACK       0x99
	#define DIK_NUMPADENTER     0x9C
	#define DIK_RCONTROL        0x9D
	#define DIK_MUTE            0xA0
	#define DIK_CALCULATOR      0xA1
	#define DIK_PLAYPAUSE       0xA2
	#define DIK_MEDIASTOP       0xA4
	#define DIK_VOLUMEDOWN      0xAE
	#define DIK_VOLUMEUP        0xB0
	#define DIK_WEBHOME         0xB2
	#define DIK_NUMPADCOMMA     0xB3
	#define DIK_DIVIDE          0xB5
	#define DIK_SYSRQ           0xB7
	#define DIK_RMENU           0xB8
	#define DIK_PAUSE           0xC5
	#define DIK_HOME            0xC7
	#define DIK_UP              0xC8
	#define DIK_PRIOR           0xC9
	#define DIK_LEFT            0xCB
	#define DIK_RIGHT           0xCD
	#define DIK_END             0xCF
	#define DIK_DOWN            0xD0
	#define DIK_NEXT            0xD1
	#define DIK_INSERT          0xD2
	#define DIK_DELETE          0xD3
	#define DIK_LWIN            0xDB
	#define DIK_RWIN            0xDC
	#define DIK_APPS            0xDD
	/* ALT key aliases (DirectInput uses LMENU/RMENU) */
	#define DIK_LALT            DIK_LMENU
	#define DIK_RALT            DIK_RMENU
#endif

/* ============================================================================
 * Internal State
 * ============================================================================ */

static int g_input_initialized = 0;
static int g_sound_initialized = 0;
static int g_music_initialized = 0;
static int g_stream_initialized = 0;

/* Input state */
static Uint8 g_key_state[SDL_NUM_SCANCODES];
static int g_mouse_x = 0;
static int g_mouse_y = 0;
static int g_mouse_wheel = 0;
static int g_mouse_buttons[3] = {0, 0, 0};

/* Text input callback */
static dxlib_textinput_callback g_textinput_callback = NULL;

/* Text editing callback */
static dxlib_textediting_callback g_textediting_callback = NULL;

/* Legacy global mouse coordinates (used by CWaitUIUpdate) */
extern int g_x, g_y;

/* Global game state (from SDLMain.cpp) */
extern bool g_bRunning;
extern BOOL g_bActiveApp;

/* DIK to SDL scancode mapping table */
static SDL_Scancode g_dik_to_scancode[256] = {SDL_SCANCODE_UNKNOWN};

/* ============================================================================
 * Input Backend Implementation
 * ============================================================================ */

static void init_key_mapping(void) {
	/* Initialize DirectInput key code to SDL scancode mapping */
	/* Alphabet */
	g_dik_to_scancode[DIK_A] = SDL_SCANCODE_A;
	g_dik_to_scancode[DIK_B] = SDL_SCANCODE_B;
	g_dik_to_scancode[DIK_C] = SDL_SCANCODE_C;
	g_dik_to_scancode[DIK_D] = SDL_SCANCODE_D;
	g_dik_to_scancode[DIK_E] = SDL_SCANCODE_E;
	g_dik_to_scancode[DIK_F] = SDL_SCANCODE_F;
	g_dik_to_scancode[DIK_G] = SDL_SCANCODE_G;
	g_dik_to_scancode[DIK_H] = SDL_SCANCODE_H;
	g_dik_to_scancode[DIK_I] = SDL_SCANCODE_I;
	g_dik_to_scancode[DIK_J] = SDL_SCANCODE_J;
	g_dik_to_scancode[DIK_K] = SDL_SCANCODE_K;
	g_dik_to_scancode[DIK_L] = SDL_SCANCODE_L;
	g_dik_to_scancode[DIK_M] = SDL_SCANCODE_M;
	g_dik_to_scancode[DIK_N] = SDL_SCANCODE_N;
	g_dik_to_scancode[DIK_O] = SDL_SCANCODE_O;
	g_dik_to_scancode[DIK_P] = SDL_SCANCODE_P;
	g_dik_to_scancode[DIK_Q] = SDL_SCANCODE_Q;
	g_dik_to_scancode[DIK_R] = SDL_SCANCODE_R;
	g_dik_to_scancode[DIK_S] = SDL_SCANCODE_S;
	g_dik_to_scancode[DIK_T] = SDL_SCANCODE_T;
	g_dik_to_scancode[DIK_U] = SDL_SCANCODE_U;
	g_dik_to_scancode[DIK_V] = SDL_SCANCODE_V;
	g_dik_to_scancode[DIK_W] = SDL_SCANCODE_W;
	g_dik_to_scancode[DIK_X] = SDL_SCANCODE_X;
	g_dik_to_scancode[DIK_Y] = SDL_SCANCODE_Y;
	g_dik_to_scancode[DIK_Z] = SDL_SCANCODE_Z;

	/* Numbers */
	g_dik_to_scancode[DIK_0] = SDL_SCANCODE_0;
	g_dik_to_scancode[DIK_1] = SDL_SCANCODE_1;
	g_dik_to_scancode[DIK_2] = SDL_SCANCODE_2;
	g_dik_to_scancode[DIK_3] = SDL_SCANCODE_3;
	g_dik_to_scancode[DIK_4] = SDL_SCANCODE_4;
	g_dik_to_scancode[DIK_5] = SDL_SCANCODE_5;
	g_dik_to_scancode[DIK_6] = SDL_SCANCODE_6;
	g_dik_to_scancode[DIK_7] = SDL_SCANCODE_7;
	g_dik_to_scancode[DIK_8] = SDL_SCANCODE_8;
	g_dik_to_scancode[DIK_9] = SDL_SCANCODE_9;

	/* Function keys */
	g_dik_to_scancode[DIK_F1] = SDL_SCANCODE_F1;
	g_dik_to_scancode[DIK_F2] = SDL_SCANCODE_F2;
	g_dik_to_scancode[DIK_F3] = SDL_SCANCODE_F3;
	g_dik_to_scancode[DIK_F4] = SDL_SCANCODE_F4;
	g_dik_to_scancode[DIK_F5] = SDL_SCANCODE_F5;
	g_dik_to_scancode[DIK_F6] = SDL_SCANCODE_F6;
	g_dik_to_scancode[DIK_F7] = SDL_SCANCODE_F7;
	g_dik_to_scancode[DIK_F8] = SDL_SCANCODE_F8;
	g_dik_to_scancode[DIK_F9] = SDL_SCANCODE_F9;
	g_dik_to_scancode[DIK_F10] = SDL_SCANCODE_F10;
	g_dik_to_scancode[DIK_F11] = SDL_SCANCODE_F11;
	g_dik_to_scancode[DIK_F12] = SDL_SCANCODE_F12;

	/* Special keys */
	g_dik_to_scancode[DIK_ESCAPE] = SDL_SCANCODE_ESCAPE;
	g_dik_to_scancode[DIK_TAB] = SDL_SCANCODE_TAB;
	g_dik_to_scancode[DIK_RETURN] = SDL_SCANCODE_RETURN;
	g_dik_to_scancode[DIK_SPACE] = SDL_SCANCODE_SPACE;
	g_dik_to_scancode[DIK_BACK] = SDL_SCANCODE_BACKSPACE;
	g_dik_to_scancode[DIK_LSHIFT] = SDL_SCANCODE_LSHIFT;
	g_dik_to_scancode[DIK_RSHIFT] = SDL_SCANCODE_RSHIFT;
	g_dik_to_scancode[DIK_LCONTROL] = SDL_SCANCODE_LCTRL;
	g_dik_to_scancode[DIK_RCONTROL] = SDL_SCANCODE_RCTRL;
	g_dik_to_scancode[DIK_LMENU] = SDL_SCANCODE_LALT;
	g_dik_to_scancode[DIK_RMENU] = SDL_SCANCODE_RALT;
	g_dik_to_scancode[DIK_CAPITAL] = SDL_SCANCODE_CAPSLOCK;
	g_dik_to_scancode[DIK_NUMLOCK] = SDL_SCANCODE_NUMLOCKCLEAR;
	g_dik_to_scancode[DIK_SCROLL] = SDL_SCANCODE_SCROLLLOCK;

	/* Arrow keys */
	g_dik_to_scancode[DIK_UP] = SDL_SCANCODE_UP;
	g_dik_to_scancode[DIK_DOWN] = SDL_SCANCODE_DOWN;
	g_dik_to_scancode[DIK_LEFT] = SDL_SCANCODE_LEFT;
	g_dik_to_scancode[DIK_RIGHT] = SDL_SCANCODE_RIGHT;
	g_dik_to_scancode[DIK_HOME] = SDL_SCANCODE_HOME;
	g_dik_to_scancode[DIK_END] = SDL_SCANCODE_END;
	g_dik_to_scancode[DIK_PRIOR] = SDL_SCANCODE_PAGEUP;
	g_dik_to_scancode[DIK_NEXT] = SDL_SCANCODE_PAGEDOWN;
	g_dik_to_scancode[DIK_INSERT] = SDL_SCANCODE_INSERT;
	g_dik_to_scancode[DIK_DELETE] = SDL_SCANCODE_DELETE;

	/* Symbols */
	g_dik_to_scancode[DIK_EQUALS] = SDL_SCANCODE_EQUALS;
	g_dik_to_scancode[DIK_MINUS] = SDL_SCANCODE_MINUS;
	g_dik_to_scancode[DIK_LBRACKET] = SDL_SCANCODE_LEFTBRACKET;
	g_dik_to_scancode[DIK_RBRACKET] = SDL_SCANCODE_RIGHTBRACKET;
	g_dik_to_scancode[DIK_SEMICOLON] = SDL_SCANCODE_SEMICOLON;
	g_dik_to_scancode[DIK_APOSTROPHE] = SDL_SCANCODE_APOSTROPHE;
	g_dik_to_scancode[DIK_GRAVE] = SDL_SCANCODE_GRAVE;
	g_dik_to_scancode[DIK_BACKSLASH] = SDL_SCANCODE_BACKSLASH;
	g_dik_to_scancode[DIK_COMMA] = SDL_SCANCODE_COMMA;
	g_dik_to_scancode[DIK_PERIOD] = SDL_SCANCODE_PERIOD;
	g_dik_to_scancode[DIK_SLASH] = SDL_SCANCODE_SLASH;

	/* Keypad */
	g_dik_to_scancode[DIK_NUMPAD0] = SDL_SCANCODE_KP_0;
	g_dik_to_scancode[DIK_NUMPAD1] = SDL_SCANCODE_KP_1;
	g_dik_to_scancode[DIK_NUMPAD2] = SDL_SCANCODE_KP_2;
	g_dik_to_scancode[DIK_NUMPAD3] = SDL_SCANCODE_KP_3;
	g_dik_to_scancode[DIK_NUMPAD4] = SDL_SCANCODE_KP_4;
	g_dik_to_scancode[DIK_NUMPAD5] = SDL_SCANCODE_KP_5;
	g_dik_to_scancode[DIK_NUMPAD6] = SDL_SCANCODE_KP_6;
	g_dik_to_scancode[DIK_NUMPAD7] = SDL_SCANCODE_KP_7;
	g_dik_to_scancode[DIK_NUMPAD8] = SDL_SCANCODE_KP_8;
	g_dik_to_scancode[DIK_NUMPAD9] = SDL_SCANCODE_KP_9;
	g_dik_to_scancode[DIK_NUMPADENTER] = SDL_SCANCODE_KP_ENTER;
	g_dik_to_scancode[DIK_ADD] = SDL_SCANCODE_KP_PLUS;
	g_dik_to_scancode[DIK_SUBTRACT] = SDL_SCANCODE_KP_MINUS;
	g_dik_to_scancode[DIK_MULTIPLY] = SDL_SCANCODE_KP_MULTIPLY;
	g_dik_to_scancode[DIK_DIVIDE] = SDL_SCANCODE_KP_DIVIDE;
	g_dik_to_scancode[DIK_DECIMAL] = SDL_SCANCODE_KP_PERIOD;
}

int dxlib_input_init(void* window_handle) {
	if (g_input_initialized) return 0;

	/* Initialize SDL subsystems (if not already initialized) */
	if (SDL_WasInit(0) == 0) {
		if (SDL_Init(0) < 0) {
			fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
			return 1;
		}
	}

	/* Initialize key mapping */
	init_key_mapping();

	/* Initialize keyboard state */
	int num_keys;
	const Uint8* state = SDL_GetKeyboardState(&num_keys);
	memcpy(g_key_state, state, sizeof(g_key_state));

	g_input_initialized = 1;
	return 0;
}

void dxlib_input_release(void) {
	g_input_initialized = 0;
}

void dxlib_input_update(void) {
	if (!g_input_initialized) {
		static int notInitCount = 0;
		if (++notInitCount <= 3) {
			printf("WARNING: dxlib_input_update called but not initialized!\n");
			fflush(stdout);
		}
		return;
	}

	/* Update SDL events */
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				g_bRunning = false;
				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
					g_bActiveApp = TRUE;
				} else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
					// Don't deactivate - keep game running in background
					// g_bActiveApp = FALSE;
				}
				break;

			case SDL_KEYDOWN:
				/* Handle control keys for text input */
				if (g_GetInputFocusManager().HasFocus()) {
					SDL_Keycode key = event.key.keysym.sym;
					unsigned int vk_code = 0;

					// Map SDL key codes to Windows virtual key codes
					switch (key) {
					case SDLK_BACKSPACE:	vk_code = 0x08; break; // VK_BACK
					case SDLK_TAB:		vk_code = 0x09; break; // VK_TAB
					case SDLK_RETURN:	vk_code = 0x0D; break; // VK_RETURN
					case SDLK_ESCAPE:	vk_code = 0x1B; break; // VK_ESCAPE
					case SDLK_LEFT:		vk_code = 0x25; break; // VK_LEFT
					case SDLK_UP:		vk_code = 0x26; break; // VK_UP
					case SDLK_RIGHT:	vk_code = 0x27; break; // VK_RIGHT
					case SDLK_DOWN:		vk_code = 0x28; break; // VK_DOWN
					case SDLK_DELETE:	vk_code = 0x2E; break; // VK_DELETE
					case SDLK_HOME:		vk_code = 0x24; break; // VK_HOME
					case SDLK_END:		vk_code = 0x23; break; // VK_END
					}

					if (vk_code != 0) {
						g_GetInputFocusManager().HandleKeyDown(vk_code);
						// Don't break here - let keyboard state update below
						// This ensures dxlib_input_key_down() works correctly
					}
				}
				/* Fall through to update keyboard state */
				/* IMPORTANT: Keyboard state MUST be updated even when InputFocusManager has focus */

			case SDL_KEYUP:
				/* Keyboard state is updated by SDL_GetKeyboardState below */
				/* No additional handling needed */
				break;

			case SDL_MOUSEMOTION:
				g_mouse_x = event.motion.x;
				g_mouse_y = event.motion.y;
				// Also update global g_x and g_y for legacy code
				g_x = event.motion.x;
				g_y = event.motion.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				// Update global g_x and g_y for legacy code
				g_x = event.button.x;
				g_y = event.button.y;

				if (event.button.button == SDL_BUTTON_LEFT) {
					g_mouse_buttons[0] = (event.type == SDL_MOUSEBUTTONDOWN) ? 1 : 0;
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					g_mouse_buttons[1] = (event.type == SDL_MOUSEBUTTONDOWN) ? 1 : 0;
				} else if (event.button.button == SDL_BUTTON_MIDDLE) {
					g_mouse_buttons[2] = (event.type == SDL_MOUSEBUTTONDOWN) ? 1 : 0;
				}
				break;

			case SDL_MOUSEWHEEL:
				g_mouse_wheel += event.wheel.y;
				break;

			case SDL_TEXTINPUT:
				/* Handle text input for IME and text entry */
				{
					// Route to InputFocusManager instead of callback
					if (event.text.text[0] != '\0') {
						g_GetInputFocusManager().HandleTextInput(event.text.text);
					}
				}
				break;

			case SDL_TEXTEDITING:
				/* Handle IME composition (text editing in progress) */
				{
					// Route to InputFocusManager instead of callback
					g_GetInputFocusManager().HandleTextEditing(event.edit.text,
					                                         event.edit.start,
					                                         event.edit.length);
				}
				break;
		}
	}

	/* Update keyboard state */
	const Uint8* state = SDL_GetKeyboardState(NULL);

	/* Update mouse position from SDL (as fallback if no events received) */
	SDL_GetMouseState(&g_mouse_x, &g_mouse_y);

	/* Also update global g_x, g_y for legacy code (IMPORTANT!) */
	g_x = g_mouse_x;
	g_y = g_mouse_y;
}

int dxlib_input_key_down(int dik_key) {
	if (!g_input_initialized) return 0;

	if (dik_key < 0 || dik_key >= 256) return 0;

	SDL_Scancode scancode = g_dik_to_scancode[dik_key];
	if (scancode == SDL_SCANCODE_UNKNOWN) return 0;

	const Uint8* state = SDL_GetKeyboardState(NULL);
	return state[scancode] ? 1 : 0;
}

void dxlib_input_get_mouse_pos(int* x, int* y) {
	if (x) *x = g_mouse_x;
	if (y) *y = g_mouse_y;
}

int dxlib_input_get_mouse_wheel(void) {
	return g_mouse_wheel;
}

void dxlib_input_get_mouse_buttons(int* left, int* right, int* center) {
	if (left) *left = g_mouse_buttons[0];
	if (right) *right = g_mouse_buttons[1];
	if (center) *center = g_mouse_buttons[2];
}

void dxlib_input_set_mouse_pos(int x, int y) {
	SDL_WarpMouseInWindow(NULL, x, y);
}

void dxlib_input_set_textinput_callback(dxlib_textinput_callback callback) {
	g_textinput_callback = callback;
}

void dxlib_input_set_textediting_callback(dxlib_textediting_callback callback) {
	g_textediting_callback = callback;
}

void dxlib_input_start_text(void) {
	SDL_StartTextInput();
}

void dxlib_input_stop_text(void) {
	SDL_StopTextInput();
}

/* ============================================================================
 * Sound Backend Implementation (SDL_mixer)
 * ============================================================================ */

#ifdef SDL_MIXER_MAJOR_VERSION

struct dxlib_sound_buffer {
	Mix_Chunk* chunk;
	int channel;
	int volume;
	int pan;
	int playing;
};

static int g_max_volume = 100;

int dxlib_sound_init(void* window_handle) {
	if (g_sound_initialized) return 0;

	/* Initialize SDL_mixer */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		fprintf(stderr, "Mix_OpenAudio failed: %s\n", Mix_GetError());
		return 1;
	}

	/* Allocate channels */
	Mix_AllocateChannels(32);

	g_sound_initialized = 1;
	return 0;
}

void dxlib_sound_release(void) {
	if (!g_sound_initialized) return;

	Mix_CloseAudio();
	g_sound_initialized = 0;
}

dxlib_sound_t dxlib_sound_load_wav(const char* filename) {
	if (!g_sound_initialized) return NULL;

	Mix_Chunk* chunk = Mix_LoadWAV(filename);
	if (!chunk) {
		fprintf(stderr, "Failed to load %s: %s\n", filename, Mix_GetError());
		return NULL;
	}

	struct dxlib_sound_buffer* sound = (struct dxlib_sound_buffer*)malloc(sizeof(struct dxlib_sound_buffer));
	if (!sound) {
		Mix_FreeChunk(chunk);
		return NULL;
	}

	sound->chunk = chunk;
	sound->channel = -1;
	sound->volume = 100;
	sound->pan = 0;
	sound->playing = 0;

	return sound;
}

dxlib_sound_t dxlib_sound_create_buffer(const void* data, int size,
                                       int channels, int sample_rate,
                                       int bits_per_sample) {
	if (!g_sound_initialized) return NULL;

	/* Convert raw data to SDL_RWops */
	SDL_RWops* rw = SDL_RWFromConstMem(data, size);
	if (!rw) return NULL;

	Mix_Chunk* chunk = Mix_LoadWAV_RW(rw, 1); /* 1 = auto-free */
	if (!chunk) return NULL;

	struct dxlib_sound_buffer* sound = (struct dxlib_sound_buffer*)malloc(sizeof(struct dxlib_sound_buffer));
	if (!sound) {
		Mix_FreeChunk(chunk);
		return NULL;
	}

	sound->chunk = chunk;
	sound->channel = -1;
	sound->volume = 100;
	sound->pan = 0;
	sound->playing = 0;

	return sound;
}

void dxlib_sound_free(dxlib_sound_t sound) {
	if (!sound) return;

	if (sound->playing) {
		Mix_HaltChannel(sound->channel);
	}

	Mix_FreeChunk(sound->chunk);
	free(sound);
}

int dxlib_sound_play(dxlib_sound_t sound, int loop) {
	if (!sound || !sound->chunk) return 1;

	int loops = loop ? -1 : 0; /* -1 = infinite loop */
	sound->channel = Mix_PlayChannel(-1, sound->chunk, loops);
	sound->playing = (sound->channel >= 0);

	return sound->playing ? 0 : 1;
}

int dxlib_sound_stop(dxlib_sound_t sound) {
	if (!sound) return 1;

	if (sound->channel >= 0) {
		Mix_HaltChannel(sound->channel);
		sound->channel = -1;
		sound->playing = 0;
	}

	return 0;
}

int dxlib_sound_is_playing(dxlib_sound_t sound) {
	if (!sound) return 0;
	return sound->playing && (sound->channel >= 0) && Mix_Playing(sound->channel);
}

int dxlib_sound_set_volume(dxlib_sound_t sound, int volume) {
	if (!sound) return 1;

	sound->volume = volume;
	int mix_volume = (volume * 128) / 100; /* Convert to SDL_mixer range */

	if (sound->channel >= 0) {
		Mix_Volume(sound->channel, mix_volume);
	}

	return 0;
}

int dxlib_sound_set_pan(dxlib_sound_t sound, int pan) {
	if (!sound) return 1;

	/* SDL_mixer doesn't support panning directly on channels */
	/* This would require using Mix_SetPosition (which is not always available) */
	/* For now, just store the pan value */
	sound->pan = pan;

	return 0;
}

int dxlib_sound_set_frequency(dxlib_sound_t sound, int frequency) {
	/* SDL_mixer doesn't support changing frequency */
	/* This would require recreating the chunk with resampled data */
	return 1;
}

dxlib_sound_t dxlib_sound_duplicate(dxlib_sound_t sound) {
	if (!sound) return NULL;

	struct dxlib_sound_buffer* duplicate = (struct dxlib_sound_buffer*)malloc(sizeof(struct dxlib_sound_buffer));
	if (!duplicate) return NULL;

	/* Reference the same chunk */
	duplicate->chunk = sound->chunk;
	duplicate->channel = -1;
	duplicate->volume = sound->volume;
	duplicate->pan = sound->pan;
	duplicate->playing = 0;

	return duplicate;
}

#else /* !SDL_MIXER_MAJOR_VERSION */

/* SDL_mixer not available - stub implementation */
int dxlib_sound_init(void* window_handle) { return 1; }
void dxlib_sound_release(void) {}
dxlib_sound_t dxlib_sound_load_wav(const char* filename) { return NULL; }
void dxlib_sound_free(dxlib_sound_t sound) {}
int dxlib_sound_play(dxlib_sound_t sound, int loop) { return 1; }
int dxlib_sound_stop(dxlib_sound_t sound) { return 1; }
int dxlib_sound_is_playing(dxlib_sound_t sound) { return 0; }
int dxlib_sound_set_volume(dxlib_sound_t sound, int volume) { return 1; }
int dxlib_sound_set_pan(dxlib_sound_t sound, int pan) { return 1; }
int dxlib_sound_set_frequency(dxlib_sound_t sound, int frequency) { return 1; }
dxlib_sound_t dxlib_sound_duplicate(dxlib_sound_t sound) { return NULL; }

#endif /* SDL_MIXER_MAJOR_VERSION */

/* ============================================================================
 * Music Backend Implementation (SDL_mixer)
 * ============================================================================ */

#ifdef SDL_MIXER_MAJOR_VERSION

static Mix_Music* g_current_music = NULL;
static int g_music_playing = 0;
static int g_music_paused = 0;
static int g_music_volume = 100;

int dxlib_music_init(void* window_handle) {
	if (g_music_initialized) return 0;

	/* SDL_mixer should already be initialized by dxlib_sound_init */
	if (!g_sound_initialized) {
		if (dxlib_sound_init(window_handle) != 0) {
			return 1;
		}
	}

	g_music_initialized = 1;
	return 0;
}

void dxlib_music_release(void) {
	if (!g_music_initialized) return;

	if (g_current_music) {
		dxlib_music_free();
	}

	g_music_initialized = 0;
}

int dxlib_music_load(const char* filename) {
	if (!g_music_initialized) return 1;

	/* Free previous music */
	if (g_current_music) {
		Mix_FreeMusic(g_current_music);
		g_current_music = NULL;
	}

	g_current_music = Mix_LoadMUS(filename);
	if (!g_current_music) {
		fprintf(stderr, "Failed to load music %s: %s\n", filename, Mix_GetError());
		return 1;
	}

	return 0;
}

void dxlib_music_free(void) {
	if (g_current_music) {
		Mix_FreeMusic(g_current_music);
		g_current_music = NULL;
	}

	g_music_playing = 0;
	g_music_paused = 0;
}

int dxlib_music_play(int loop) {
	if (!g_current_music) return 1;

	int loops = loop ? -1 : 0; /* -1 = infinite loop */
	if (Mix_PlayMusic(g_current_music, loops) < 0) {
		return 1;
	}

	g_music_playing = 1;
	g_music_paused = 0;

	return 0;
}

void dxlib_music_stop(void) {
	Mix_HaltMusic();
	g_music_playing = 0;
	g_music_paused = 0;
}

void dxlib_music_pause(void) {
	if (g_music_playing && !g_music_paused) {
		Mix_PauseMusic();
		g_music_paused = 1;
	}
}

void dxlib_music_resume(void) {
	if (g_music_playing && g_music_paused) {
		Mix_ResumeMusic();
		g_music_paused = 0;
	}
}

int dxlib_music_is_playing(void) {
	return g_music_playing && !g_music_paused && Mix_PlayingMusic();
}

int dxlib_music_is_paused(void) {
	return g_music_paused;
}

int dxlib_music_set_volume(int volume) {
	g_music_volume = volume;
	int mix_volume = (volume * 128) / 100;
	Mix_VolumeMusic(mix_volume);
	return 0;
}

int dxlib_music_set_tempo(float tempo) {
	/* SDL_mixer doesn't support tempo changes */
	return 1;
}

#else /* !SDL_MIXER_MAJOR_VERSION */

/* SDL_mixer not available - stub implementation */
int dxlib_music_init(void* window_handle) { return 1; }
void dxlib_music_release(void) {}
int dxlib_music_load(const char* filename) { return 1; }
void dxlib_music_free(void) {}
int dxlib_music_play(int loop) { return 1; }
void dxlib_music_stop(void) {}
void dxlib_music_pause(void) {}
void dxlib_music_resume(void) {}
int dxlib_music_is_playing(void) { return 0; }
int dxlib_music_is_paused(void) { return 0; }
int dxlib_music_set_volume(int volume) { return 1; }
int dxlib_music_set_tempo(float tempo) { return 1; }

#endif /* SDL_MIXER_MAJOR_VERSION */

/* ============================================================================
 * Stream Backend (uses music backend)
 * ============================================================================ */

int dxlib_stream_init(void* window_handle) {
	return dxlib_music_init(window_handle);
}

void dxlib_stream_release(void) {
	dxlib_music_release();
}

dxlib_stream_t dxlib_stream_load(const char* filename) {
	/* For simplicity, streams use the music backend */
	return (dxlib_stream_t)1; /* Non-null value */
}

void dxlib_stream_free(dxlib_stream_t stream) {
	dxlib_music_free();
}

int dxlib_stream_play(dxlib_stream_t stream, int loop) {
	return dxlib_music_play(loop);
}

void dxlib_stream_stop(dxlib_stream_t stream) {
	dxlib_music_stop();
}

int dxlib_stream_update(dxlib_stream_t stream) {
	/* SDL_mixer handles streaming automatically */
	return 0;
}

int dxlib_stream_set_volume(dxlib_stream_t stream, int volume) {
	return dxlib_music_set_volume(volume);
}

int dxlib_stream_is_playing(dxlib_stream_t stream) {
	return dxlib_music_is_playing();
}

/* ============================================================================
 * Backend Information
 * ============================================================================ */

const char* dxlib_get_backend_name(void) {
	return "SDL2";
}

int dxlib_get_capabilities(void) {
	int caps = 0;

	#ifdef SDL_MIXER_MAJOR_VERSION
		caps |= DXLIB_CAP_SOUND | DXLIB_CAP_MUSIC | DXLIB_CAP_STREAM | DXLIB_CAP_MP3 | DXLIB_CAP_OGG;
	#endif

	caps |= DXLIB_CAP_INPUT;

	return caps;
}

#endif /* DXLIB_BACKEND_SDL */
