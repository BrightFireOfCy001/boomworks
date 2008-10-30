// JspCounter.cpp: implementation of the JspCounter class.
//
//////////////////////////////////////////////////////////////////////

#include <wx/textfile.h>
#include "JspCounter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JspCounter::JspCounter()
{

}

JspCounter::~JspCounter()
{

}

void JspCounter::countingSourceFile(
		wxTextFile& file, int& nLines, int& nCodeLines, int& nCommentLines, int& nBlankLines )
{
	// Get file size
//	nSize = file.Len();

	//////////////////////////////////////////////////////////////////////////

	//
	wxString bufCurLine;

	BOOL bHasSettedCodeFlag	= FALSE;	//�A�˘ΗR�ĉ��� �o"<%"��TRUE, "%>"��FALSE
	// BOOL bHasSettedCommentFlag	= FALSE;	//�u�����R�ĉ��� �Պ̙��Κo�����"��TRUE, �a���Κo�����"��FALSE

	BOOL bStatisticedAsComment	= FALSE;	//�]�Μ����A�˘ΕŌ��ϗR�č�
	BOOL bStatisticedAsCode		= FALSE;	//�]�Μ�������ΕŌ��ϗR�č�

	int nLen = 0;

	//while( file.ReadString( bufCurLine ))
    for ( bufCurLine = file.GetFirstLine(); !file.Eof(); bufCurLine = file.GetNextLine() )

	{
		// Total lines
		nLines++;

		bStatisticedAsComment	= FALSE;	//�]�Μ����A�˘ΕŌ��ϗR�č�
		bStatisticedAsCode		= FALSE;	//�]�Μ�������ΕŌ��ϗR�č�

		bufCurLine.Trim(false);

		if( bufCurLine.Len() == 0 )
		{//���t�՘� OK
			nBlankLines++;
			continue;
		}

		//////////////////////////////////////////////////////////////////////////

		bufCurLine.Trim();

		nLen = bufCurLine.Len();

		if (nLen < 2)
		{
			if (bHasSettedCodeFlag)
			{
				nCodeLines++;
				continue;
			}
			else
			{

			}
		}

		//////////////////////////////////////////////////////////////////////////

		if ( !bHasSettedCodeFlag
				&& bufCurLine[ 0 ]			==  _T( '<' )
				&& bufCurLine[ 1 ]			==	_T( '%' )
				&& bufCurLine[ nLen - 2 ]	==	_T( '%' )
				&& bufCurLine[ nLen - 1 ]	==  _T( '>' ))
		{
			nCodeLines++;
			continue;
		}
		else if ( !bHasSettedCodeFlag
				  && bufCurLine.Find( _T( "<%" )) != -1
				  && bufCurLine.Find( _T( "%>" )) != -1 )
		{
			//nCommentLines++;
			nCodeLines++;
			continue;
		}

		//////////////////////////////////////////////////////////////////////////

		if ( !bHasSettedCodeFlag
				&& bufCurLine[ 0 ]			==  _T( '<' )
				&& bufCurLine[ 1 ]			==	_T( '%' )
				&& bufCurLine.Find( _T( "%>" )) == -1 )
		{
			bHasSettedCodeFlag = TRUE;
			nCodeLines++;
			continue;
		}

		if (bHasSettedCodeFlag
				&& bufCurLine[ nLen - 2 ]	!=	_T( '%' )
				&& bufCurLine[ nLen - 1 ]	!=  _T( '>' ) )
		{//
			if( bufCurLine[0]	== _T( '/' )
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

		}else if ( bHasSettedCodeFlag
				&& bufCurLine[ nLen - 2 ]	==	_T( '%' )
				&& bufCurLine[ nLen - 1 ]	==  _T( '>' ))
		{
			bHasSettedCodeFlag = FALSE;
			nCodeLines++;
			continue;
		}


		//////////////////////////////////////////////////////////////////////////
		if( bufCurLine.Find( _T( "//" ) ) == -1
				&& bufCurLine.Find( _T( "<%" ) ) == -1
				&& bufCurLine.Find( _T( "%>" ) ) == -1 )
		{ //�卛�]�Ό��]�˗ܜA�ˌ��E�𙨉��ŜA�˘΁E�����Ŋ����
			if( bHasSettedCodeFlag )
			{
				nCodeLines++;
				continue;
			}
			else
			{
				//nCommentLines++;
				continue;
			}
		}

	}// End while
}
