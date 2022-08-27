#include <typedefs.h>

#include <lowlevel/vga.h>
#include <lowlevel/io.h>
#include <lowlevel/wait.h>
#include <lowlevel/power.h>

#include <system/keyboard.h>

#include <houric/houricio.h>
#include <houric/houristr.h>
#include <houric/hourilib.h>

#define newline() putchar('\n');
#define CurX() linechar

static UI8 cmd[255];
static UI8 chget;

static UI16 CurY() {	// TODO: does it work ?
	UI32 tmp=cursor;
	UI32 n=0;
	while ((tmp-LINE_SIZE)>=0) {
		n++;
		tmp-=LINE_SIZE;
	}
	return n;
}

UI0 SSOnce(U0);
UI0 SSPrompt(U0);
UI0 SSScroll(U0);
UI0 SSMenu(U0);
UI8* SSGetHost(U0);
UI8* SSGetString(U0);
UI32 SSCheckCommand(const UI8* cmd);

UI0 SSMenu(U0) {
	#define MENUENTRY(x) putstr_attr(x,RED,YELLOW)
	newline();
	MENUENTRY("C-m");
	putstr(": Prints this menu");
	newline();
}

UI0 SSOnce(U0) {
	putstr_attr("OK\n",GREEN,DEF_BG);
	putstr_attr("\n\nSSatori Hello World!\n",MAGENTA,YELLOW);
	putstr("Novice user press C-h for a list of built-in commands\n");
	putstr("Press C-m for a list of defined key shortcuts\n");
	putstr("M-F4 to exit\n");
}

UI0 SSPrompt(U0) {
	newline();
	putstr_attr("@localhost> ",MAGENTA,DEF_BG);
}

I32 SSControl(const keypacket* key) {
	if (isCtrl(key->mod)==1)
		if (keycodeToASCII(key->keycode)=='m')
			SSMenu();
	if (isMeta(key->mod)==1)
		if (key->keycode==KEY_F4)
			return -1;
}

UI32 ssatori_entry() {
	SSOnce();
	keypacket key;
	UI8 hexstr[10];
	SSPrompt();
	newline();
	while (SSControl(&key)!=-1) {
		getkey(&key);
		if (keypacketToASCII(&key)!=0x00)
			putchar_attr(keypacketToASCII(&key),RED,DEF_BG);
	}
	return 0xFF;
}
