//----------------------------------------------------------------------
// new PacketNum Edit by Sonic 2006.4.10
// Filename    : Packet.h 
// Written By  : Reiot
// Description : 
// 
//----------------------------------------------------------------------
#ifndef __PACKET_H__
#define __PACKET_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

// forward declaration
class Socket;
class Player;

//----------------------------------------------------------------------
// Packet ID Type
//----------------------------------------------------------------------
typedef ushort PacketID_t;
const uint szPacketID = sizeof(PacketID_t);

//----------------------------------------------------------------------
// Packet Size Type
//----------------------------------------------------------------------
typedef uint   PacketSize_t;
const uint szPacketSize   = sizeof(PacketSize_t);

//----------------------------------------------------------------------
// Packet Sequence Type
//----------------------------------------------------------------------
typedef BYTE   SequenceSize_t;
const uint szSequenceSize   = sizeof(SequenceSize_t);

//----------------------------------------------------------------------
// Packet Header Size
//----------------------------------------------------------------------
const uint szPacketHeader = szPacketID + szPacketSize + szSequenceSize;


//----------------------------------------------------------------------
//
// class Packet;
//
// 패킷 데이타를 나타내는 인터페이스 클래스이다.
//
// read()와 write()를 주의깊게 살펴보라. 소켓입력스트림과 소켓출력스트림은
// 모든 패킷을 어떻게 읽고 쓸 것인지를 알 수 없다. 따라서, 패킷 자신이 각
// 스트림에 어떻게 읽고 쓸 것인지를 알고 있어야 한다.
//
//----------------------------------------------------------------------

class Packet {

public :

