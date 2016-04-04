/*
DISPLAY MODES:
DM_320x240_VGA : 320x240 VGA 60Hz
DM_320x240_NTSC : 320x240 NTSC 60Hz
DM_640x480_VGA : 640x480 VGA 60Hz
DM_640x480_NTSC_IL : 640x480 NTSC Interlaced 60Hz
DM_800x608_VGA : 800x608 VGA 60Hz
DM_768x480_NTSC_IL : 768x480 NTSC Interlaced 60Hz
DM_320x240_VGA_MB : 320x240 VGA 60Hz, 4FBs
DM_320x240_NTSC_MB : 320x240 NTSC 60Hz, 4FBs
DM_640x480_VGA_MB : 640x480 VGA 60Hz, 4FBs
DM_640x480_NTSC_IL_MB : 640x480 NTSC IL 60Hz, 4FBs
DM_800x608_VGA_MB : 800x608 VGA 60Hz, 4FBs
DM_768x480_NTSC_IL_MB : 768x480 NTSC IL 60Hz, 4FBs

PIXEL MODES:
PM_RGB555 : 15-bit (xRRRRRGGGGGBBBBB)
PM_RGB565 : 16-bit (RRRRRGGGGGGBBBBB)
PM_RGB888 : 24-bit (RRRRRRRR GGGGGGGG BBBBBBBB)

SNIPPETS:
	dbglog(DBG_DEBUG, "\n");

*/

#include <system.h>

extern uint8 romdisk[];
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
KOS_INIT_ROMDISK(romdisk);

int bCount = 0;
int mountState = 0;
enum stateMachine stateM = stateMenu;

int main(int argc, char **argv) {

	initVideo(DM_640x480, PM_RGB565);
	initTXT(TEXT_ENC);
	initBG("/rd/bg.jpg");

	if(!initDD())
		dbglog(DBG_DEBUG, "* Failed to Initialize Disk Drive *\n");
	/*
		if(!initFS("/dd"))
			dbglog(DBG_DEBUG, "* Failed to Initialize FileSystem *\n");
	*/

	while (1) {

		updateCon();

		switch (stateM) {

			case statePaused:
				drawP();

			case stateMenu:
				drawM();

		}
	}
	return 0;
}

void updateCon() {

	maple_device_t *cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

	if(cont) {

		cont_state_t *state = (cont_state_t *)maple_dev_status(cont);

		if(!state)
			return;

		//DPAD_UP
		if(state->buttons & CONT_DPAD_UP) {

			dbglog(DBG_DEBUG, "\nDPAD Up Pressed\n");

			bCount += 1;

			if (bCount == 2) {
				//SECOND PRESS DEACTIVATE

				bCount = 0;

			} else if(1) {
				//FIRST  PRESS ACTIVATE

			}

			usleep(400000);
		}

		//DPAD_DOWN
		if(state->buttons & CONT_DPAD_DOWN) {

			dbglog(DBG_DEBUG, "\nDPAD Down Pressed\n");

		}

		//DPAD_LEFT
		if(state->buttons & CONT_DPAD_LEFT) {

			dbglog(DBG_DEBUG, "\nDPAD Left Pressed\n");

			bCount += 1;

			if (bCount == 2) {

				//SECOND PRESS DEACTIVATE

				bCount = 0;

			} else if(1) {

				//FIRST  PRESS ACTIVATE

			}

			usleep(400000);

		}

		//DPAD_RIGHT
		if(state->buttons & CONT_DPAD_RIGHT) {

			dbglog(DBG_DEBUG, "\nDPAD Right Pressed\n");

			bCount += 1;

			if (bCount == 2) {

				//SECOND PRESS DEACTIVATE

				bCount = 0;

			} else if(1) {

				//FIRST  PRESS ACTIVATE

			}

			usleep(400000);

		}

		//BUTTON_Y
		if(state->buttons & CONT_Y) {

			dbglog(DBG_DEBUG, "\nButton Y Pressed\n");

			bCount += 1;

			if (bCount == 2) {

				//SECOND PRESS DEACTIVATE

				bCount = 0;

			} else if(1) {

				//FIRST  PRESS ACTIVATE

			}

			usleep(400000);

		}

		//BUTTON_A
		if(state->buttons & CONT_A) {

			dbglog(DBG_DEBUG, "\nButton A Pressed\n");

			bCount += 1;

			if (bCount == 2) {

				//SECOND PRESS DEACTIVATE

				bCount = 0;

			} else if(1) {

				//FIRST  PRESS ACTIVATE
			}

			usleep(400000);

		}

		//BUTTON_X
		if(state->buttons & CONT_X) {

			dbglog(DBG_DEBUG, "\nButton X Pressed\n");

			if(mountState) {

				mountCTL("/dd", 0);
				mountState = 0;

			} else if (!mountState) {

				dbglog(DBG_DEBUG, "~ Mount EXT2 File System ~\n");

				mountCTL("/dd", 1);
				mountState = 1;

			}

			usleep(400000);

		}

		//BUTTON_B
		if(state->buttons & CONT_B) {

			dbglog(DBG_DEBUG, "\nButton B Pressed\n");

			if (stateM == statePaused) {

				dbglog(DBG_DEBUG, "\n*** RESUME RENDERING PVR***\n");
				stateM = stateMenu;

			} else if (stateM != statePaused) {

				dbglog(DBG_DEBUG, "\n*** PAUSE RENDERING PVR***\n");
				stateM = statePaused;

			}
			usleep(400000);
		}

		//BUTTON_START
		if(state->buttons & CONT_START) {

			dbglog(DBG_DEBUG, "\nStart Button Pressed\n\n");

			//shutDown("/dd");
			arch_exit();

		}

	}

}

