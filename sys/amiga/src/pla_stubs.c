#include "plplot/pla_menu.h"






int PlplotItem0( void )
{
	return(plamiga_Open());
}

int PlplotItem1( void )
{
	return(plamiga_Save());
}

int PlplotItem2( void )
{
	return(plamiga_SaveAs_ILBM());
}

int PlplotItem3( void )
{
	return(plamiga_Print_Bitmap());
}

int PlplotItem4( void )
{
	return(plamiga_Print_landscape());
}

int PlplotItem5( void )
{
	return(plamiga_Print_portrait());
}

int PlplotItem6( void )
{
	return(plamiga_About());
}

int PlplotItem7( void )
{
	return(plamiga_Quit());
}

int PlplotItem8( void )
{
	return(plamiga_Screenmode());
}

int PlplotItem9( void )
{
	return(plamiga_Palette0());
}

int PlplotItem10( void )
{
	return(plamiga_Palette1());
}

int PlplotItem11( void )
{
	return(plamiga_LoadConfig());
}

int PlplotItem12( void )
{
	return(plamiga_SaveConfigAs());
}

int PlplotItem13( void )
{
	return(plamiga_SaveConfig());
}

int PlplotItem14( void )
{
	return(plamiga_DUMMY());
}

int PlplotVanillaKey( void )
{
	return(plamiga_KEY());
}