	//----------------------------------------------------------------------
	//
	// Packet ID
	//
	// The priority of prefix is G > L > C. (GL/LG, GC/CG, LC/CL)
	// Carefully configure which modules to include which ones.
	// For example, LC is a module that must include both the login server and the client. 
	//----------------------------------------------------------------------
	enum
	{
			PACKET_CG_ADD_SMS_ADDRESS=0,  		//   0
			PACKET_CG_ABSORB_SOUL,		//   1
			PACKET_CG_ACCEPT_UNION,     		//   2
			PACKET_CG_ADD_GEAR_TO_MOUSE,		//   3
			PACKET_CG_ADD_INVENTORY_TO_MOUSE,		//   4
			PACKET_CG_ADD_ITEM_TO_CODE_SHEET,		//   5
			PACKET_CG_ADD_ITEM_TO_ITEM,		//   6
			PACKET_CG_ADD_MOUSE_TO_GEAR,		//   7
			PACKET_CG_ADD_MOUSE_TO_INVENTORY,		//   8
			PACKET_CG_ADD_MOUSE_TO_QUICKSLOT,		//   9
			PACKET_CG_ADD_MOUSE_TO_ZONE,		//   10
			PACKET_CG_ADD_QUICKSLOT_TO_MOUSE,		//   11
			PACKET_CG_ADD_ZONE_TO_INVENTORY,		//   12
			PACKET_CG_ADD_ZONE_TO_MOUSE,		//   13
			PACKET_CG_APPOINT_SUBMASTER,		//   14
			PACKET_CG_ATTACK,		//   15
			PACKET_CG_AUTH_KEY, 		//   16
			PACKET_CG_BLOOD_DRAIN,		//   17
			PACKET_CG_BUY_STORE_ITEM,   		//   18
			PACKET_CG_CASTING_SKILL,		//   19
			PACKET_CG_COMMAND_MONSTER,		//   20
			PACKET_CG_CONNECT,		//   21
			PACKET_CG_CRASH_REPORT,		//   22
			PACKET_CG_DELETE_SMS_ADDRESS,       		//   23
			PACKET_CG_DENY_UNION,       		//   24
			PACKET_CG_DEPOSIT_PET,		//   25
			PACKET_CG_DIAL_UP,		//   26
			PACKET_CG_DISPLAY_ITEM,     		//   27
			PACKET_CG_DISSECTION_CORPSE,		//   28
			PACKET_CG_DONATION_MONEY, 		//   29
			PACKET_CG_DOWN_SKILL,		//   30
			PACKET_CG_DROP_MONEY,		//   31
			PACKET_CG_EXPEL_GUILD,      		//   32
			PACKET_CG_EXPEL_GUILD_MEMBER,		//   33
			PACKET_CG_FAIL_QUEST,		//   34
			PACKET_CG_GET_EVENT_ITEM,  		//   35
			PACKET_CG_GET_OFF_MOTORCYCLE,		//   36
			PACKET_CG_GLOBAL_CHAT,		//   37
			PACKET_CG_GQUEST_ACCEPT,		//   38
			PACKET_CG_GQUEST_CANCEL,		//   39
			PACKET_CG_GUILD_CHAT,		//   40
			PACKET_CG_JOIN_GUILD,		//   41
			PACKET_CG_LEARN_SKILL,		//   42
			PACKET_CG_LOGOUT,		//   43
			PACKET_CG_LOTTERY_SELECT,		//   44
			PACKET_CG_MAKE_ITEM,		//   45
			PACKET_CG_MIX_ITEM,		//   46
			PACKET_CG_MODIFY_GUILD_INTRO,		//   47
			PACKET_CG_MODIFY_GUILD_MEMBER,		//   48
			PACKET_CG_MODIFY_GUILDMEMBER_INTRO,		//   49
			PACKET_CG_MODIFY_NICKNAME,  		//   50
			PACKET_CG_MODIFY_TAX_RATIO,		//   51
			PACKET_CG_MOUSE_TO_STASH,		//   52
			PACKET_CG_MOVE,		//   53
			PACKET_CG_NPC_ASK_ANSWER,		//   54
			PACKET_CG_NPC_TALK,		//   55
			PACKET_CG_PARTY_INVITE,		//   56
			PACKET_CG_PARTY_LEAVE,		//   57
			PACKET_CG_PARTY_POSITION,		//   58
			PACKET_CG_PARTY_SAY,		//   59
			PACKET_CG_PET_GAMBLE,		//   60
			PACKET_CG_PHONE_DISCONNECT,		//   61
			PACKET_CG_PHONE_SAY,		//   62
			PACKET_CG_PICKUP_MONEY,		//   63
			PACKET_CG_PORT_CHECK,		//   64
			PACKET_CG_QUIT_GUILD,		//   65
			PACKET_CG_QUIT_UNION,       		//   66
			PACKET_CG_QUIT_UNION_ACCEPT,		//   67
			PACKET_CG_QUIT_UNION_DENY,  		//   68
			PACKET_CG_RANGER_SAY,		//   69
			PACKET_CG_READY,		//   70
			PACKET_CG_REGIST_GUILD,		//   71
			PACKET_CG_RELIC_TO_OBJECT,		//   72
			PACKET_CG_RELOAD_FROM_INVENTORY,		//   73
			PACKET_CG_RELOAD_FROM_QUICKSLOT,		//   74
			PACKET_CG_REQUEST_GUILD_LIST,       		//   75
			PACKET_CG_REQUEST_GUILD_MEMBER_LIST,		//   76
			PACKET_CG_REQUEST_INFO,		//   77
			PACKET_CG_REQUEST_IP,		//   78
			PACKET_CG_REQUEST_NEWBIE_ITEM,		//   79
			PACKET_CG_REQUEST_POWER_POINT,      		//   80
			PACKET_CG_REQUEST_REPAIR,		//   81
			PACKET_CG_REQUEST_STORE_INFO,       		//   82
			PACKET_CG_REQUEST_UNION,    		//   83
			PACKET_CG_REQUEST_UNION_INFO,       		//   84
			PACKET_CG_RESURRECT,		//   85
			PACKET_CG_RIDE_MOTORCYCLE,		//   86
			PACKET_CG_SAY,		//   87
			PACKET_CG_SELECT_BLOOD_BIBLE,		//   88
			PACKET_CG_SELECT_GUILD,		//   89
			PACKET_CG_SELECT_GUILD_MEMBER,		//   90
			PACKET_CG_SELECT_NICKNAME,  		//   91
			PACKET_CG_SELECT_PORTAL,		//   92
			PACKET_CG_SELECT_QUEST,		//   93
			PACKET_CG_SELECT_RANK_BONUS,		//   94
			PACKET_CG_SELECT_REGEN_ZONE,		//   95
			PACKET_CG_SELECT_TILE_EFFECT,		//   96
			PACKET_CG_SELECT_WAYPOINT,		//   97
			PACKET_CG_SET_SLAYER_HOT_KEY,		//   98
			PACKET_CG_SET_VAMPIRE_HOT_KEY,		//   99
			PACKET_CG_SHOP_REQUEST_BUY,		//   100
			PACKET_CG_SHOP_REQUEST_LIST,		//   101
			PACKET_CG_SHOP_REQUEST_SELL,		//   102
			PACKET_CG_SILVER_COATING,		//   103
			PACKET_CG_SKILL_TO_INVENTORY,		//   104
			PACKET_CG_SKILL_TO_NAMED,		//   105
			PACKET_CG_SKILL_TO_OBJECT,		//   106
			PACKET_CG_SKILL_TO_SELF,		//   107
			PACKET_CG_SKILL_TO_TILE,		//   108
			PACKET_CG_SMS_ADDRESS_LIST, 		//   109
			PACKET_CG_SMS_SEND, 		//   110
			PACKET_CG_STASH_DEPOSIT,		//   111
			PACKET_CG_STASH_LIST,		//   112
			PACKET_CG_STASH_REQUEST_BUY,		//   113
			PACKET_CG_STASH_TO_MOUSE,		//   114
			PACKET_CG_STASH_WITHDRAW,		//   115
			PACKET_CG_STORE_CLOSE,      		//   116
			PACKET_CG_STORE_OPEN,       		//   117
			PACKET_CG_STORE_SIGN,       		//   118
			PACKET_CG_SUBMIT_SCORE,		//   119
			PACKET_CG_TAKE_OUT_GOOD,		//   120
			PACKET_CG_TAME_MONSTER,		//   121
			PACKET_CG_THROW_BOMB,		//   122
			PACKET_CG_THROW_ITEM,		//   123
			PACKET_CG_TRADE_ADD_ITEM,		//   124
			PACKET_CG_TRADE_FINISH,		//   125
			PACKET_CG_TRADE_MONEY,		//   126
			PACKET_CG_TRADE_PREPARE,		//   127
			PACKET_CG_TRADE_REMOVE_ITEM,		//   128
			PACKET_CG_TRY_JOIN_GUILD,		//   129
			PACKET_CG_TYPE_STRING_LIST,		//   130
			PACKET_CG_UNBURROW,		//   131
			PACKET_CG_UNDISPLAY_ITEM,   		//   132
			PACKET_CG_UNTRANSFORM,		//   133
			PACKET_CG_USE_BONUS_POINT,		//   134
			PACKET_CG_USE_ITEM_FROM_GEAR,		//   135
			PACKET_CG_USE_ITEM_FROM_GQUEST_INVENTORY,   		//   136
			PACKET_CG_USE_ITEM_FROM_INVENTORY,		//   137
			PACKET_CG_USE_MESSAGE_ITEM_FROM_INVENTORY,		//   138
			PACKET_CG_USE_POTION_FROM_INVENTORY,		//   139
			PACKET_CG_USE_POTION_FROM_QUICKSLOT,		//   140
			PACKET_CG_USE_POWER_POINT,  		//   141
			PACKET_CG_VERIFY_TIME,		//   142
			PACKET_CG_VISIBLE,		//   143
			PACKET_CG_WHISPER,		//   144
			PACKET_CG_WITHDRAW_PET,		//   145
			PACKET_CG_WITHDRAW_TAX,		//   146
			PACKET_CL_CHANGE_SERVER,		//   147
			PACKET_CL_CREATE_PC,		//   148
			PACKET_CL_DELETE_PC,		//   149
			PACKET_CL_GET_PC_LIST,		//   150
			PACKET_CL_GET_SERVER_LIST,		//   151
			PACKET_CL_GET_WORLD_LIST,		//   152
			PACKET_CL_LOGIN,		//   153
			PACKET_CL_LOGOUT,		//   154
			PACKET_CL_QUERY_CHARACTER_NAME,		//   155
			PACKET_CL_QUERY_PLAYER_ID,		//   156
			PACKET_CL_RECONNECT_LOGIN,		//   157
			PACKET_CL_REGISTER_PLAYER,		//   158
			PACKET_CL_SELECT_PC,		//   159
			PACKET_CL_SELECT_SERVER,		//   160
			PACKET_CL_SELECT_WORLD,		//   161
			PACKET_CL_VERSION_CHECK,		//   162
			PACKET_COMMON_BILLING,		//   163
			PACKET_CR_CONNECT,		//   164
			PACKET_CR_DISCONNECT,		//   165
			PACKET_CR_REQUEST,		//   166
			PACKET_CR_WHISPER,		//   167
			PACKET_CU_BEGIN_UPDATE,		//   168
			PACKET_CU_END_UPDATE,		//   169
			PACKET_CU_REQUEST,		//   170
			PACKET_CU_REQUEST_LOGIN_MODE,		//   171
			PACKET_GC_ACTIVE_GUILD_LIST,		//   172
			PACKET_GC_ADD_BAT,		//   173
			PACKET_GC_ADD_BURROWING_CREATURE,		//   174
			PACKET_GC_ADD_EFFECT,		//   175
			PACKET_GC_ADD_EFFECT_TO_TILE,		//   176
			PACKET_GC_ADD_GEAR_TO_INVENTORY,		//   177
			PACKET_GC_ADD_GEAR_TO_ZONE,		//   178
			PACKET_GC_ADD_HELICOPTER,		//   179
			PACKET_GC_ADD_INJURIOUS_CREATURE,		//   180
			PACKET_GC_ADD_INSTALLED_MINE_TO_ZONE,		//   181
			PACKET_GC_ADD_ITEM_TO_ITEM_VERIFY,		//   182
			PACKET_GC_ADD_MONSTER,		//   183
			PACKET_GC_ADD_MONSTER_CORPSE,		//   184
			PACKET_GC_ADD_MONSTER_FROM_BURROWING,		//   185
			PACKET_GC_ADD_MONSTER_FROM_TRANSFORMATION,		//   186
			PACKET_GC_ADD_NEW_ITEM_TO_ZONE,		//   187
			PACKET_GC_ADD_NICKNAME,     		//   188
			PACKET_GC_ADD_NPC,		//   189
			PACKET_GC_ADD_OUSTERS,		//   190
			PACKET_GC_ADD_OUSTERS_CORPSE,		//   191
			PACKET_GC_ADD_SLAYER,		//   192
			PACKET_GC_ADD_SLAYER_CORPSE,		//   193
			PACKET_GC_ADD_STORE_ITEM,   		//   194
			PACKET_GC_ADD_VAMPIRE,		//   195
			PACKET_GC_ADD_VAMPIRE_CORPSE,		//   196
			PACKET_GC_ADD_VAMPIRE_FROM_BURROWING,		//   197
			PACKET_GC_ADD_VAMPIRE_FROM_TRANSFORMATION,		//   198
			PACKET_GC_ADD_VAMPIRE_PORTAL,		//   199
			PACKET_GC_ADD_WOLF,		//   200
			PACKET_GC_ADDRESS_LIST_VERIFY,      		//   201
			PACKET_GC_ATTACK,		//   202
			PACKET_GC_ATTACK_ARMS_OK_1,		//   203
			PACKET_GC_ATTACK_ARMS_OK_2,		//   204
			PACKET_GC_ATTACK_ARMS_OK_3,		//   205
			PACKET_GC_ATTACK_ARMS_OK_4,		//   206
			PACKET_GC_ATTACK_ARMS_OK_5,		//   207
			PACKET_GC_ATTACK_MELEE_OK_1,		//   208
			PACKET_GC_ATTACK_MELEE_OK_2,		//   209
			PACKET_GC_ATTACK_MELEE_OK_3,		//   210
			PACKET_GC_AUTH_KEY, 		//   211
			PACKET_GC_BLOOD_BIBLE_LIST,		//   212
			PACKET_GC_BLOOD_BIBLE_SIGN_INFO,		//   213
			PACKET_GC_BLOOD_BIBLE_STATUS,		//   214
			PACKET_GC_BLOOD_DRAIN_OK_1,		//   215
			PACKET_GC_BLOOD_DRAIN_OK_2,		//   216
			PACKET_GC_BLOOD_DRAIN_OK_3,		//   217
			PACKET_GC_CANNOT_ADD,		//   218
			PACKET_GC_CANNOT_USE,		//   219
			PACKET_GC_CASTING_SKILL,		//   220
			PACKET_GC_CHANGE_DARK_LIGHT,		//   221
			PACKET_GC_CHANGE_SHAPE,		//   222
			PACKET_GC_CHANGE_WEATHER,		//   223
			PACKET_GC_CREATE_ITEM,		//   224
			PACKET_GC_CREATURE_DIED,		//   225
			PACKET_GC_CROSS_COUNTER_OK_1,		//   226
			PACKET_GC_CROSS_COUNTER_OK_2,		//   227
			PACKET_GC_CROSS_COUNTER_OK_3,		//   228
			PACKET_GC_DELETE_AND_PICKUP_OK,		//   229
			PACKET_GC_DELETE_EFFECT_FROM_TILE,		//   230
			PACKET_GC_DELETE_INVENTORY_ITEM,		//   231
			PACKET_GC_DELETE_OBJECT,		//   232
			PACKET_GC_DISCONNECT,		//   233
			PACKET_GC_DOWN_SKILL_FAILED,		//   234
			PACKET_GC_DOWN_SKILL_OK,		//   235
			PACKET_GC_DROP_ITEM_TO_ZONE,		//   236
			PACKET_GC_ENTER_VAMPIRE_PORTAL,		//   237
			PACKET_GC_EXECUTE_ELEMENT,		//   238
			PACKET_GC_FAKE_MOVE,		//   239
			PACKET_GC_FAST_MOVE,		//   240
			PACKET_GC_FLAG_WAR_STATUS,		//   241
			PACKET_GC_GET_DAMAGE,		//   242
			PACKET_GC_GET_OFF_MOTORCYCLE,		//   243
			PACKET_GC_GET_OFF_MOTORCYCLE_FAILED,		//   244
			PACKET_GC_GET_OFF_MOTORCYCLE_OK,		//   245
			PACKET_GC_GLOBAL_CHAT,		//   246
			PACKET_GC_GOODS_LIST,		//   247
			PACKET_GC_GQUEST_INVENTORY,		//   248
			PACKET_GC_GQUEST_STATUS_INFO,		//   249
			PACKET_GC_GQUEST_STATUS_MODIFY,		//   250
			PACKET_GC_GUILD_CHAT,		//   251
			PACKET_GC_GUILD_MEMBER_LIST,		//   252
			PACKET_GC_GUILD_RESPONSE,   		//   253
			PACKET_GC_HOLY_LAND_BONUS_INFO,		//   254
			PACKET_GC_HP_RECOVERY_END_TO_OTHERS,		//   255
			PACKET_GC_HP_RECOVERY_END_TO_SELF,		//   256
			PACKET_GC_HP_RECOVERY_START_TO_OTHERS,		//   257
			PACKET_GC_HP_RECOVERY_START_TO_SELF,		//   258
			PACKET_GC_KICK_MESSAGE,		//   259
			PACKET_GC_KNOCK_BACK,		//   260
			PACKET_GC_KNOCKS_TARGET_BACK_OK_1,		//   261
			PACKET_GC_KNOCKS_TARGET_BACK_OK_2,		//   262
			PACKET_GC_KNOCKS_TARGET_BACK_OK_4,		//   263
			PACKET_GC_KNOCKS_TARGET_BACK_OK_5,		//   264
			PACKET_GC_LEARN_SKILL_FAILED,		//   265
			PACKET_GC_LEARN_SKILL_OK,		//   266
			PACKET_GC_LEARN_SKILL_READY,		//   267
			PACKET_GC_LIGHTNING,		//   268
			PACKET_GC_MAKE_ITEM_FAIL,		//   269
			PACKET_GC_MAKE_ITEM_OK,		//   270
			PACKET_GC_MINE_EXPLOSION_OK_1,		//   271
			PACKET_GC_MINE_EXPLOSION_OK_2,		//   272
			PACKET_GC_MINI_GAME_SCORES,		//   273
			PACKET_GC_MODIFY_GUILD_MEMBER_INFO,		//   274
			PACKET_GC_MODIFY_INFORMATION,		//   275
			PACKET_GC_MODIFY_MONEY,		//   276
			PACKET_GC_MODIFY_NICKNAME,  		//   277
			PACKET_GC_MONSTER_KILL_QUEST_INFO,		//   278
			PACKET_GC_MORPH_1,		//   279
			PACKET_GC_MORPH_SLAYER_2,		//   280
			PACKET_GC_MORPH_VAMPIRE_2,		//   281
			PACKET_GC_MOVE,		//   282
			PACKET_GC_MOVE_ERROR,		//   283
			PACKET_GC_MOVE_OK,		//   284
			PACKET_GC_MP_RECOVERY_END,		//   285
			PACKET_GC_MP_RECOVERY_START,		//   286
			PACKET_GC_MY_STORE_INFO,    		//   287
			PACKET_GC_NICKNAME_LIST,    		//   288
			PACKET_GC_NICKNAME_VERIFY,  		//   289
			PACKET_GC_NOTICE_EVENT,		//   290
			PACKET_GC_NOTIFY_WIN,		//   291
			PACKET_GC_NPC_ASK,		//   292
			PACKET_GC_NPC_ASK_DYNAMIC,		//   293
			PACKET_GC_NPC_ASK_VARIABLE,		//   294
			PACKET_GC_NPC_INFO,		//   295
			PACKET_GC_NPC_RESPONSE,		//   296
			PACKET_GC_NPC_SAY,		//   297
			PACKET_GC_NPC_SAY_DYNAMIC,		//   298
			PACKET_GC_OTHER_GUILD_NAME,		//   299
			PACKET_GC_OTHER_MODIFY_INFO,		//   300
			PACKET_GC_OTHER_STORE_INFO, 		//   301
			PACKET_GC_PARTY_ERROR,		//   302
			PACKET_GC_PARTY_INVITE,		//   303
			PACKET_GC_PARTY_JOINED,		//   304
			PACKET_GC_PARTY_LEAVE,		//   305
			PACKET_GC_PARTY_POSITION,		//   306
			PACKET_GC_PARTY_SAY,		//   307
			PACKET_GC_PET_INFO,		//   308
			PACKET_GC_PET_STASH_LIST,		//   309
			PACKET_GC_PET_STASH_VERIFY,		//   310
			PACKET_GC_PET_USE_SKILL,		//   311
			PACKET_GC_PHONE_CONNECTED,		//   312
			PACKET_GC_PHONE_CONNECTION_FAILED,		//   313
			PACKET_GC_PHONE_DISCONNECTED,		//   314
			PACKET_GC_PHONE_SAY,		//   315
			PACKET_GC_QUEST_STATUS,		//   316
			PACKET_GC_RANK_BONUS_INFO,		//   317
			PACKET_GC_REAL_WEARING_INFO,		//   318
			PACKET_GC_RECONNECT,		//   319
			PACKET_GC_RECONNECT_LOGIN,		//   320
			PACKET_GC_REGEN_ZONE_STATUS,		//   321
			PACKET_GC_RELOAD_OK,		//   322
			PACKET_GC_REMOVE_CORPSE_HEAD,		//   323
			PACKET_GC_REMOVE_EFFECT,		//   324
			PACKET_GC_REMOVE_FROM_GEAR,		//   325
			PACKET_GC_REMOVE_INJURIOUS_CREATURE,		//   326
			PACKET_GC_REMOVE_STORE_ITEM,		//   327
			PACKET_GC_REQUEST_FAILED,		//   328
			PACKET_GC_REQUEST_POWER_POINT_RESULT,       		//   329
			PACKET_GC_REQUESTED_IP,		//   330
			PACKET_GC_RIDE_MOTORCYCLE,		//   331
			PACKET_GC_RIDE_MOTORCYCLE_FAILED,		//   332
			PACKET_GC_RIDE_MOTORCYCLE_OK,		//   333
			PACKET_GC_RING,		//   334
			PACKET_GC_SAY,		//   335
			PACKET_GC_SEARCH_MOTORCYCLE_FAIL,		//   336
			PACKET_GC_SEARCH_MOTORCYCLE_OK,		//   337
			PACKET_GC_SELECT_QUEST_ID,		//   338
			PACKET_GC_SELECT_RANK_BONUS_FAILED,		//   339
			PACKET_GC_SELECT_RANK_BONUS_OK,		//   340
			PACKET_GC_SET_POSITION,		//   341
			PACKET_GC_SHOP_BOUGHT,		//   342
			PACKET_GC_SHOP_BUY_FAIL,		//   343
			PACKET_GC_SHOP_BUY_OK,		//   344
			PACKET_GC_SHOP_LIST,		//   345
			PACKET_GC_SHOP_LIST_MYSTERIOUS,		//   346
			PACKET_GC_SHOP_MARKET_CONDITION,		//   347
			PACKET_GC_SHOP_SELL_FAIL,		//   348
			PACKET_GC_SHOP_SELL_OK,		//   349
			PACKET_GC_SHOP_SOLD,		//   350
			PACKET_GC_SHOP_VERSION,		//   351
			PACKET_GC_SHOW_GUILD_INFO,		//   352
			PACKET_GC_SHOW_GUILD_JOIN,		//   353
			PACKET_GC_SHOW_GUILD_MEMBER_INFO,		//   354
			PACKET_GC_SHOW_GUILD_REGIST,		//   355
			PACKET_GC_SHOW_MESSAGE_BOX,		//   356
			PACKET_GC_SHOW_UNION_INFO,  		//   357
			PACKET_GC_SHOW_WAIT_GUILD_INFO,		//   358
			PACKET_GC_SKILL_FAILED_1,		//   359
			PACKET_GC_SKILL_FAILED_2,		//   360
			PACKET_GC_SKILL_INFO,		//   361
			PACKET_GC_SKILL_TO_INVENTORY_OK_1,		//   362
			PACKET_GC_SKILL_TO_INVENTORY_OK_2,		//   363
			PACKET_GC_SKILL_TO_OBJECT_OK_1,		//   364
			PACKET_GC_SKILL_TO_OBJECT_OK_2,		//   365
			PACKET_GC_SKILL_TO_OBJECT_OK_3,		//   366
			PACKET_GC_SKILL_TO_OBJECT_OK_4,		//   367
			PACKET_GC_SKILL_TO_OBJECT_OK_5,		//   368
			PACKET_GC_SKILL_TO_OBJECT_OK_6,		//   369
			PACKET_GC_SKILL_TO_SELF_OK_1,		//   370
			PACKET_GC_SKILL_TO_SELF_OK_2,		//   371
			PACKET_GC_SKILL_TO_SELF_OK_3,		//   372
			PACKET_GC_SKILL_TO_TILE_OK_1,		//   373
			PACKET_GC_SKILL_TO_TILE_OK_2,		//   374
			PACKET_GC_SKILL_TO_TILE_OK_3,		//   375
			PACKET_GC_SKILL_TO_TILE_OK_4,		//   376
			PACKET_GC_SKILL_TO_TILE_OK_5,		//   377
			PACKET_GC_SKILL_TO_TILE_OK_6,		//   378
			PACKET_GC_SMS_ADDRESS_LIST, 		//   379
			PACKET_GC_STASH_LIST,		//   380
			PACKET_GC_STASH_SELL,		//   381
			PACKET_GC_STATUS_CURRENT_HP,		//   382
			PACKET_GC_SUB_INVENTORY_INFO,		//   383
			PACKET_GC_SWEEPER_BONUS_INFO,		//   384
			PACKET_GC_SYSTEM_AVAILABILITIES,    		//   385
			PACKET_GC_SYSTEM_MESSAGE,		//   386
			PACKET_GC_TAKE_OFF,		//   387
			PACKET_GC_TAKE_OUT_FAIL,		//   388
			PACKET_GC_TAKE_OUT_OK,		//   389
			PACKET_GC_TEACH_SKILL_INFO,		//   390
			PACKET_GC_THROW_BOMB_OK_1,		//   391
			PACKET_GC_THROW_BOMB_OK_2,		//   392
			PACKET_GC_THROW_BOMB_OK_3,		//   393
			PACKET_GC_THROW_ITEM_OK_1,		//   394
			PACKET_GC_THROW_ITEM_OK_2,		//   395
			PACKET_GC_THROW_ITEM_OK_3,		//   396
			PACKET_GC_TIME_LIMIT_ITEM_INFO,		//   397
			PACKET_GC_TRADE_ADD_ITEM,		//   398
			PACKET_GC_TRADE_ERROR,		//   399
			PACKET_GC_TRADE_FINISH,		//   400
			PACKET_GC_TRADE_MONEY,		//   401
			PACKET_GC_TRADE_PREPARE,		//   402
			PACKET_GC_TRADE_REMOVE_ITEM,		//   403
			PACKET_GC_TRADE_VERIFY,		//   404
			PACKET_GC_UNBURROW_FAIL,		//   405
			PACKET_GC_UNBURROW_OK,		//   406
			PACKET_GC_UNION_OFFER_LIST,		//   407
			PACKET_GC_UNTRANSFORM_FAIL,		//   408
			PACKET_GC_UNTRANSFORM_OK,		//   409
			PACKET_GC_UPDATE_INFO,		//   410
			PACKET_GC_USE_BONUS_POINT_FAIL,		//   411
			PACKET_GC_USE_BONUS_POINT_OK,		//   412
			PACKET_GC_USE_OK,		//   413
			PACKET_GC_USE_POWER_POINT_RESULT,   		//   414
			PACKET_GC_VISIBLE_FAIL,		//   415
			PACKET_GC_VISIBLE_OK,		//   416
			PACKET_GC_WAIT_GUILD_LIST,		//   417
			PACKET_GC_WAR_LIST,		//   418
			PACKET_GC_WAR_SCHEDULE_LIST,		//   419
			PACKET_GC_WHISPER,		//   420
			PACKET_GC_WHISPER_FAILED,		//   421
			PACKET_GG_COMMAND,		//   422
			PACKET_GG_GUILD_CHAT,		//   423
			PACKET_GG_SERVER_CHAT,		//   424
			PACKET_GL_INCOMING_CONNECTION,		//   425
			PACKET_GL_INCOMING_CONNECTION_ERROR,		//   426
			PACKET_GL_INCOMING_CONNECTION_OK,		//   427
			PACKET_GL_KICK_VERIFY,		//   428
			PACKET_GM_SERVER_INFO,		//   429
			PACKET_GS_ADD_GUILD,		//   430
			PACKET_GS_ADD_GUILD_MEMBER,		//   431
			PACKET_GS_EXPEL_GUILD_MEMBER,		//   432
			PACKET_GS_GUILD_ACTION,     		//   433
			PACKET_GS_GUILDMEMBER_LOGON,		//   434
			PACKET_GS_MODIFY_GUILD_INTRO,		//   435
			PACKET_GS_MODIFY_GUILD_MEMBER,		//   436
			PACKET_GS_QUIT_GUILD,		//   437
			PACKET_GS_REQUEST_GUILD_INFO,		//   438
			PACKET_GTO_ACKNOWLEDGEMENT, 		//   439
			PACKET_LC_CREATE_PC_ERROR,		//   440
			PACKET_LC_CREATE_PC_OK,		//   441
			PACKET_LC_DELETE_PC_ERROR,		//   442
			PACKET_LC_DELETE_PC_OK,		//   443
			PACKET_LC_LOGIN_ERROR,		//   444
			PACKET_LC_LOGIN_OK,		//   445
			PACKET_LC_PC_LIST,		//   446
			PACKET_LC_PORT_CHECK,		//   447
			PACKET_LC_QUERY_RESULT_CHARACTER_NAME,		//   448
			PACKET_LC_QUERY_RESULT_PLAYER_ID,		//   449
			PACKET_LC_RECONNECT,		//   450
			PACKET_LC_REGISTER_PLAYER_ERROR,		//   451
			PACKET_LC_REGISTER_PLAYER_OK,		//   452
			PACKET_LC_SELECT_PC_ERROR,		//   453
			PACKET_LC_SERVER_LIST,		//   454
			PACKET_LC_VERSION_CHECK_ERROR,		//   455
			PACKET_LC_VERSION_CHECK_OK,		//   456
			PACKET_LC_WORLD_LIST,		//   457
			PACKET_LG_INCOMING_CONNECTION,		//   458
			PACKET_LG_INCOMING_CONNECTION_ERROR,		//   459
			PACKET_LG_INCOMING_CONNECTION_OK,		//   460
			PACKET_LG_KICK_CHARACTER,		//   461
			PACKET_RC_CHARACTER_INFO,		//   462
			PACKET_RC_CONNECT_VERIFY,		//   463
			PACKET_RC_POSITION_INFO,		//   464
			PACKET_RC_REQUEST_VERIFY,		//   465
			PACKET_RC_REQUESTED_FILE,		//   466
			PACKET_RC_SAY,		//   467
			PACKET_RC_STATUS_HP,		//   468
			PACKET_SG_ADD_GUILD_MEMBER_OK,		//   469
			PACKET_SG_ADD_GUILD_OK,		//   470
			PACKET_SG_DELETE_GUILD_OK,		//   471
			PACKET_SG_EXPEL_GUILD_MEMBER_OK,		//   472
			PACKET_SG_GUILD_INFO,		//   473
			PACKET_SG_GUILD_RESPONSE,   		//   474
			PACKET_SG_GUILDMEMBER_LOGON_OK,		//   475
			PACKET_SG_MODIFY_GUILD_INTRO_OK,		//   476
			PACKET_SG_MODIFY_GUILD_MEMBER_OK,		//   477
			PACKET_SG_MODIFY_GUILD_OK,		//   478
			PACKET_SG_QUIT_GUILD_OK,		//   479
			PACKET_UC_REQUEST_LOGIN_MODE,		//   480
			PACKET_UC_UPDATE,		//   481
			PACKET_UC_UPDATE_LIST,		//   482
			PACKET_CG_ENCODE_KEY,		//   483 add by viva 2008-12-27 : Packet Encode Key
			PACKET_GC_FRIEND_CHATTING,	//add by viva 
			PACKET_GC_USE_SKILLCARD_OK, // by viva
        		PACKET_MAX
	};

public :
	
