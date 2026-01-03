//--------------------------------------------------------------------------------
// RequestType.h
//--------------------------------------------------------------------------------

#ifndef __REQUEST_TYPES_H__
#define __REQUEST_TYPES_H__

//--------------------------------------------------------------------------------
// RequestMode
//--------------------------------------------------------------------------------
enum REQUEST_CLIENT_MODE
{
	REQUEST_CLIENT_MODE_NULL,

	REQUEST_CLIENT_MODE_WHISPER,
	REQUEST_CLIENT_MODE_PROFILE,
	REQUEST_CLIENT_MODE_GUILDMARK,
	REQUEST_CLIENT_MODE_POSITION_REPEATLY,

	REQUEST_CLIENT_MODE_CHARACTER_INFO,

	REQUEST_CLIENT_MODE_MAX
};

//--------------------------------------------------------------------------------
// RequestFileType
//--------------------------------------------------------------------------------
enum REQUEST_FILE_TYPE
{
	REQUEST_FILE_NULL,

	REQUEST_FILE_GUILDMARK,
	REQUEST_FILE_PROFILE,
	REQUEST_FILE_PROFILE_INDEX,
	REQUEST_FILE_NORMAL,
};

#endif