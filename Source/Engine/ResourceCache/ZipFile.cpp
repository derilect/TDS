#include <cctype>
#include "ZipFile.h"

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


