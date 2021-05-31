/*
 *
 *   TEA - A quick and simple text preprocessor
 *   Copyright (C) 2010-2016 DarkBatcher
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

#ifndef WIN32
#include <strings.h>
#endif

#include <libpBat.h>
#include "tea_out.h"
#include "../Tea.h"

void Tea_HtmlOutputHandler(TEAPAGE* lpTeaPage, FILE* pFile, int i, char** argv)
{
	ESTR *lpEsStr=pBat_EsInit(),
		 *lpEsTitle=pBat_EsInit(),
		 *lpEsMeta=pBat_EsInit(),
		 *lpEsHead=pBat_EsInit(),
		 *lpEsFoot=pBat_EsInit();

	char *lpHeadTag="<h1>",
	      *lpHeadTagEnd="</h1>";

	int iLastLevel=0,
	    iCompMode=0,
	    iBckLevel;

	/* execute command line */


	while (argv[i]) {

		if (!stricmp(argv[i], "/T")) {

			if (!argv[++i]) {

				fputs("TEA :: Error : Invalid parameter (after /T)\n", stderr);
				goto error;

			}

			pBat_EsCpy(lpEsTitle, argv[i]);

		} else if (!strcmp(argv[i], "/H")) {

			if (!argv[++i]) {

				fputs("TEA :: Error : Invalid parameter (after /H)\n", stderr);
				goto error;

			}

			Tea_HtmlLoadFile(lpEsHead, argv[i]);


		} else if (!strcmp(argv[i], "/F")) {

			if (!argv[++i]) {

				fputs("TEA :: Error : Invalid parameter (after /F)\n", stderr);
				goto error;

			}

			Tea_HtmlLoadFile(lpEsFoot, argv[i]);

		} else if (!strcmp(argv[i], "/M")) {

			if (!argv[++i]) {

				fputs("TEA :: Error : Invalid parameter (after /M)\n", stderr);
				goto error;

			}

			Tea_HtmlLoadFile(lpEsMeta, argv[i]);

		} else if (!strnicmp(argv[i], "/C", 2)) {

			argv[i]+=2;

			if (*(argv[i])==':')
				argv[i]++;

			if (!stricmp(argv[i], "HTML4")) {

				iCompMode=1;

			} else if (!stricmp(argv[i], "XHTML")) {

				iCompMode=2;

			} else if (!stricmp(argv[i], "MD")) {

			    iCompMode=3;

			} else {

				fprintf(stderr,"TEA :: Error : ``%s'' is not a valid html flavour.\n", argv[i]);
				goto error;

			}

		} else {

			fprintf(stderr,"TEA :: Error : Invalid argument \"%s\"\n", argv[i]);
			goto error;

		}

		i++;
	}


	/* Get the first heading to put it on the
	   the title of the document */
	Tea_HtmlGetTitle(lpTeaPage, lpEsStr);
	pBat_EsCatE(lpEsTitle, lpEsStr);

	if (iCompMode==0) {

		fprintf(pFile, "<!DOCTYPE html>\n"
		        "<!--\n"
		        "\tThis HTML5 compliant page was generated by the\n"
		        "\tTEA text processor (version 2.0)\n\n"
		        "\tLearn more or get involved at http://www.picobat.org\n"
		        "-->\n"
		        "<html>\n"
		        "<head>\n"
		        "<meta charset=\"%s\" />\n"
		        "<title>%s</title>\n"
		        "%s\n"
		        "</head>\n"
		        "<body>\n"
		        "%s\n",
		        pBat_EsToChar(lpEsEncoding),
		        pBat_EsToChar(lpEsTitle),
		        pBat_EsToChar(lpEsMeta),
		        pBat_EsToChar(lpEsHead));

	} else if (iCompMode==1) {

		fprintf(pFile, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3c.org/TR/html4/strict.dtd\">\n"
		        "<!--\n"
		        "\tThis HTML 4.01 compliant page was generated by the\n"
		        "\tTEA text processor (version 2.0)\n\n"
		        "\tLearn more or get involved at http://www.picobat.org\n"
		        "-->\n"
		        "<html>\n"
		        "<head>\n"
		        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=%s\" />\n"
		        "<title>%s</title>\n"
		        "%s\n"
		        "</head>\n"
		        "<body>\n"
		        "%s\n",
		        pBat_EsToChar(lpEsEncoding),
		        pBat_EsToChar(lpEsTitle),
		        pBat_EsToChar(lpEsMeta),
		        pBat_EsToChar(lpEsHead));


	} else if (iCompMode==2) {

		fprintf(pFile, "<!DOCTYPE html PUBLIC \"-//W3C//DTD//XHTML 1.0 Strict//EN\" \"http://www.w3c.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
		        "<!--\n"
		        "\tThis xHTML 1.1 compliant page was generated by the\n"
		        "\tTEA text processor (version 2.0)\n\n"
		        "\tLearn more or get involved at http://www.picobat.org\n"
		        "-->\n"
		        "<html xmlns=\"http://www.w3c.org/1999/xhtml\">\n"
		        "<head>\n"
		        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=%s\" />\n"
		        "<title>%s</title>\n"
		        "%s\n"
		        "</head>\n"
		        "<body>\n"
		        "%s\n",
		        pBat_EsToChar(lpEsEncoding),
		        pBat_EsToChar(lpEsTitle),
		        pBat_EsToChar(lpEsMeta),
		        pBat_EsToChar(lpEsHead));


	} else if (iCompMode == 3) {

        fprintf(pFile, "---\n"
                "Title: %s\n"
                "Description: %s\n"
                "template: index\n"
                "---\n\n",
                pBat_EsToChar(lpEsTitle),
                pBat_EsToChar(lpEsTitle)
                );

	}

	while (lpTeaPage) {

		switch (lpTeaPage->iBlockType) {

		case TEA_BLOCK_HEADING:

			while (iLastLevel) {

				fputs("</li></ul>\n", pFile);
				iLastLevel--;

			}

			fputs(lpHeadTag, pFile);

			pBat_EsCpy(lpEsStr, lpTeaPage->lpBlockContent);
			pBat_EsReplace(lpEsStr, "\n", "<br />");
			fputs(pBat_EsToChar(lpEsStr), pFile);


			fputs(lpHeadTagEnd, pFile);

			lpHeadTag="<h2>";
			lpHeadTagEnd="</h2>";

			break;

		case TEA_BLOCK_CODE:

			while (iLastLevel > lpTeaPage->iBlockLevel) {

				fputs("</li></ul>\n", pFile);
				iLastLevel--;

			}

			while (iLastLevel < lpTeaPage->iBlockLevel) {

				fputs("<ul><li>\n", pFile);
				iLastLevel++;

			}

			fputs("<pre><code>", pFile);

			pBat_EsCpy(lpEsStr, lpTeaPage->lpBlockContent);
			pBat_EsReplace(lpEsStr, "\n", "<br />");
			fputs(pBat_EsToChar(lpEsStr), pFile);

			fputs("</code></pre>", pFile);

			break;

		case TEA_BLOCK_PARAGRAPH:

			iBckLevel=iLastLevel;

			while (iLastLevel > lpTeaPage->iBlockLevel) {

				fputs("</li></ul>\n", pFile);
				iLastLevel--;

			}

			while (iLastLevel < lpTeaPage->iBlockLevel) {

				fputs("<ul><li>\n", pFile);
				iLastLevel++;

			}

			if (!(iLastLevel>iBckLevel)
			    && !(lpTeaPage->iFlag & TEA_LIST_NO_MARK)
			    && iLastLevel)
				fputs("</li><li>", pFile);



			fputs("<p>", pFile);

			Tea_HtmlOutputParagraph(lpTeaPage->lpTeaNode, pFile,  iCompMode);

			fputs("</p>", pFile);


		}

		fputs("\n\n", pFile);

		lpTeaPage=lpTeaPage->lpTeaNext;

	}

	while (iLastLevel) {

		fputs("</li></ul>\n", pFile);
		iLastLevel--;

	}

	if (iCompMode != 3)
        fprintf(pFile, "%s\n"
	        "</body>\n"
	        "</html>",
	        pBat_EsToChar(lpEsFoot));


	pBat_EsFree(lpEsStr);
	pBat_EsFree(lpEsTitle);
	pBat_EsFree(lpEsHead);
	pBat_EsFree(lpEsMeta);
	pBat_EsFree(lpEsFoot);
	return;

