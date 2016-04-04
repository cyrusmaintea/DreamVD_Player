#include <kos.h>							/*  KallistiOS  */

#include <errno.h>							/*  STD C  */
#include <stdio.h>							/*  STD C  */
#include <stdint.h>							/*  STD C  */
#include <string.h>							/*  STD C  */
#include <dirent.h>							/*  STD C  */
#include <assert.h>							/*  STD C  */

#include <arch/timer.h>						/*  Arch  */
#include <arch/types.h>						/*  Arch  */

#include <jpeg/jpeg.h>						/*  Ports  */
#include <zlib/zlib.h>						/*  Ports  */

#include <dc/g1ata.h>						/*  Disk Drive  */
#include <ext2/fs_ext2.h>					/*  Disk Drive  */

#include <dc/maple/keyboard.h>				/*  Maple  */
#include <dc/maple/mouse.h>					/*  Maple  */

#define TEXT_ENC							(BFONT_CODE_ISO8859_1 || 0)
#define MNT_MODE 							FS_EXT2_MOUNT_READWRITE
#define DISP_WIDTH						640
#define DISP_HEIGHT						480

enum stateMachine {
	statePaused,
	stateMenu
};

pvr_ptr_t back_tex;
kos_blockdev_t bd_pio;
uint8 pt;

int mountState;
struct dirent *entry;

void updateCon();
void drawM();
void drawP();
void drawBG();
void shutDown(char *blkDD);
void initTXT(int n);
void initBG(char *bg);
void initVideo(int disMode, int pixMode);
void clearVideo(int r, int g, int b);
void printPVR(int x, int y, char *str);

void mountCTL(char *blkDD, int state);
int checkDD();
int initDD();
int initFS(char *blkDD);
int removeFS(char *blkDD);

int saveToVMU(char *src);
int loadFromVMU(char *dst);
