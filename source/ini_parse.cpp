#include "resources.h"


#define INI_DATA_TYPE_BOOL		0
#define INI_DATA_TYPE_INT		1
#define INI_DATA_TYPE_UINT		2
#define INI_DATA_TYPE_FLOAT		3
#define INI_DATA_TYPE_CHAR		4
#define INI_DATA_TYPE_STRING	5

void readSetting(char *fileBuffer, string heading, string setting, unsigned int dataType, void *internalLocation)
{
	char *spot = fileBuffer;

	char delim[] = {'\x20', '\x0d', '\x0a', '=', '\x00'};


	heading.insert(0, "[");
	heading += "]";
	//printf("heading: %s\n", heading.c_str() );

	spot = strstr( spot, heading.c_str() );
	spot += heading.size();


	spot = strstr(spot, setting.c_str() );
	spot += setting.size();
	spot += strspn(spot, delim);

	if (dataType == INI_DATA_TYPE_UINT)
	{
		unsigned int uintBuffer = atoi(spot);
		//printf("\n\ngot: %u\n\n", uintBuffer);

		unsigned int *uintLoc = (unsigned int *)internalLocation;

		*uintLoc = uintBuffer;

		//printf("coverted to: %u\n", display_x);
	}
	if (dataType == INI_DATA_TYPE_BOOL)			// don't need to mess with this actually.
	{
		// look for yes, no, true, and false
		//strstr
	}


	//printf("======\n%s\n======", spot);


}








void load_configuration(string filename)
{

	FILE *fp = fopen(filename.c_str(), "r");


	if (!fp)
	{
		printf("couldn't open file: %s\n", filename.c_str() );
		return;
	}


	fseek(fp, 0, SEEK_END);
	unsigned int fsize = ftell(fp);
	rewind(fp);

	char *fbuffer = (char *)malloc( sizeof(char) * fsize );

	unsigned int success = fread (fbuffer,1,fsize,fp);

	if (success != fsize)
	{
		printf("fread error\n");
	}


	fbuffer[fsize-1] = 0;
	fclose(fp);




	///////////////////////////
	// parse data
	readSetting(fbuffer, "video", "display_x", INI_DATA_TYPE_UINT, &displayWidth);
	readSetting(fbuffer, "video", "display_y", INI_DATA_TYPE_UINT, &displayHeight);
	readSetting(fbuffer, "video", "display_bpp", INI_DATA_TYPE_UINT, &displayBpp);

	unsigned int fullscreen = 0;
	readSetting(fbuffer, "video", "fullscreen", INI_DATA_TYPE_UINT, &fullscreen);
	if (fullscreen == 0)
		displayFullscreen = false;
	else
		displayFullscreen = true;

	unsigned int hw_cursor = 1;
	readSetting(fbuffer, "video", "hardware_cursor", INI_DATA_TYPE_UINT, &hw_cursor);
	if (hw_cursor == 0)
		hardwareCursor = false;
	else
		hardwareCursor = true;

	//printf("hwc=%u, fs=%u\n", (unsigned int)hardwareCursor, (unsigned int)displayFullscreen );



	free(fbuffer);
}





