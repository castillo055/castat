
static const char modules_dir[] = "/home/victor/.config/dwmstatus/";

static const char delim[] = "  ";
//static const char delim[] = "^f3^^r0,0,2,26^^f3^";
//static const char delim[] = "^f1^^c#4c2e04^^l0,1,7,27^^c#8c5e04^^l1,0,8,26^^c#fcae1e^^l2,0,9,26^^c#8c5e04^^l3,0,10,26^^f11^^d^";

static const Module modules[] = {
	/* path, 	period */
	{"/home/victor/.config/dwmstatus/00_server-list", 	5.0},
	{"/home/victor/.config/dwmstatus/55_prgs", 		0.1},
	{"/home/victor/.config/dwmstatus/01_padd-status", 	3.0},
	{"/home/victor/.config/dwmstatus/09_mpd-status", 	1.0},
	{"/home/victor/.config/dwmstatus/90_brightness", 	0.03},
	{"/home/victor/.config/dwmstatus/91_volume", 		0.2},
	{"/home/victor/.config/dwmstatus/95_comms", 		3.0},
	{"/home/victor/.config/dwmstatus/96_cpustat", 		2.0},
	{"/home/victor/.config/dwmstatus/98_memstat", 		0.2},
	//{"/home/victor/.config/dwmstatus/97_coretemp", 		2.0},
	{"/home/victor/.config/dwmstatus/99_battery", 		0.1},
	{"/home/victor/.config/dwmstatus/ZZ_date", 		1.0},

};
