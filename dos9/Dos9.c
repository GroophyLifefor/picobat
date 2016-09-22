/*
 *
 *   Dos9 - A Free, Cross-platform command prompt - The Dos9 project
 *   Copyright (C) 2010-2014 Romain GARBI
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include <signal.h>


#if defined WIN32
#include <conio.h>
#endif

#include <libDos9.h>

#include "errors/Dos9_Errors.h"

#include "lang/Dos9_Lang.h"
#include "lang/Dos9_ShowHelp.h"

#include "core/Dos9_Core.h"

#if defined(WIN32) && defined(DOS9_USE_LIBCU8)
#include <libcu8.h>
#endif


/* those are codes of internal commands */

#include "command/Dos9_Commands.h"
#include "command/Dos9_CommandInfo.h"

//#define DOS9_DBG_MODE
#include "core/Dos9_Debug.h"

#include "../config.h"


int main(int argc, char *argv[])
{
    /*  a function which initializes Dos9's core,
        parses the command line argumments,
        And display starting message
    */

    char *lpFileName="",
          lpFileAbs[FILENAME_MAX]="",
         *lpCmdCSwitch=NULL;


    int bQuiet=FALSE,
        bExitAfterCmd=TRUE;

    ESTR* lpesCmd;

#if defined(WIN32) && defined(DOS9_USE_LIBCU8)
    /* Initialize libcu8 */
    if (libcu8_init(&argv) == -1) {

        fprintf(stderr, "Unable to load libcu8...\n");
        return -1;

    }
#endif

    /* Initialize libDos9 */
    Dos9_InitLibDos9();

    /* Set the error handler called on errors that require the process to exit */
    pErrorHandler=Dos9_Exit;

    /* Process the command line, and set the appropriate special variables for
     * special parameters. (note %0 is set when the absolute path of the script
     * is computed) */
    lpCmdCSwitch = Dos9_GetParameters(argv, &lpFileName, &bExitAfterCmd, &bQuiet);

    /* Initialize stdlib random */
    srand(time(NULL));

    /* Set the default terminal color */
    colColor=DOS9_COLOR_DEFAULT;

    /* Set the console title depending on the type of actions performed (ie.
     * command prompt or batch interpretor) and set the DOS9_IS_SCRIPT var
     * appropriately */
    Dos9_InitConsoleTitles(lpFileName, bQuiet);

    /* Generates the absolute path of the script and sets the special variable
     * %0 accordingly */
    if (*lpFileName!='\0') {

        /* generates real path if the path is uncomplete */
        if (Dos9_GetFilePath(lpFileAbs, lpFileName, sizeof(lpFileAbs))==-1)
            Dos9_ShowErrorMessage(DOS9_FILE_ERROR, lpFileName, -1);

        Dos9_SetLocalVar(lpvLocalVars, '0', lpFileAbs);

    } else {

        Dos9_SetLocalVar(lpvLocalVars, '0', "");

    }

    /* Map the commandinfo */
    lpclCommands = Dos9_MapCommandInfo(lpCmdInfo,
                                sizeof(lpCmdInfo)/sizeof(COMMANDINFO));

    /* running auto batch initialisation */
    Dos9_RunAutoBat();

    if (lpCmdCSwitch != NULL) {

        lpesCmd=Dos9_EsInit();

        Dos9_EsCpy(lpesCmd, lpCmdCSwitch);
        Dos9_RunLine(lpesCmd);

        if (bExitAfterCmd == TRUE)
            goto skip;
    }

    /* Set parameters for the file */
    strcpy(ifIn.lpFileName, lpFileAbs);
    ifIn.iPos=0;
    ifIn.bEof=FALSE;

    /* Run either the command prompt or the batch script */
    Dos9_RunBatch(&ifIn);

skip:

    Dos9_Exit();

    Dos9_LibClose();

    return 0;

}
