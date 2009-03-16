/**
 * @file CppCounter.cpp
 * @brief implementation of the CppCounter class.
 * @author Boom( boomworks@hotmail.com )
 * @author Copyright(C) 2004-2005 boomworks.org , All right reserved.
 * @date 2005-12-10
 * $Revision: $
 */

#include <wx/textfile.h>
#include "CppCounter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CppCounter
//////////////////////////////////////////////////////////////////////
CppCounter* CppCounter::ms_instance = 0;

CppCounter::CppCounter()
{}

CppCounter::~CppCounter()
{}

CppCounter* CppCounter::GetInstance()
{
    if ( ms_instance == 0)
    {
        ms_instance = new CppCounter;
    }
    return ms_instance;
}

void CppCounter::countingSourceFile(wxTextFile& file, int& nLines, int& nCodeLines, int& nCommentLines, int& nBlankLines )
{
    // get file size
    // nSize = file.Len();

    //////////////////////////////////////////////////////////////////////////

    BOOL bCommentSet = FALSE; //�A�˘ΗR�ĉ��� �o"/*"��TRUE, "*/"��FALSE
    BOOL bQuatoSet = FALSE;   //�u�����R�ĉ��� �Պ̙��Κo�����"��TRUE, �a���Κo�����"��FALSE

    wxString bufRead;


    for ( bufRead = file.GetFirstLine(); !file.Eof(); bufRead = file.GetNextLine() )
    {
        BOOL bStatedComment = FALSE;//�]�Μ����A�˘ΕŌ��ϗR�č�
        BOOL bStatedCode = FALSE;   //�]�Μ�������ΕŌ��ϗR�č�

        nLines++;

        bufRead.Trim(false); //�g�J���V�c�t��ĉ�������

        if ( bufRead.Len() ==0 ) //���t�՘�
        {
            nBlankLines++;
            continue;
        }

        if ( bCommentSet && bufRead.Find( _T("*/") ) == -1 )
        {
            nCommentLines++;
            continue;
        }

        if ( bufRead.Find( _T("//") ) ==-1 && bufRead.Find( _T("/*") ) ==-1 && bufRead.Find( _T("*/") ) ==-1 )
        {//�卛�]�Ό��]�˗ܜA�ˌ��E�𙨉��ŜA�ˌ��E�����Ŋ����
            if ( bCommentSet )
            {
                nCommentLines++;
                continue;
            }
            else
            {
                if ( bufRead.Find( _T('"') ) ==-1 )
                {
                    nCodeLines++;
                    continue;
                }
            }
        }

        if ( bufRead.Find( _T("//") ) ==0 && !bCommentSet && !bQuatoSet )
        {
            nCommentLines++;
            continue;
        }

        BOOL bDoubleSlashFound = FALSE;
        BOOL bSplashStarFound = FALSE;

        for (unsigned int i=0; i<bufRead.Len()-1; i++)
        {
            // TCHAR cTemp = bufRead[i];
            if ( bufRead[i] == _T('/') && bufRead[i+1] == _T('/') && !bCommentSet && !bQuatoSet )
            {
                if ( !bStatedComment
                        && ( m_nLineCountingType == NStatisticAll || m_nLineCountingType == NStatisticCommentOnly ))
                {
                    bStatedComment = TRUE;
                    nCommentLines++;
                }
                bDoubleSlashFound = TRUE;
                //i++;//�E��+1�E�J���Z�o���S�b��
                break;
            }
            else if ( bufRead[i] == _T('/') && bufRead[i+1] == _T('*') && !bCommentSet && !bQuatoSet )
            {
                if ( !bStatedComment
                        && ( m_nLineCountingType == NStatisticAll || m_nLineCountingType == NStatisticCommentOnly ))
                {
                    bStatedComment = TRUE;
                    nCommentLines++;
                }
                bCommentSet = TRUE;
                bSplashStarFound = TRUE;
                i++;
            }
            //�Ė�����Ήw���bCommentSet�w�t���O
            else if ( bufRead[i] != _T(' ') && bufRead[i] != _T('\t') && !bCommentSet )
            {
                if ( !bStatedCode )
                {
                    bStatedCode = TRUE;
                    nCodeLines++;
                }
                if ( bufRead[i] == _T('\\') )
                {//\����c�u��������
                    i++;
                    continue;
                }
                if ( bufRead[i]=='\'' )
                {
                    if ( bufRead[i+1]=='\\' )
                        i+=2;
                    else
                        i+=1;
                    continue;
                }
                if ( bufRead[i]=='"' )
                {//"�w�����֕сE�Θ�ltzhou
                    bQuatoSet = !bQuatoSet;
                }
            }
            else if ( bufRead[i]=='*' && bufRead[i+1]=='/' && bCommentSet && !bQuatoSet )
            {
                if ( !bStatedComment
                        && ( m_nLineCountingType == NStatisticAll || m_nLineCountingType == NStatisticCommentOnly))
                {
                    bStatedComment = TRUE;
                    nCommentLines++;
                }
                bCommentSet = FALSE;
                bSplashStarFound = TRUE;
                i++;
            }
        }

        if ( bDoubleSlashFound )
        {
            if ( m_nLineCountingType == NStatisticCommentOnly && bStatedCode ) //�卛�R�Č\�G���y���ԁE�q�K���o����Κ��A�˘΁E�𛹎ĜA�˘�
            {
                nCodeLines--;
            }

            if ( m_nLineCountingType == NStatisticCodeOnly && !bStatedCode ) //�卛�R�Č\�G���y���ԁE�q����������ΗR�č��E�ŒS�w���A�˘�
            {
                nCommentLines++;
            }
            continue;
        }

        if ( bufRead[bufRead.Len() -1]=='"'&&!bCommentSet )
        {//�򒱘Μ��񙹌���"�E���w���b���w�tbQuatoSet�E�Ŋ���Ι��΁E�����G��
            bQuatoSet = !bQuatoSet;
            if ( !bQuatoSet )
            {
                if ( !bStatedCode )
                {
                    bStatedCode = TRUE;
                    nCodeLines++;
                }
            }
            else
            {
//				CStdioFile fileLog;
//				if( fileLog.Open( m_strLogFile, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate ) ==TRUE )
//				{
//					wxString strMsg;
//					if( fileLog.Len() ==0 )
//					{
//						strMsg.Format( "��\t��\t�Ȗ�\n", strFileName, nLines );
//						fileLog.WriteString( strMsg );
//					}
//					strMsg.Format( "%s\t%d\t�u�����Z�Η��b\\\n", strFileName, nLines );
//					fileLog.WriteString( strMsg );
//					fileLog.Close();
//				}
            }
            continue;
        }

        if ( bufRead[bufRead.Len() -1]!=' ' && bufRead[bufRead.Len() -1]!='\t' && !bCommentSet
                && bufRead[bufRead.Len() -2]!='*' && bufRead[bufRead.Len() -1]!='/' )
        {//�卛���񙹌��u���f�t��ĉ����E�q�O����/*�E����\���u������*/�E�𗨊����
            if ( !bStatedCode )
            {
                bStatedCode = TRUE;
                nCodeLines++;
            }
        }

        if ( bSplashStarFound )
        {
            if ( m_nLineCountingType == NStatisticCommentOnly && bStatedCode ) //�卛�R�Č\�G���y���ԁE�q�K���o����Κ��A�˘΁E�𛹎ĜA�˘�
            {
                nCodeLines--;
            }

            if ( m_nLineCountingType == NStatisticCodeOnly && !bStatedCode && !bStatedComment ) 	//����Η܊����    /*abc*/ //222
                //�J�ŗR�Č\�G�ŋy���ԁE��晨�V�ќA�˘Ύĕ�����
            {
                bStatedComment = TRUE;
                nCommentLines++;
            }
        }

        if ( !bStatedComment && bCommentSet ) //�h�ڕŔO���o/*�E�؋y���ԗR�Č\�G�΁E����������ΎĖ����E�ŒS�]�ΐn���ŜA�˘�
        {
            if ( m_nLineCountingType == NStatisticCodeOnly && !bStatedCode )
            {
                bStatedComment = TRUE;
                nCommentLines++;
            }
        }
    }
}