	// destructor
	virtual ~Packet ()  {}

	// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
	virtual void read (SocketInputStream & iStream)  = 0;

	// 소켓으로부터 직접 데이터를 읽어서 패킷을 초기화한다.
	virtual void read (Socket* pSocket)  { throw UnsupportedError(); }	

	// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
	virtual void write (SocketOutputStream & oStream) const  = 0;

	// 소켓으로 직접 패킷의 바이너리 이미지를 보낸다.
	virtual void write (Socket* pSocket) const  { throw UnsupportedError(); }	

	// 출력 스트림에 패킷의 헤더와 바디를 모두 쓴다.
	void writeHeaderNBody (SocketOutputStream& oStream ) const
	{
		oStream.write( getPacketID() );
		oStream.write( getPacketSize() );
		oStream.write( "0",1 );
		if ( getPacketSize() != 0 ) {
			write( oStream );
			if (oStream.length() != getPacketSize() + szPacketID + szPacketSize + 1) {
			  std::cout << "writeHeaderNBody: " << getPacketID() << " size:" << getPacketSize() << std::endl;
			}
		}
	}
	
	// execute packet's handler
	virtual void execute (Player* pPlayer)  = 0;
	
	// get packet's PacketID	
	virtual PacketID_t getPacketID () const  = 0;

	// get packet's size
	virtual PacketSize_t getPacketSize () const  = 0;

	#if !defined(__GAME_CLIENT__) || defined(__DEBUG_OUTPUT__)
		// get packet's name
		virtual string getPacketName () const  = 0;
		
		// get packet's debug string
		virtual string toString () const  = 0;
	#endif
	
};

#endif