error:
	pBat_EsFree(lpEsStr);
	pBat_EsFree(lpEsTitle);
	pBat_EsFree(lpEsHead);
	pBat_EsFree(lpEsMeta);
	pBat_EsFree(lpEsFoot);
	exit(-1);
}

void Tea_HtmlOutputParagraph(TEANODE* lpTeaNode, FILE* pFile, int iCompMode)
{
	char* lpCh, *ext;
	int   bFirstLine=TRUE;

	while (lpTeaNode) {

		if (lpTeaNode->iNodeType==TEA_NODE_LINK) {

            if (!(ext = strrchr(lpTeaNode->lpTarget, '/')))
                ext = lpTeaNode->lpTarget;

            if (!(ext = strchr(ext, '.'))) {

                ext = ".html";

            } else {

                ext = "";

            }

            if (!strncmp(lpTeaNode->lpTarget, "http://", 7)
                || !strncmp(lpTeaNode->lpTarget, "https://", 8)
                || !strncmp(lpTeaNode->lpTarget, "mail://", 7)
                || iCompMode == 3 )
                ext = "";

			fprintf(pFile, "<a href=\"%s%s\">", lpTeaNode->lpTarget, ext);

		} else if (lpTeaNode->iNodeType==TEA_NODE_EMPHASIS) {

			fputs("<strong>", pFile);

		}

		lpCh=lpTeaNode->lpContent;

		if ((lpTeaNode->iNodeType==TEA_NODE_PLAIN_TEXT)
		    && (bFirstLine)
		    && (*lpCh=='-')) {

			lpCh++;

		}

		bFirstLine=FALSE;

		fputs(lpCh, pFile);

		if (lpTeaNode->iNodeType==TEA_NODE_LINK) {

			fputs("</a>", pFile);

		} else if (lpTeaNode->iNodeType==TEA_NODE_EMPHASIS) {

			fputs("</strong>", pFile);

		}


		lpTeaNode=lpTeaNode->lpTeaNodeNext;

	}

}