void drawM() {

	pvr_wait_ready();

	pvr_scene_begin();

		pvr_list_begin(PVR_LIST_OP_POLY);
			drawBG();
		pvr_list_finish();

		pvr_list_begin(PVR_LIST_TR_POLY);
			printPVR(0, 0, "DreamVD Player | Ver.0.1 | Press START To Exit");
			if (mountState) {
				printPVR(12, 24, "Optical Drive Mounted");
			}
			if (!mountState) {
				printPVR(12, 24, "Optical Drive Unounted");
			}
		pvr_list_finish();

	pvr_scene_finish();

}

void drawP() {

	pvr_wait_ready();

	pvr_scene_begin();

		clearVideo(77, 154, 231);

		pvr_list_begin(PVR_LIST_OP_POLY);
		pvr_list_finish();

		pvr_list_begin(PVR_LIST_TR_POLY);
			printPVR(0, 0, "DreamVD Player | Ver.0.1 | Press START To Exit");
			printPVR((DISP_WIDTH / 2)-36, (DISP_HEIGHT / 2)-24, "Paused"); //36 = char count / 2 * 12
			printPVR(12, 24, "Press B To Resume");
		pvr_list_finish();

	pvr_scene_finish();

}

void drawBG() {

	pvr_poly_cxt_t cxt;
	pvr_poly_hdr_t hdr;
	pvr_vertex_t vert;

	pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_RGB565, 512, 512, back_tex, PVR_FILTER_BILINEAR);
	pvr_poly_compile(&hdr, &cxt);
	pvr_prim(&hdr, sizeof(hdr));

	vert.flags = PVR_CMD_VERTEX;
	vert.x = 1;
	vert.y = 1;
	vert.z = 1;
	vert.u = 0;
	vert.v = 0;
	vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	pvr_prim(&vert, sizeof(vert));

	vert.x = DISP_WIDTH;
	vert.y = 1;
	vert.z = 1;
	vert.u = 1;
	vert.v = 0;
	vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	pvr_prim(&vert, sizeof(vert));

	vert.x = 1;
	vert.y = DISP_HEIGHT;
	vert.z = 1;
	vert.u = 0;
	vert.v = 1;
	vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	pvr_prim(&vert, sizeof(vert));

	vert.flags = PVR_CMD_VERTEX_EOL;
	vert.x = DISP_WIDTH;
	vert.y = DISP_HEIGHT;
	vert.z = 1;
	vert.u = 1;
	vert.v = 1;
	vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	pvr_prim(&vert, sizeof(vert));

}

void shutDown(char *blkDD) {

	dbglog(DBG_DEBUG, "ShutDown Application\n");

	removeFS(blkDD);
	g1_ata_shutdown();

}

void initTXT(int n) {

	bfont_set_encoding(n);

	dbglog(DBG_DEBUG, "\nText Initialized\n");

}

void initBG(char *bg) {

	back_tex = (unsigned short *)malloc((512*512)*2);
	jpeg_to_texture(bg, back_tex, 512, 1);

	dbglog(DBG_DEBUG, "BackGround Initialized\n");

}

void initVideo(int disMode, int pixMode) {

	vid_init(disMode, pixMode);
	pvr_init_defaults();

	dbglog(DBG_DEBUG, "PowerVR Initialized\n");

}

void clearVideo(int r, int g, int b) {
	vid_clear(r, g, b);
	vid_flip(-1);
}

void printPVR(int x, int y, char *str) {
	int strt = 24;
	int chrt = 12;

	bfont_draw_str(vram_s+(strt+y)*DISP_WIDTH+(chrt+x), DISP_WIDTH, 0, str);
}

void mountCTL(char *blkDD, int state) {
	if(state) {

		initFS(blkDD);
		dbglog(DBG_DEBUG, "~ Mount State Was Set To True  ~\n");

	} else if(!state) {

		removeFS(blkDD);
		dbglog(DBG_DEBUG, "~ Mount State Was Set To False ~\n");

	}
}

