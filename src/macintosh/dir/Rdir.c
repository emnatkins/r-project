/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 2000--2001  Stefano Iacus and the R core team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __APPLEEVENTS__
# include <AppleEvents.h>
#endif

#ifndef __AEREGISTRY__
# include <AERegistry.h>
#endif

#ifndef __DISKINIT__
# include <DiskInit.h>
#endif

#ifndef __TEXTSERVICES__
# include <TextServices.h>
#endif

#ifndef __WEDEMOAPP__
# include "RIntf.h"
#endif
#include "Graphics.h"
#include "StandardGetFolder.h"

void closedir(DIR *entry)
{
    if (entry != (DIR *) NULL)
	free((void *) entry); 
/*     FreeMemory((void *) entry); */
    else
	return ;   
}

DIR *opendir(char *path)
{
    char pathname[2048];
    CInfoPBRec search_info;
    DIR *entry;
    int error;

    search_info.hFileInfo.ioNamePtr = 0;
    if ((path != (char *) NULL) || (*path != '\0'))
	if ((path[0] != '.') || (path[1] != '\0'))
	    search_info.hFileInfo.ioNamePtr = c2pstr(strcpy(pathname,path));
    search_info.hFileInfo.ioCompletion = 0;
    search_info.hFileInfo.ioVRefNum = 0;
    search_info.hFileInfo.ioFDirIndex = 0;
    search_info.hFileInfo.ioDirID = 0;
    error = PBGetCatInfoSync(&search_info);
    if (error != noErr)
    {
	errno = error;
	return((DIR *) NULL);
    }
    entry = (DIR *) malloc(sizeof(DIR));
    if (entry == (DIR *) NULL)
	return((DIR *) NULL);
    entry->ioVRefNum = search_info.hFileInfo.ioVRefNum;
    entry->ioDrDirID = search_info.hFileInfo.ioDirID;
    entry->ioFDirIndex = 1;
/*  entry->d_VRefNum = search_info.hFileInfo.ioVRefNum;
    entry->d_DirID = search_info.hFileInfo.ioDirID;
    entry->d_index = 1;
*/  
    return(entry);
}

static unsigned char pathname[2048];

struct dirent *readdir(DIR *entry)
{
    CInfoPBRec search_info;
    int error;
    static struct dirent dir_entry;

    if (entry == (DIR *) NULL)
	return((struct dirent *) NULL);
    search_info.hFileInfo.ioCompletion = 0;
    search_info.hFileInfo.ioNamePtr = pathname;
    search_info.hFileInfo.ioVRefNum = 0;
    search_info.hFileInfo.ioFDirIndex = entry->ioFDirIndex;
    search_info.hFileInfo.ioDirID = entry->ioDrDirID;
/*  search_info.hFileInfo.ioFDirIndex = entry->d_index;
    search_info.hFileInfo.ioDirID = entry->d_DirID;
*/
    error = PBGetCatInfoSync(&search_info);
    if (error != noErr)
    {
	errno = error;
	return((struct dirent *) NULL);
    }
/*  entry->d_index++;
 */
    entry->ioFDirIndex++;
    (void) strcpy(dir_entry.d_name,p2cstr(search_info.hFileInfo.ioNamePtr));
    dir_entry.d_namlen = strlen(dir_entry.d_name);
    return(&dir_entry);
}

void seekdir(DIR *entry, long position)
{
    if (entry != (DIR *) NULL)
	entry->ioFDirIndex = position;
    /*   entry->d_index = position; */
    else
	return ;   
}

void rewinddir(DIR *entry)
{
    seekdir(entry, 1);
}
