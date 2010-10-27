#include <linux/input.h>

#define EV_MAP( N ) [ N ] = #N

static char *EV_NAME[EV_MAX] = {
#ifdef EV_SYN
	EV_MAP( EV_SYN ),
#endif
#ifdef EV_KEY
	EV_MAP( EV_KEY ),
#endif
#ifdef EV_REL
	EV_MAP( EV_REL ),
#endif
#ifdef EV_ABS
	EV_MAP( EV_ABS ),
#endif
#ifdef EV_MSC
	EV_MAP( EV_MSC ),
#endif
#ifdef EV_SW
	EV_MAP( EV_SW ),
#endif
#ifdef EV_LED
	EV_MAP( EV_LED ),
#endif
#ifdef EV_SND
	EV_MAP( EV_SND ),
#endif
#ifdef EV_REP
	EV_MAP( EV_REP ),
#endif
#ifdef EV_FF
	EV_MAP( EV_FF ),
#endif
#ifdef EV_PWR
	EV_MAP( EV_PWR ),
#endif
#ifdef EV_FF_STATUS
	EV_MAP( EV_FF_STATUS ),
#endif
};

static char *KEY_NAME[KEY_MAX] = {
#ifdef KEY_RESERVED
	EV_MAP( KEY_RESERVED ),
#endif
#ifdef KEY_ESC
	EV_MAP( KEY_ESC ),
#endif
#ifdef KEY_1
	EV_MAP( KEY_1 ),
#endif
#ifdef KEY_2
	EV_MAP( KEY_2 ),
#endif
#ifdef KEY_3
	EV_MAP( KEY_3 ),
#endif
#ifdef KEY_4
	EV_MAP( KEY_4 ),
#endif
#ifdef KEY_5
	EV_MAP( KEY_5 ),
#endif
#ifdef KEY_6
	EV_MAP( KEY_6 ),
#endif
#ifdef KEY_7
	EV_MAP( KEY_7 ),
#endif
#ifdef KEY_8
	EV_MAP( KEY_8 ),
#endif
#ifdef KEY_9
	EV_MAP( KEY_9 ),
#endif
#ifdef KEY_0
	EV_MAP( KEY_0 ),
#endif
#ifdef KEY_MINUS
	EV_MAP( KEY_MINUS ),
#endif
#ifdef KEY_EQUAL
	EV_MAP( KEY_EQUAL ),
#endif
#ifdef KEY_BACKSPACE
	EV_MAP( KEY_BACKSPACE ),
#endif
#ifdef KEY_TAB
	EV_MAP( KEY_TAB ),
#endif
#ifdef KEY_Q
	EV_MAP( KEY_Q ),
#endif
#ifdef KEY_W
	EV_MAP( KEY_W ),
#endif
#ifdef KEY_E
	EV_MAP( KEY_E ),
#endif
#ifdef KEY_R
	EV_MAP( KEY_R ),
#endif
#ifdef KEY_T
	EV_MAP( KEY_T ),
#endif
#ifdef KEY_Y
	EV_MAP( KEY_Y ),
#endif
#ifdef KEY_U
	EV_MAP( KEY_U ),
#endif
#ifdef KEY_I
	EV_MAP( KEY_I ),
#endif
#ifdef KEY_O
	EV_MAP( KEY_O ),
#endif
#ifdef KEY_P
	EV_MAP( KEY_P ),
#endif
#ifdef KEY_LEFTBRACE
	EV_MAP( KEY_LEFTBRACE ),
#endif
#ifdef KEY_RIGHTBRACE
	EV_MAP( KEY_RIGHTBRACE ),
#endif
#ifdef KEY_ENTER
	EV_MAP( KEY_ENTER ),
#endif
#ifdef KEY_LEFTCTRL
	EV_MAP( KEY_LEFTCTRL ),
#endif
#ifdef KEY_A
	EV_MAP( KEY_A ),
#endif
#ifdef KEY_S
	EV_MAP( KEY_S ),
#endif
#ifdef KEY_D
	EV_MAP( KEY_D ),
#endif
#ifdef KEY_F
	EV_MAP( KEY_F ),
#endif
#ifdef KEY_G
	EV_MAP( KEY_G ),
#endif
#ifdef KEY_H
	EV_MAP( KEY_H ),
#endif
#ifdef KEY_J
	EV_MAP( KEY_J ),
#endif
#ifdef KEY_K
	EV_MAP( KEY_K ),
#endif
#ifdef KEY_L
	EV_MAP( KEY_L ),
#endif
#ifdef KEY_SEMICOLON
	EV_MAP( KEY_SEMICOLON ),
#endif
#ifdef KEY_APOSTROPHE
	EV_MAP( KEY_APOSTROPHE ),
#endif
#ifdef KEY_GRAVE
	EV_MAP( KEY_GRAVE ),
#endif
#ifdef KEY_LEFTSHIFT
	EV_MAP( KEY_LEFTSHIFT ),
#endif
#ifdef KEY_BACKSLASH
	EV_MAP( KEY_BACKSLASH ),
#endif
#ifdef KEY_Z
	EV_MAP( KEY_Z ),
#endif
#ifdef KEY_X
	EV_MAP( KEY_X ),
#endif
#ifdef KEY_C
	EV_MAP( KEY_C ),
#endif
#ifdef KEY_V
	EV_MAP( KEY_V ),
#endif
#ifdef KEY_B
	EV_MAP( KEY_B ),
#endif
#ifdef KEY_N
	EV_MAP( KEY_N ),
#endif
#ifdef KEY_M
	EV_MAP( KEY_M ),
#endif
#ifdef KEY_COMMA
	EV_MAP( KEY_COMMA ),
#endif
#ifdef KEY_DOT
	EV_MAP( KEY_DOT ),
#endif
#ifdef KEY_SLASH
	EV_MAP( KEY_SLASH ),
#endif
#ifdef KEY_RIGHTSHIFT
	EV_MAP( KEY_RIGHTSHIFT ),
#endif
#ifdef KEY_KPASTERISK
	EV_MAP( KEY_KPASTERISK ),
#endif
#ifdef KEY_LEFTALT
	EV_MAP( KEY_LEFTALT ),
#endif
#ifdef KEY_SPACE
	EV_MAP( KEY_SPACE ),
#endif
#ifdef KEY_CAPSLOCK
	EV_MAP( KEY_CAPSLOCK ),
#endif
#ifdef KEY_F1
	EV_MAP( KEY_F1 ),
#endif
#ifdef KEY_F2
	EV_MAP( KEY_F2 ),
#endif
#ifdef KEY_F3
	EV_MAP( KEY_F3 ),
#endif
#ifdef KEY_F4
	EV_MAP( KEY_F4 ),
#endif
#ifdef KEY_F5
	EV_MAP( KEY_F5 ),
#endif
#ifdef KEY_F6
	EV_MAP( KEY_F6 ),
#endif
#ifdef KEY_F7
	EV_MAP( KEY_F7 ),
#endif
#ifdef KEY_F8
	EV_MAP( KEY_F8 ),
#endif
#ifdef KEY_F9
	EV_MAP( KEY_F9 ),
#endif
#ifdef KEY_F10
	EV_MAP( KEY_F10 ),
#endif
#ifdef KEY_NUMLOCK
	EV_MAP( KEY_NUMLOCK ),
#endif
#ifdef KEY_SCROLLLOCK
	EV_MAP( KEY_SCROLLLOCK ),
#endif
#ifdef KEY_KP7
	EV_MAP( KEY_KP7 ),
#endif
#ifdef KEY_KP8
	EV_MAP( KEY_KP8 ),
#endif
#ifdef KEY_KP9
	EV_MAP( KEY_KP9 ),
#endif
#ifdef KEY_KPMINUS
	EV_MAP( KEY_KPMINUS ),
#endif
#ifdef KEY_KP4
	EV_MAP( KEY_KP4 ),
#endif
#ifdef KEY_KP5
	EV_MAP( KEY_KP5 ),
#endif
#ifdef KEY_KP6
	EV_MAP( KEY_KP6 ),
#endif
#ifdef KEY_KPPLUS
	EV_MAP( KEY_KPPLUS ),
#endif
#ifdef KEY_KP1
	EV_MAP( KEY_KP1 ),
#endif
#ifdef KEY_KP2
	EV_MAP( KEY_KP2 ),
#endif
#ifdef KEY_KP3
	EV_MAP( KEY_KP3 ),
#endif
#ifdef KEY_KP0
	EV_MAP( KEY_KP0 ),
#endif
#ifdef KEY_KPDOT
	EV_MAP( KEY_KPDOT ),
#endif
#ifdef KEY_ZENKAKUHANKAKU
	EV_MAP( KEY_ZENKAKUHANKAKU ),
#endif
#ifdef KEY_102ND
	EV_MAP( KEY_102ND ),
#endif
#ifdef KEY_F11
	EV_MAP( KEY_F11 ),
#endif
#ifdef KEY_F12
	EV_MAP( KEY_F12 ),
#endif
#ifdef KEY_RO
	EV_MAP( KEY_RO ),
#endif
#ifdef KEY_KATAKANA
	EV_MAP( KEY_KATAKANA ),
#endif
#ifdef KEY_HIRAGANA
	EV_MAP( KEY_HIRAGANA ),
#endif
#ifdef KEY_HENKAN
	EV_MAP( KEY_HENKAN ),
#endif
#ifdef KEY_KATAKANAHIRAGANA
	EV_MAP( KEY_KATAKANAHIRAGANA ),
#endif
#ifdef KEY_MUHENKAN
	EV_MAP( KEY_MUHENKAN ),
#endif
#ifdef KEY_KPJPCOMMA
	EV_MAP( KEY_KPJPCOMMA ),
#endif
#ifdef KEY_KPENTER
	EV_MAP( KEY_KPENTER ),
#endif
#ifdef KEY_RIGHTCTRL
	EV_MAP( KEY_RIGHTCTRL ),
#endif
#ifdef KEY_KPSLASH
	EV_MAP( KEY_KPSLASH ),
#endif
#ifdef KEY_SYSRQ
	EV_MAP( KEY_SYSRQ ),
#endif
#ifdef KEY_RIGHTALT
	EV_MAP( KEY_RIGHTALT ),
#endif
#ifdef KEY_LINEFEED
	EV_MAP( KEY_LINEFEED ),
#endif
#ifdef KEY_HOME
	EV_MAP( KEY_HOME ),
#endif
#ifdef KEY_UP
	EV_MAP( KEY_UP ),
#endif
#ifdef KEY_PAGEUP
	EV_MAP( KEY_PAGEUP ),
#endif
#ifdef KEY_LEFT
	EV_MAP( KEY_LEFT ),
#endif
#ifdef KEY_RIGHT
	EV_MAP( KEY_RIGHT ),
#endif
#ifdef KEY_END
	EV_MAP( KEY_END ),
#endif
#ifdef KEY_DOWN
	EV_MAP( KEY_DOWN ),
#endif
#ifdef KEY_PAGEDOWN
	EV_MAP( KEY_PAGEDOWN ),
#endif
#ifdef KEY_INSERT
	EV_MAP( KEY_INSERT ),
#endif
#ifdef KEY_DELETE
	EV_MAP( KEY_DELETE ),
#endif
#ifdef KEY_MACRO
	EV_MAP( KEY_MACRO ),
#endif
#ifdef KEY_MUTE
	EV_MAP( KEY_MUTE ),
#endif
#ifdef KEY_VOLUMEDOWN
	EV_MAP( KEY_VOLUMEDOWN ),
#endif
#ifdef KEY_VOLUMEUP
	EV_MAP( KEY_VOLUMEUP ),
#endif
#ifdef KEY_POWER
	EV_MAP( KEY_POWER ),
#endif
#ifdef KEY_KPEQUAL
	EV_MAP( KEY_KPEQUAL ),
#endif
#ifdef KEY_KPPLUSMINUS
	EV_MAP( KEY_KPPLUSMINUS ),
#endif
#ifdef KEY_PAUSE
	EV_MAP( KEY_PAUSE ),
#endif
#ifdef KEY_SCALE
	EV_MAP( KEY_SCALE ),
#endif
#ifdef KEY_KPCOMMA
	EV_MAP( KEY_KPCOMMA ),
#endif
#ifdef KEY_HANGEUL
	EV_MAP( KEY_HANGEUL ),
#endif
#ifdef KEY_HANGUEL
	EV_MAP( KEY_HANGUEL ),
#endif
#ifdef KEY_HANJA
	EV_MAP( KEY_HANJA ),
#endif
#ifdef KEY_YEN
	EV_MAP( KEY_YEN ),
#endif
#ifdef KEY_LEFTMETA
	EV_MAP( KEY_LEFTMETA ),
#endif
#ifdef KEY_RIGHTMETA
	EV_MAP( KEY_RIGHTMETA ),
#endif
#ifdef KEY_COMPOSE
	EV_MAP( KEY_COMPOSE ),
#endif
#ifdef KEY_STOP
	EV_MAP( KEY_STOP ),
#endif
#ifdef KEY_AGAIN
	EV_MAP( KEY_AGAIN ),
#endif
#ifdef KEY_PROPS
	EV_MAP( KEY_PROPS ),
#endif
#ifdef KEY_UNDO
	EV_MAP( KEY_UNDO ),
#endif
#ifdef KEY_FRONT
	EV_MAP( KEY_FRONT ),
#endif
#ifdef KEY_COPY
	EV_MAP( KEY_COPY ),
#endif
#ifdef KEY_OPEN
	EV_MAP( KEY_OPEN ),
#endif
#ifdef KEY_PASTE
	EV_MAP( KEY_PASTE ),
#endif
#ifdef KEY_FIND
	EV_MAP( KEY_FIND ),
#endif
#ifdef KEY_CUT
	EV_MAP( KEY_CUT ),
#endif
#ifdef KEY_HELP
	EV_MAP( KEY_HELP ),
#endif
#ifdef KEY_MENU
	EV_MAP( KEY_MENU ),
#endif
#ifdef KEY_CALC
	EV_MAP( KEY_CALC ),
#endif
#ifdef KEY_SETUP
	EV_MAP( KEY_SETUP ),
#endif
#ifdef KEY_SLEEP
	EV_MAP( KEY_SLEEP ),
#endif
#ifdef KEY_WAKEUP
	EV_MAP( KEY_WAKEUP ),
#endif
#ifdef KEY_FILE
	EV_MAP( KEY_FILE ),
#endif
#ifdef KEY_SENDFILE
	EV_MAP( KEY_SENDFILE ),
#endif
#ifdef KEY_DELETEFILE
	EV_MAP( KEY_DELETEFILE ),
#endif
#ifdef KEY_XFER
	EV_MAP( KEY_XFER ),
#endif
#ifdef KEY_PROG1
	EV_MAP( KEY_PROG1 ),
#endif
#ifdef KEY_PROG2
	EV_MAP( KEY_PROG2 ),
#endif
#ifdef KEY_WWW
	EV_MAP( KEY_WWW ),
#endif
#ifdef KEY_MSDOS
	EV_MAP( KEY_MSDOS ),
#endif
#ifdef KEY_COFFEE
	EV_MAP( KEY_COFFEE ),
#endif
#ifdef KEY_SCREENLOCK
	EV_MAP( KEY_SCREENLOCK ),
#endif
#ifdef KEY_DIRECTION
	EV_MAP( KEY_DIRECTION ),
#endif
#ifdef KEY_CYCLEWINDOWS
	EV_MAP( KEY_CYCLEWINDOWS ),
#endif
#ifdef KEY_MAIL
	EV_MAP( KEY_MAIL ),
#endif
#ifdef KEY_BOOKMARKS
	EV_MAP( KEY_BOOKMARKS ),
#endif
#ifdef KEY_COMPUTER
	EV_MAP( KEY_COMPUTER ),
#endif
#ifdef KEY_BACK
	EV_MAP( KEY_BACK ),
#endif
#ifdef KEY_FORWARD
	EV_MAP( KEY_FORWARD ),
#endif
#ifdef KEY_CLOSECD
	EV_MAP( KEY_CLOSECD ),
#endif
#ifdef KEY_EJECTCD
	EV_MAP( KEY_EJECTCD ),
#endif
#ifdef KEY_EJECTCLOSECD
	EV_MAP( KEY_EJECTCLOSECD ),
#endif
#ifdef KEY_NEXTSONG
	EV_MAP( KEY_NEXTSONG ),
#endif
#ifdef KEY_PLAYPAUSE
	EV_MAP( KEY_PLAYPAUSE ),
#endif
#ifdef KEY_PREVIOUSSONG
	EV_MAP( KEY_PREVIOUSSONG ),
#endif
#ifdef KEY_STOPCD
	EV_MAP( KEY_STOPCD ),
#endif
#ifdef KEY_RECORD
	EV_MAP( KEY_RECORD ),
#endif
#ifdef KEY_REWIND
	EV_MAP( KEY_REWIND ),
#endif
#ifdef KEY_PHONE
	EV_MAP( KEY_PHONE ),
#endif
#ifdef KEY_ISO
	EV_MAP( KEY_ISO ),
#endif
#ifdef KEY_CONFIG
	EV_MAP( KEY_CONFIG ),
#endif
#ifdef KEY_HOMEPAGE
	EV_MAP( KEY_HOMEPAGE ),
#endif
#ifdef KEY_REFRESH
	EV_MAP( KEY_REFRESH ),
#endif
#ifdef KEY_EXIT
	EV_MAP( KEY_EXIT ),
#endif
#ifdef KEY_MOVE
	EV_MAP( KEY_MOVE ),
#endif
#ifdef KEY_EDIT
	EV_MAP( KEY_EDIT ),
#endif
#ifdef KEY_SCROLLUP
	EV_MAP( KEY_SCROLLUP ),
#endif
#ifdef KEY_SCROLLDOWN
	EV_MAP( KEY_SCROLLDOWN ),
#endif
#ifdef KEY_KPLEFTPAREN
	EV_MAP( KEY_KPLEFTPAREN ),
#endif
#ifdef KEY_KPRIGHTPAREN
	EV_MAP( KEY_KPRIGHTPAREN ),
#endif
#ifdef KEY_NEW
	EV_MAP( KEY_NEW ),
#endif
#ifdef KEY_REDO
	EV_MAP( KEY_REDO ),
#endif
#ifdef KEY_F13
	EV_MAP( KEY_F13 ),
#endif
#ifdef KEY_F14
	EV_MAP( KEY_F14 ),
#endif
#ifdef KEY_F15
	EV_MAP( KEY_F15 ),
#endif
#ifdef KEY_F16
	EV_MAP( KEY_F16 ),
#endif
#ifdef KEY_F17
	EV_MAP( KEY_F17 ),
#endif
#ifdef KEY_F18
	EV_MAP( KEY_F18 ),
#endif
#ifdef KEY_F19
	EV_MAP( KEY_F19 ),
#endif
#ifdef KEY_F20
	EV_MAP( KEY_F20 ),
#endif
#ifdef KEY_F21
	EV_MAP( KEY_F21 ),
#endif
#ifdef KEY_F22
	EV_MAP( KEY_F22 ),
#endif
#ifdef KEY_F23
	EV_MAP( KEY_F23 ),
#endif
#ifdef KEY_F24
	EV_MAP( KEY_F24 ),
#endif
#ifdef KEY_PLAYCD
	EV_MAP( KEY_PLAYCD ),
#endif
#ifdef KEY_PAUSECD
	EV_MAP( KEY_PAUSECD ),
#endif
#ifdef KEY_PROG3
	EV_MAP( KEY_PROG3 ),
#endif
#ifdef KEY_PROG4
	EV_MAP( KEY_PROG4 ),
#endif
#ifdef KEY_DASHBOARD
	EV_MAP( KEY_DASHBOARD ),
#endif
#ifdef KEY_SUSPEND
	EV_MAP( KEY_SUSPEND ),
#endif
#ifdef KEY_CLOSE
	EV_MAP( KEY_CLOSE ),
#endif
#ifdef KEY_PLAY
	EV_MAP( KEY_PLAY ),
#endif
#ifdef KEY_FASTFORWARD
	EV_MAP( KEY_FASTFORWARD ),
#endif
#ifdef KEY_BASSBOOST
	EV_MAP( KEY_BASSBOOST ),
#endif
#ifdef KEY_PRINT
	EV_MAP( KEY_PRINT ),
#endif
#ifdef KEY_HP
	EV_MAP( KEY_HP ),
#endif
#ifdef KEY_CAMERA
	EV_MAP( KEY_CAMERA ),
#endif
#ifdef KEY_SOUND
	EV_MAP( KEY_SOUND ),
#endif
#ifdef KEY_QUESTION
	EV_MAP( KEY_QUESTION ),
#endif
#ifdef KEY_EMAIL
	EV_MAP( KEY_EMAIL ),
#endif
#ifdef KEY_CHAT
	EV_MAP( KEY_CHAT ),
#endif
#ifdef KEY_SEARCH
	EV_MAP( KEY_SEARCH ),
#endif
#ifdef KEY_CONNECT
	EV_MAP( KEY_CONNECT ),
#endif
#ifdef KEY_FINANCE
	EV_MAP( KEY_FINANCE ),
#endif
#ifdef KEY_SPORT
	EV_MAP( KEY_SPORT ),
#endif
#ifdef KEY_SHOP
	EV_MAP( KEY_SHOP ),
#endif
#ifdef KEY_ALTERASE
	EV_MAP( KEY_ALTERASE ),
#endif
#ifdef KEY_CANCEL
	EV_MAP( KEY_CANCEL ),
#endif
#ifdef KEY_BRIGHTNESSDOWN
	EV_MAP( KEY_BRIGHTNESSDOWN ),
#endif
#ifdef KEY_BRIGHTNESSUP
	EV_MAP( KEY_BRIGHTNESSUP ),
#endif
#ifdef KEY_MEDIA
	EV_MAP( KEY_MEDIA ),
#endif
#ifdef KEY_SWITCHVIDEOMODE
	EV_MAP( KEY_SWITCHVIDEOMODE ),
#endif
#ifdef KEY_KBDILLUMTOGGLE
	EV_MAP( KEY_KBDILLUMTOGGLE ),
#endif
#ifdef KEY_KBDILLUMDOWN
	EV_MAP( KEY_KBDILLUMDOWN ),
#endif
#ifdef KEY_KBDILLUMUP
	EV_MAP( KEY_KBDILLUMUP ),
#endif
#ifdef KEY_SEND
	EV_MAP( KEY_SEND ),
#endif
#ifdef KEY_REPLY
	EV_MAP( KEY_REPLY ),
#endif
#ifdef KEY_FORWARDMAIL
	EV_MAP( KEY_FORWARDMAIL ),
#endif
#ifdef KEY_SAVE
	EV_MAP( KEY_SAVE ),
#endif
#ifdef KEY_DOCUMENTS
	EV_MAP( KEY_DOCUMENTS ),
#endif
#ifdef KEY_BATTERY
	EV_MAP( KEY_BATTERY ),
#endif
#ifdef KEY_BLUETOOTH
	EV_MAP( KEY_BLUETOOTH ),
#endif
#ifdef KEY_WLAN
	EV_MAP( KEY_WLAN ),
#endif
#ifdef KEY_UWB
	EV_MAP( KEY_UWB ),
#endif
#ifdef KEY_UNKNOWN
	EV_MAP( KEY_UNKNOWN ),
#endif
#ifdef KEY_VIDEO_NEXT
	EV_MAP( KEY_VIDEO_NEXT ),
#endif
#ifdef KEY_VIDEO_PREV
	EV_MAP( KEY_VIDEO_PREV ),
#endif
#ifdef KEY_BRIGHTNESS_CYCLE
	EV_MAP( KEY_BRIGHTNESS_CYCLE ),
#endif
#ifdef KEY_BRIGHTNESS_ZERO
	EV_MAP( KEY_BRIGHTNESS_ZERO ),
#endif
#ifdef KEY_DISPLAY_OFF
	EV_MAP( KEY_DISPLAY_OFF ),
#endif
#ifdef KEY_WIMAX
	EV_MAP( KEY_WIMAX ),
#endif
#ifdef KEY_OK
	EV_MAP( KEY_OK ),
#endif
#ifdef KEY_SELECT
	EV_MAP( KEY_SELECT ),
#endif
#ifdef KEY_GOTO
	EV_MAP( KEY_GOTO ),
#endif
#ifdef KEY_CLEAR
	EV_MAP( KEY_CLEAR ),
#endif
#ifdef KEY_POWER2
	EV_MAP( KEY_POWER2 ),
#endif
#ifdef KEY_OPTION
	EV_MAP( KEY_OPTION ),
#endif
#ifdef KEY_INFO
	EV_MAP( KEY_INFO ),
#endif
#ifdef KEY_TIME
	EV_MAP( KEY_TIME ),
#endif
#ifdef KEY_VENDOR
	EV_MAP( KEY_VENDOR ),
#endif
#ifdef KEY_ARCHIVE
	EV_MAP( KEY_ARCHIVE ),
#endif
#ifdef KEY_PROGRAM
	EV_MAP( KEY_PROGRAM ),
#endif
#ifdef KEY_CHANNEL
	EV_MAP( KEY_CHANNEL ),
#endif
#ifdef KEY_FAVORITES
	EV_MAP( KEY_FAVORITES ),
#endif
#ifdef KEY_EPG
	EV_MAP( KEY_EPG ),
#endif
#ifdef KEY_PVR
	EV_MAP( KEY_PVR ),
#endif
#ifdef KEY_MHP
	EV_MAP( KEY_MHP ),
#endif
#ifdef KEY_LANGUAGE
	EV_MAP( KEY_LANGUAGE ),
#endif
#ifdef KEY_TITLE
	EV_MAP( KEY_TITLE ),
#endif
#ifdef KEY_SUBTITLE
	EV_MAP( KEY_SUBTITLE ),
#endif
#ifdef KEY_ANGLE
	EV_MAP( KEY_ANGLE ),
#endif
#ifdef KEY_ZOOM
	EV_MAP( KEY_ZOOM ),
#endif
#ifdef KEY_MODE
	EV_MAP( KEY_MODE ),
#endif
#ifdef KEY_KEYBOARD
	EV_MAP( KEY_KEYBOARD ),
#endif
#ifdef KEY_SCREEN
	EV_MAP( KEY_SCREEN ),
#endif
#ifdef KEY_PC
	EV_MAP( KEY_PC ),
#endif
#ifdef KEY_TV
	EV_MAP( KEY_TV ),
#endif
#ifdef KEY_TV2
	EV_MAP( KEY_TV2 ),
#endif
#ifdef KEY_VCR
	EV_MAP( KEY_VCR ),
#endif
#ifdef KEY_VCR2
	EV_MAP( KEY_VCR2 ),
#endif
#ifdef KEY_SAT
	EV_MAP( KEY_SAT ),
#endif
#ifdef KEY_SAT2
	EV_MAP( KEY_SAT2 ),
#endif
#ifdef KEY_CD
	EV_MAP( KEY_CD ),
#endif
#ifdef KEY_TAPE
	EV_MAP( KEY_TAPE ),
#endif
#ifdef KEY_RADIO
	EV_MAP( KEY_RADIO ),
#endif
#ifdef KEY_TUNER
	EV_MAP( KEY_TUNER ),
#endif
#ifdef KEY_PLAYER
	EV_MAP( KEY_PLAYER ),
#endif
#ifdef KEY_TEXT
	EV_MAP( KEY_TEXT ),
#endif
#ifdef KEY_DVD
	EV_MAP( KEY_DVD ),
#endif
#ifdef KEY_AUX
	EV_MAP( KEY_AUX ),
#endif
#ifdef KEY_MP3
	EV_MAP( KEY_MP3 ),
#endif
#ifdef KEY_AUDIO
	EV_MAP( KEY_AUDIO ),
#endif
#ifdef KEY_VIDEO
	EV_MAP( KEY_VIDEO ),
#endif
#ifdef KEY_DIRECTORY
	EV_MAP( KEY_DIRECTORY ),
#endif
#ifdef KEY_LIST
	EV_MAP( KEY_LIST ),
#endif
#ifdef KEY_MEMO
	EV_MAP( KEY_MEMO ),
#endif
#ifdef KEY_CALENDAR
	EV_MAP( KEY_CALENDAR ),
#endif
#ifdef KEY_RED
	EV_MAP( KEY_RED ),
#endif
#ifdef KEY_GREEN
	EV_MAP( KEY_GREEN ),
#endif
#ifdef KEY_YELLOW
	EV_MAP( KEY_YELLOW ),
#endif
#ifdef KEY_BLUE
	EV_MAP( KEY_BLUE ),
#endif
#ifdef KEY_CHANNELUP
	EV_MAP( KEY_CHANNELUP ),
#endif
#ifdef KEY_CHANNELDOWN
	EV_MAP( KEY_CHANNELDOWN ),
#endif
#ifdef KEY_FIRST
	EV_MAP( KEY_FIRST ),
#endif
#ifdef KEY_LAST
	EV_MAP( KEY_LAST ),
#endif
#ifdef KEY_AB
	EV_MAP( KEY_AB ),
#endif
#ifdef KEY_NEXT
	EV_MAP( KEY_NEXT ),
#endif
#ifdef KEY_RESTART
	EV_MAP( KEY_RESTART ),
#endif
#ifdef KEY_SLOW
	EV_MAP( KEY_SLOW ),
#endif
#ifdef KEY_SHUFFLE
	EV_MAP( KEY_SHUFFLE ),
#endif
#ifdef KEY_BREAK
	EV_MAP( KEY_BREAK ),
#endif
#ifdef KEY_PREVIOUS
	EV_MAP( KEY_PREVIOUS ),
#endif
#ifdef KEY_DIGITS
	EV_MAP( KEY_DIGITS ),
#endif
#ifdef KEY_TEEN
	EV_MAP( KEY_TEEN ),
#endif
#ifdef KEY_TWEN
	EV_MAP( KEY_TWEN ),
#endif
#ifdef KEY_VIDEOPHONE
	EV_MAP( KEY_VIDEOPHONE ),
#endif
#ifdef KEY_GAMES
	EV_MAP( KEY_GAMES ),
#endif
#ifdef KEY_ZOOMIN
	EV_MAP( KEY_ZOOMIN ),
#endif
#ifdef KEY_ZOOMOUT
	EV_MAP( KEY_ZOOMOUT ),
#endif
#ifdef KEY_ZOOMRESET
	EV_MAP( KEY_ZOOMRESET ),
#endif
#ifdef KEY_WORDPROCESSOR
	EV_MAP( KEY_WORDPROCESSOR ),
#endif
#ifdef KEY_EDITOR
	EV_MAP( KEY_EDITOR ),
#endif
#ifdef KEY_SPREADSHEET
	EV_MAP( KEY_SPREADSHEET ),
#endif
#ifdef KEY_GRAPHICSEDITOR
	EV_MAP( KEY_GRAPHICSEDITOR ),
#endif
#ifdef KEY_PRESENTATION
	EV_MAP( KEY_PRESENTATION ),
#endif
#ifdef KEY_DATABASE
	EV_MAP( KEY_DATABASE ),
#endif
#ifdef KEY_NEWS
	EV_MAP( KEY_NEWS ),
#endif
#ifdef KEY_VOICEMAIL
	EV_MAP( KEY_VOICEMAIL ),
#endif
#ifdef KEY_ADDRESSBOOK
	EV_MAP( KEY_ADDRESSBOOK ),
#endif
#ifdef KEY_MESSENGER
	EV_MAP( KEY_MESSENGER ),
#endif
#ifdef KEY_DISPLAYTOGGLE
	EV_MAP( KEY_DISPLAYTOGGLE ),
#endif
#ifdef KEY_SPELLCHECK
	EV_MAP( KEY_SPELLCHECK ),
#endif
#ifdef KEY_LOGOFF
	EV_MAP( KEY_LOGOFF ),
#endif
#ifdef KEY_DOLLAR
	EV_MAP( KEY_DOLLAR ),
#endif
#ifdef KEY_EURO
	EV_MAP( KEY_EURO ),
#endif
#ifdef KEY_FRAMEBACK
	EV_MAP( KEY_FRAMEBACK ),
#endif
#ifdef KEY_FRAMEFORWARD
	EV_MAP( KEY_FRAMEFORWARD ),
#endif
#ifdef KEY_CONTEXT_MENU
	EV_MAP( KEY_CONTEXT_MENU ),
#endif
#ifdef KEY_MEDIA_REPEAT
	EV_MAP( KEY_MEDIA_REPEAT ),
#endif
#ifdef KEY_DEL_EOL
	EV_MAP( KEY_DEL_EOL ),
#endif
#ifdef KEY_DEL_EOS
	EV_MAP( KEY_DEL_EOS ),
#endif
#ifdef KEY_INS_LINE
	EV_MAP( KEY_INS_LINE ),
#endif
#ifdef KEY_DEL_LINE
	EV_MAP( KEY_DEL_LINE ),
#endif
#ifdef KEY_FN
	EV_MAP( KEY_FN ),
#endif
#ifdef KEY_FN_ESC
	EV_MAP( KEY_FN_ESC ),
#endif
#ifdef KEY_FN_F1
	EV_MAP( KEY_FN_F1 ),
#endif
#ifdef KEY_FN_F2
	EV_MAP( KEY_FN_F2 ),
#endif
#ifdef KEY_FN_F3
	EV_MAP( KEY_FN_F3 ),
#endif
#ifdef KEY_FN_F4
	EV_MAP( KEY_FN_F4 ),
#endif
#ifdef KEY_FN_F5
	EV_MAP( KEY_FN_F5 ),
#endif
#ifdef KEY_FN_F6
	EV_MAP( KEY_FN_F6 ),
#endif
#ifdef KEY_FN_F7
	EV_MAP( KEY_FN_F7 ),
#endif
#ifdef KEY_FN_F8
	EV_MAP( KEY_FN_F8 ),
#endif
#ifdef KEY_FN_F9
	EV_MAP( KEY_FN_F9 ),
#endif
#ifdef KEY_FN_F10
	EV_MAP( KEY_FN_F10 ),
#endif
#ifdef KEY_FN_F11
	EV_MAP( KEY_FN_F11 ),
#endif
#ifdef KEY_FN_F12
	EV_MAP( KEY_FN_F12 ),
#endif
#ifdef KEY_FN_1
	EV_MAP( KEY_FN_1 ),
#endif
#ifdef KEY_FN_2
	EV_MAP( KEY_FN_2 ),
#endif
#ifdef KEY_FN_D
	EV_MAP( KEY_FN_D ),
#endif
#ifdef KEY_FN_E
	EV_MAP( KEY_FN_E ),
#endif
#ifdef KEY_FN_F
	EV_MAP( KEY_FN_F ),
#endif
#ifdef KEY_FN_S
	EV_MAP( KEY_FN_S ),
#endif
#ifdef KEY_FN_B
	EV_MAP( KEY_FN_B ),
#endif
#ifdef KEY_BRL_DOT1
	EV_MAP( KEY_BRL_DOT1 ),
#endif
#ifdef KEY_BRL_DOT2
	EV_MAP( KEY_BRL_DOT2 ),
#endif
#ifdef KEY_BRL_DOT3
	EV_MAP( KEY_BRL_DOT3 ),
#endif
#ifdef KEY_BRL_DOT4
	EV_MAP( KEY_BRL_DOT4 ),
#endif
#ifdef KEY_BRL_DOT5
	EV_MAP( KEY_BRL_DOT5 ),
#endif
#ifdef KEY_BRL_DOT6
	EV_MAP( KEY_BRL_DOT6 ),
#endif
#ifdef KEY_BRL_DOT7
	EV_MAP( KEY_BRL_DOT7 ),
#endif
#ifdef KEY_BRL_DOT8
	EV_MAP( KEY_BRL_DOT8 ),
#endif
#ifdef KEY_BRL_DOT9
	EV_MAP( KEY_BRL_DOT9 ),
#endif
#ifdef KEY_BRL_DOT10
	EV_MAP( KEY_BRL_DOT10 ),
#endif
#ifdef KEY_NUMERIC_0
	EV_MAP( KEY_NUMERIC_0 ),
#endif
#ifdef KEY_NUMERIC_1
	EV_MAP( KEY_NUMERIC_1 ),
#endif
#ifdef KEY_NUMERIC_2
	EV_MAP( KEY_NUMERIC_2 ),
#endif
#ifdef KEY_NUMERIC_3
	EV_MAP( KEY_NUMERIC_3 ),
#endif
#ifdef KEY_NUMERIC_4
	EV_MAP( KEY_NUMERIC_4 ),
#endif
#ifdef KEY_NUMERIC_5
	EV_MAP( KEY_NUMERIC_5 ),
#endif
#ifdef KEY_NUMERIC_6
	EV_MAP( KEY_NUMERIC_6 ),
#endif
#ifdef KEY_NUMERIC_7
	EV_MAP( KEY_NUMERIC_7 ),
#endif
#ifdef KEY_NUMERIC_8
	EV_MAP( KEY_NUMERIC_8 ),
#endif
#ifdef KEY_NUMERIC_9
	EV_MAP( KEY_NUMERIC_9 ),
#endif
#ifdef KEY_NUMERIC_STAR
	EV_MAP( KEY_NUMERIC_STAR ),
#endif
#ifdef KEY_NUMERIC_POUND
	EV_MAP( KEY_NUMERIC_POUND ),
#endif
#ifdef KEY_MIN_INTERESTING
	EV_MAP( KEY_MIN_INTERESTING ),
#endif
#ifdef BTN_MISC
	EV_MAP( BTN_MISC ),
#endif
#ifdef BTN_0
	EV_MAP( BTN_0 ),
#endif
#ifdef BTN_1
	EV_MAP( BTN_1 ),
#endif
#ifdef BTN_2
	EV_MAP( BTN_2 ),
#endif
#ifdef BTN_3
	EV_MAP( BTN_3 ),
#endif
#ifdef BTN_4
	EV_MAP( BTN_4 ),
#endif
#ifdef BTN_5
	EV_MAP( BTN_5 ),
#endif
#ifdef BTN_6
	EV_MAP( BTN_6 ),
#endif
#ifdef BTN_7
	EV_MAP( BTN_7 ),
#endif
#ifdef BTN_8
	EV_MAP( BTN_8 ),
#endif
#ifdef BTN_9
	EV_MAP( BTN_9 ),
#endif
#ifdef BTN_MOUSE
	EV_MAP( BTN_MOUSE ),
#endif
#ifdef BTN_LEFT
	EV_MAP( BTN_LEFT ),
#endif
#ifdef BTN_RIGHT
	EV_MAP( BTN_RIGHT ),
#endif
#ifdef BTN_MIDDLE
	EV_MAP( BTN_MIDDLE ),
#endif
#ifdef BTN_SIDE
	EV_MAP( BTN_SIDE ),
#endif
#ifdef BTN_EXTRA
	EV_MAP( BTN_EXTRA ),
#endif
#ifdef BTN_FORWARD
	EV_MAP( BTN_FORWARD ),
#endif
#ifdef BTN_BACK
	EV_MAP( BTN_BACK ),
#endif
#ifdef BTN_TASK
	EV_MAP( BTN_TASK ),
#endif
#ifdef BTN_JOYSTICK
	EV_MAP( BTN_JOYSTICK ),
#endif
#ifdef BTN_TRIGGER
	EV_MAP( BTN_TRIGGER ),
#endif
#ifdef BTN_THUMB
	EV_MAP( BTN_THUMB ),
#endif
#ifdef BTN_THUMB2
	EV_MAP( BTN_THUMB2 ),
#endif
#ifdef BTN_TOP
	EV_MAP( BTN_TOP ),
#endif
#ifdef BTN_TOP2
	EV_MAP( BTN_TOP2 ),
#endif
#ifdef BTN_PINKIE
	EV_MAP( BTN_PINKIE ),
#endif
#ifdef BTN_BASE
	EV_MAP( BTN_BASE ),
#endif
#ifdef BTN_BASE2
	EV_MAP( BTN_BASE2 ),
#endif
#ifdef BTN_BASE3
	EV_MAP( BTN_BASE3 ),
#endif
#ifdef BTN_BASE4
	EV_MAP( BTN_BASE4 ),
#endif
#ifdef BTN_BASE5
	EV_MAP( BTN_BASE5 ),
#endif
#ifdef BTN_BASE6
	EV_MAP( BTN_BASE6 ),
#endif
#ifdef BTN_DEAD
	EV_MAP( BTN_DEAD ),
#endif
#ifdef BTN_GAMEPAD
	EV_MAP( BTN_GAMEPAD ),
#endif
#ifdef BTN_A
	EV_MAP( BTN_A ),
#endif
#ifdef BTN_B
	EV_MAP( BTN_B ),
#endif
#ifdef BTN_C
	EV_MAP( BTN_C ),
#endif
#ifdef BTN_X
	EV_MAP( BTN_X ),
#endif
#ifdef BTN_Y
	EV_MAP( BTN_Y ),
#endif
#ifdef BTN_Z
	EV_MAP( BTN_Z ),
#endif
#ifdef BTN_TL
	EV_MAP( BTN_TL ),
#endif
#ifdef BTN_TR
	EV_MAP( BTN_TR ),
#endif
#ifdef BTN_TL2
	EV_MAP( BTN_TL2 ),
#endif
#ifdef BTN_TR2
	EV_MAP( BTN_TR2 ),
#endif
#ifdef BTN_SELECT
	EV_MAP( BTN_SELECT ),
#endif
#ifdef BTN_START
	EV_MAP( BTN_START ),
#endif
#ifdef BTN_MODE
	EV_MAP( BTN_MODE ),
#endif
#ifdef BTN_THUMBL
	EV_MAP( BTN_THUMBL ),
#endif
#ifdef BTN_THUMBR
	EV_MAP( BTN_THUMBR ),
#endif
#ifdef BTN_DIGI
	EV_MAP( BTN_DIGI ),
#endif
#ifdef BTN_TOOL_PEN
	EV_MAP( BTN_TOOL_PEN ),
#endif
#ifdef BTN_TOOL_RUBBER
	EV_MAP( BTN_TOOL_RUBBER ),
#endif
#ifdef BTN_TOOL_BRUSH
	EV_MAP( BTN_TOOL_BRUSH ),
#endif
#ifdef BTN_TOOL_PENCIL
	EV_MAP( BTN_TOOL_PENCIL ),
#endif
#ifdef BTN_TOOL_AIRBRUSH
	EV_MAP( BTN_TOOL_AIRBRUSH ),
#endif
#ifdef BTN_TOOL_FINGER
	EV_MAP( BTN_TOOL_FINGER ),
#endif
#ifdef BTN_TOOL_MOUSE
	EV_MAP( BTN_TOOL_MOUSE ),
#endif
#ifdef BTN_TOOL_LENS
	EV_MAP( BTN_TOOL_LENS ),
#endif
#ifdef BTN_TOUCH
	EV_MAP( BTN_TOUCH ),
#endif
#ifdef BTN_STYLUS
	EV_MAP( BTN_STYLUS ),
#endif
#ifdef BTN_STYLUS2
	EV_MAP( BTN_STYLUS2 ),
#endif
#ifdef BTN_TOOL_DOUBLETAP
	EV_MAP( BTN_TOOL_DOUBLETAP ),
#endif
#ifdef BTN_TOOL_TRIPLETAP
	EV_MAP( BTN_TOOL_TRIPLETAP ),
#endif
#ifdef BTN_TOOL_QUADTAP
	EV_MAP( BTN_TOOL_QUADTAP ),
#endif
#ifdef BTN_WHEEL
	EV_MAP( BTN_WHEEL ),
#endif
#ifdef BTN_GEAR_DOWN
	EV_MAP( BTN_GEAR_DOWN ),
#endif
#ifdef BTN_GEAR_UP
	EV_MAP( BTN_GEAR_UP ),
#endif
};

static char *SW_NAME[SW_MAX] = {
#ifdef SW_LID
	EV_MAP( SW_LID ),
#endif
#ifdef SW_TABLET_MODE
	EV_MAP( SW_TABLET_MODE ),
#endif
#ifdef SW_HEADPHONE_INSERT
	EV_MAP( SW_HEADPHONE_INSERT ),
#endif
#ifdef SW_RFKILL_ALL
	EV_MAP( SW_RFKILL_ALL ),
#endif
#ifdef SW_MICROPHONE_INSERT
	EV_MAP( SW_MICROPHONE_INSERT ),
#endif
#ifdef SW_DOCK
	EV_MAP( SW_DOCK ),
#endif
#ifdef SW_LINEOUT_INSERT
	EV_MAP( SW_LINEOUT_INSERT ),
#endif
#ifdef SW_JACK_PHYSICAL_INSERT
	EV_MAP( SW_JACK_PHYSICAL_INSERT ),
#endif
#ifdef SW_VIDEOOUT_INSERT
	EV_MAP( SW_VIDEOOUT_INSERT ),
#endif
};
