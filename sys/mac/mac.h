// private functions prototype

void plD_init_mac(PLStream *pls);
void plD_line_mac(PLStream *pls, short x1a, short y1a, short x2a, short y2a);
void plD_polyline_mac(PLStream *pls, short *xa, short *ya, PLINT npts);
void plD_bop_mac(PLStream *pls);
void plD_eop_mac(PLStream *pls);
void plD_tidy_mac(PLStream *pls);
void plD_state_mac(PLStream *pls, PLINT op);
void plD_esc_mac(PLStream *pls, PLINT op, void *ptr);
void mac_text(PLStream *pls);
void doSave(void);
void SetUpMenu(void);
void AdjustMenu(void);
void RestoreMenu(void);
Boolean MAC_event(void);
Boolean doMenuCommand(long mCmd);
void doCopy();
void PutPictToFile(void);
void DoAbout( Str255 s );
void mac_plot_area(Rect *plotArea);
void CatenatePStrings(Str255 targetStr, Str255 appendStr);
void CopyPStrings(Str255 targetStr, Str255 appendStr);
