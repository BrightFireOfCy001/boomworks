/**
 * @file PasCounter.cpp
 * @brief implementation of the PascalCounter class.
 * @author Boom( boomworks@hotmail.com )
 * @author Copyright(C) 2004-2005 BoomWorks.Net , All right reserved.
 * @date 2005-12-10
 * $Revision: $
 */

#include <wx/textfile.h>

#include "PasCounter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PascalCounter::PascalCounter()
{}

PascalCounter::~PascalCounter()
{}

void PascalCounter::countingSourceFile( wxTextFile& file, int& nLines, int& nCodeLines, int& nCommentLines, int& nBlankLines )
{
    // get file size
//	nSize = file.Len();

    //////////////////////////////////////////////////////////////////////////

    BOOL bHasSettedCommentFlag	= FALSE;	//�A�˘ΗR�ĉ��� �o"{"��TRUE, "}"��FALSE
    BOOL bHasSettedQuatoFlag	= FALSE;	//�u�����R�ĉ��� �Պ̙��Κo�����"��TRUE, �a���Κo�����"��FALSE

    wxString bufCurLine;
    for ( bufCurLine = file.GetFirstLine(); !file.Eof(); bufCurLine = file.GetNextLine() )
    {
        //BOOL bStatisticedAsComment	= FALSE;	//�]�Μ����A�˘ΕŌ��ϗR�č�
        //BOOL bStatisticedAsCode		= FALSE;	//�]�Μ�������ΕŌ��ϗR�č�

        nLines++;

        bufCurLine.Trim(false); //�g�J���V�c�t��ĉ�������

        if ( bufCurLine.Len() == 0 )
        { //���t�՘� OK
            nBlankLines++;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////

        bufCurLine.Trim();
        int nLen = bufCurLine.Len();
        if ( !bHasSettedCommentFlag
                && bufCurLine[ 0 ]			==  _T( '{' )
                && bufCurLine[ nLen - 1 ]	==  _T( '}' ))
        {
            nCommentLines++;
            continue;
        }
        else if ( !bHasSettedCommentFlag
                  && bufCurLine.Find( _T( "{" )) != -1
                  && bufCurLine.Find( _T( "}" )) != -1 )
        {
            nCommentLines++;
            nCodeLines++;
            continue;
        }

        if ( !bHasSettedCommentFlag )
        {
            if ( bufCurLine[0]	== _T( '/' )
                    && bufCurLine[1]		== _T( '/' ) )
            {
                nCommentLines++;
                continue;
            }
            else if ( bufCurLine.Find( _T( "//" )) != -1 )
            { // �ۉ��[�� ���ΐ�o''�c�Ȗ�
                nCommentLines++;
                nCodeLines++;
                continue;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        if ( !bHasSettedCommentFlag
                && bufCurLine[0] == _T('{')
                && bufCurLine.Find( _T( '}' )) == -1 )
        {
            bHasSettedCommentFlag = TRUE;
            nCommentLines++;
            continue;
        }

        if (bHasSettedCommentFlag
                && bufCurLine[nLen - 1] != _T('}') )
        {
            nCommentLines++;
            continue;
        }
        else if ( bHasSettedCommentFlag
                  && bufCurLine[nLen - 1] == _T('}'))
        {
            bHasSettedCommentFlag = FALSE;
            nCommentLines++;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////

        if ( bufCurLine.Find( _T( "//" ) ) == -1
                && bufCurLine.Find( _T( "{" ) ) == -1
                && bufCurLine.Find( _T( "}" ) ) == -1 )
        { //�卛�]�Ό��]�˗ܜA�ˌ��E�𙨉��ŜA�˘΁E�����Ŋ����
            if ( bHasSettedCommentFlag )
            {
                nCommentLines++;
                continue;
            }
            else
            {
                nCodeLines++;
                continue;
            }
        }

        //////////////////////////////////////////////////////////////////////////

        if ( bufCurLine.Find( _T( "//" ) ) == 0 && !bHasSettedCommentFlag && !bHasSettedQuatoFlag )
        { // single line comment
            nCommentLines++;
            continue;
        }

        //////////////////////////////////////////////////////////////////////////


        //////////////////////////////////////////////////////////////////////////

    }// End for
}
