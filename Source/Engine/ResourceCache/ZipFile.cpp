#include <cctype>
#include <string>
#include "ZipFile.h"
#include "..\MSVC\GameCodeStd.h"

typedef unsigned long dword;
typedef unsigned short word;
typedef unsigned char byte;

#pragma pack(1)

struct ZipFile::TZipLocalHeader
{
	enum
	{
		SIGNATURE = 0x04034b50,
	};

	dword	sig;
	word	version;
	word	flag;
	word	compression;	// Z_NO_COMPRESSION or Z_DEFLATED
	word	modTime;
	word	modDate;
	dword	crc32;
	dword	cSize;
	dword	ucSize;
	word	fnameLen;	// Filename string follows header
	word	xtraLen;	// Extra Field follows filename
};


struct ZipFile::TZipDirHeader
{
	enum
	{
		SIGNATURE = 0x06054b50
	};

	dword	sig;
	word	nDisk;
	word	nStartDisk;
	word	nDirEntries;
	word	totalDirEntries;
	dword	dirSize;
	dword	dirOffset;
	word	cmntLen;
};


struct ZipFile::TZipDirFileHeader
{
	enum
	{
		SIGNATURE   = 0x02014b50
	};

	dword	sig;
	word	verMade;
	word	verNeeded;
	word	flag;
	word	compression;
	word	modTime;
	word	modDate;
	dword	crc32;
	dword	cSize;
	dword	ucSize;
	word	fnameLen;
	word	xtraLen;
	word	cmntLen;
	word	diskStart;
	word	intAttr;
	dword	extAttr;
	dword	hdrOffset;

	char *GeName() const { return (char *)(this + 1); }
	char *GetExtra() const { return GetName() + fnameLen; }
	char *GetComment() const { return GetExtra() + xtraLen; }
};

#pragma pack()

bool ZipFile::Init(const std::wstring &resFilename)
{
	End();
	_wfopen_s(&m_pFile, resFilename.c_str(), _T("rb"));
	
	if(!m_pFile)
		return false;
	TZipDirHeader dh;

	fseek(m_pFile, -(int)sizeof(dh), SEEK_END);
	long dhOffset = ftell(m_pFile);
	memset(&dh, 0, sizeof(dh));
	fread(&dh, sizeof(dh), 1, m_pFile);

	if(dh.sig != TZipDirHeader::SIGNATURE)
		return false;

	fseek(m_pFile, dhOffset - dh.dirSize, SEEK_SET);

	m_pDirData = new char[dh.dirSize + dh.nDirEntries * sizeof(*m_papDir)];

	if(!m_pDirData)
		return false;
	
	memset(m_pDirData, 0, dh.dirSize + dh.nDirEntries * sizeof(*m_papDir));
	fread(m_pDirData, dh.dirSize, 1, m_pFile);

	// process each entry
	char *pfh = m_pDirData;
	m_papDir = (const TZipDirFileHeader **)(m_pDirData + dh.dirSize);

	bool success = true;

	for(int i = 0; i < dh.nDirEntries && success; i++)
	{
		TZipDirFileHeader &fh = *(TZipDirFileHeader*)pfh;
		m_papDir[i] = &fh;

		if(fh.sig != TZipDirFileHeader::SIGNATURE)
			success = false;
		else
		{
			pfh += sizeof(fh);

			for(int j = 0; j < fh.fnameLen; j++)
				if(pfh[j] == '/')
					pfh[j] = '\\';

			char fileName[_MAX_PATH];
			memcpy(fileName, pfh, fh.fnameLen);
			fileName[fh.fnameLen] = 0;
			_strlwr_s(fileName, _MAX_PATH);
			std::string spath = fileName;
			m_ZipContentsMap[spath] = i;

			pfh += fh.fnameLen + fh.xtraLen + fh.cmntLen;
		}
	}
	if(!success)
	{
		SAFE_DELETE_ARRAY(m_pDirData);
	}
	else
	{
		m_nEntries = dh.nDirEntries;
	}
	
	return success;
}