void Tea_HtmlParseHandler(int iMsg ,void* lpData)
{

	ESTR* lpEsStr;

	if (iMsg!=TEA_MSG_READ_FILE)
		return;
	/* do nothing */

	lpEsStr=(ESTR*)lpData;

	Tea_HtmlEscapeChar(lpEsStr);
}

void Tea_HtmlEscapeChar(ESTR* lpEsStr)
{
	pBat_EsReplace(lpEsStr, "&", "&amp;");
	pBat_EsReplace(lpEsStr, "<", "&lt;");
	pBat_EsReplace(lpEsStr, ">", "&gt;");

}

void Tea_HtmlGetTitle(TEAPAGE* lpTeaPage, ESTR* lpEsStr)
{

	while (lpTeaPage) {

		if (lpTeaPage->iBlockType == TEA_BLOCK_HEADING) {

			pBat_EsCpy(lpEsStr, lpTeaPage->lpBlockContent);
			return;

		}

		lpTeaPage=lpTeaPage->lpTeaNext;

	}

}

void Tea_HtmlLoadFile(ESTR* lpEsReturn, char* lpFileName)
{

	ESTR* lpEsTmp=pBat_EsInit();
	FILE* pFile;

	pBat_EsCpy(lpEsReturn, "");

	if (!(pFile=fopen(lpFileName, "r"))) {

		fprintf(stderr, "TEA :: Error : Unable to load file \"%s\"\n", lpFileName);
		exit(-1);

	}

	while (!pBat_EsGet(lpEsTmp,pFile)) {

		pBat_EsCatE(lpEsReturn, lpEsTmp);

	}


	pBat_EsFree(lpEsTmp);

}