int checkDD() {
	if(g1_ata_blockdev_for_partition(0, 0, &bd_pio, &pt)) {
		dbglog(DBG_DEBUG, "* Couldn't get PIO blockdev for partition *\n");
		return -1;
	}

	dbglog(DBG_DEBUG, "~ ATA Device Hardware Check Passed ~\n");

	return 1;
}

int initDD() {

	uint16_t *buf = 64 * 1024;

	if(g1_ata_init()) {
		dbglog(DBG_DEBUG, "* G1ATA Failed to Initialize *\n");
		return -1;
	}

	if(g1_ata_select_device(G1_ATA_SLAVE)) {
		dbglog(DBG_DEBUG, "* G1ATA Failed to set SLAVE *\n");
		return -1;
	}

	if(g1_ata_read_lba(0, 1024, buf)) {
		dbglog(DBG_DEBUG, "* G1ATA Failed to read LBA *\n");
		return -1;
	}

	dbglog(DBG_DEBUG, "*** PIO MODE ***\n");
	dbglog(DBG_DEBUG, "~ G1 ATA Device Initialized ~\n");

	return 1;
}

int initFS(char *blkDD) {

	if(!checkDD()) {
		dbglog(DBG_DEBUG, "* Failed to Check Disk Drive *\n");
		return -1;
	}

	if(fs_ext2_init()) {
		dbglog(DBG_DEBUG, "* Failed ext2 FS Init *\n");
		return -1;
	}

	if(fs_ext2_mount(blkDD, &bd_pio, MNT_MODE)) {
		dbglog(DBG_DEBUG, "* Failed Mount in PIO Mode *\n");
		return -1;
	}

	mountState = 1;
	dbglog(DBG_DEBUG, "~ Initialized & Mounted the filesystem ~\n");

	return 1;
}

int removeFS(char *blkDD) {

	if(fs_ext2_unmount(blkDD)) {
		dbglog(DBG_DEBUG, "* Failed to Unmount EXT2 File System *\n");
		return -1;
	}

	dbglog(DBG_DEBUG, "~ Unmount EXT2 File System ~\n");

	return 1;
}

int saveToVMU(char *src) {

	char dst[32];
	file_t file;
	int filesize = 0;
	unsigned long zipsize = 0;
	uint8 *data;
	uint8 *zipdata;
	vmu_pkg_t pkg;
	uint8 *pkg_out;
	int pkg_size;

	strcpy(dst, "/vmu/a1/");
	strcat(dst, src);

	file = fs_open(src, O_RDONLY);
	filesize = fs_total(file);
	data = (uint8*)malloc(filesize);
	fs_read(file, data, filesize);
	fs_close(file);

	zipsize = filesize * 2;
	zipdata = (uint8*)malloc(zipsize);

	compress(zipdata, &zipsize, data, filesize);

	strcpy(pkg.desc_short, "DreamVD\\DC");
	strcpy(pkg.desc_long, "Settings");
	strcpy(pkg.app_id, "DreamVD\\DC");
	pkg.icon_cnt = 1;
	pkg.icon_anim_speed = 0;
	//memcpy(&pkg.icon_pal[0], icon_data, 32);
	//pkg.icon_data = icon_data + 32;
	pkg.eyecatch_type = VMUPKG_EC_NONE;
	pkg.data_len = zipsize;
	pkg.data = zipdata;
	vmu_pkg_build(&pkg, &pkg_out, &pkg_size);

	fs_unlink(dst);
	file = fs_open(dst, O_WRONLY);
	fs_write(file, pkg_out, pkg_size);
	fs_close(file);

	free(pkg_out);
	free(data);
	free(zipdata);

	return 1;
}

int loadFromVMU(char *dst) {

	char src[32];
	int file;
	int filesize;
	unsigned long unzipsize;
	uint8* data;
	uint8* unzipdata;
	vmu_pkg_t pkg;

	strcpy(src, "/vmu/a1/");
	strcat(src, dst);

	file = fs_open(src, O_RDONLY);
	if(file == 0) return -1;
	filesize = fs_total(file);
	if(filesize <= 0) return -1;
	data = (uint8*)malloc(filesize);
	fs_read(file, data, filesize);
	vmu_pkg_parse(data, &pkg);
	fs_close(file);

	unzipdata = (uint8 *)malloc(65536);
	unzipsize = 65536;

	uncompress(unzipdata, &unzipsize, (uint8 *)pkg.data, pkg.data_len);
	fs_unlink(dst);
	file = fs_open(dst, O_WRONLY);
	fs_write(file, unzipdata, unzipsize);
	fs_close(file);

	free(data);
	free(unzipdata);

	return 1;
}
