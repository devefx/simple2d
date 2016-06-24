//Bitmap.cpp

#ifndef __YGGR_BITMAP_H__
#define __YGGR_BITMAP_H__

#include <string>

class Bitmap
{
	typedef unsigned char u8;
	typedef signed char s8;

	typedef unsigned short u16;
	typedef signed short s16;

	typedef unsigned long u32;
	typedef signed long s32;

	#include "PshPack2.h" //使数据两字节对齐
	struct BitmapFileHeader //14
	{ 
		u16 type; //must mb 
		u32 size; 
		u16 rvd1; 
		u16 rvd2; 
		u32 offbits; 
	};
	#include "PopPack.h"

	struct BitmapInfoHeader //40
	{
		u32 size; 
		s32 w; 
		s32 h; 
		u16 plans; 
		u16 bitCount; 
		u32 compression; 
		u32 sizeImage; 
		s32 xPelsPerMeter; 
		s32 yPelsPerMeter; 
		u32 clrUsed; 
		u32 clrImportant; 
	};
public:
	Bitmap(void);
	Bitmap(const std::string& fname);
	virtual ~Bitmap(void);

	bool read(const std::string& fname);
	bool write(const std::string& fname);
	
	void clear(void)
	{
		if(_ppattle)
		{
			delete[] _ppattle;
			_ppattle = 0;
		}

		if(_ppix)
		{
			delete[] _ppix;
			_ppix = 0;
		}

		if(_pidx)
		{
			delete[] _pidx;
			_pidx = 0;
		}
	}

	inline u32 getWidth(void) const
	{
		return _bih.w;
	};

	inline u32 getHeight(void) const
	{
		return _bih.h;
	};

	inline u32 getBitCount(void) const
	{
		return _nowbits;
	};

	inline u32 getOrgBitCount(void) const
	{
		return _bih.bitCount;
	};

#ifdef _WINDOWS_
	const u8* getBitmap(HBITMAP& hbmp) const
	{
		//reverseRow((u32*)_ppix);
		hbmp = CreateBitmap(_bih.w, _bih.h, _bih.plans, _nowbits, _ppix);
		return _ppix;
	}
#endif //_WINDOWS_

	const u8* getBitmap(void) const
	{
		return _ppix;
	}
private:
	
	bool loadBitmap(std::ifstream& ifs);
	bool saveBitmap(std::ofstream& ofs);

	void changeBits(void);

	void change32To32(void);
	void change24To32(void);
	void change16To32(void);

	void change16To32Idx(void);
	void change8To32Idx(void);
	void change4To32Idx(void);
	void change1To32Idx(void);

	void changeBits(u8* pdst, const u8* psrc) const;
	void change32To32(u8* pdst, const u8* psrc) const;
	void change32To24(u8* pdst, const u8* psrc) const;
	void change32To16(u8* pdst, const u8* psrc) const;

	void change32To16Idx(u8* pdst, const u8* psrc) const;
	void change32To8Idx(u8* pdst, const u8* psrc) const;
	void change32To4Idx(u8* pdst, const u8* psrc) const;
	void change32To1Idx(u8* pdst, const u8* psrc) const;
	

	void reverseRow(u32 *ppix) const;

private:
	std::string _fname;
	BitmapFileHeader _bfh; //文件头
	BitmapInfoHeader _bih; //信息头
	BitmapFileHeader _bfhOut; //输出时的文件头
	BitmapInfoHeader _bihOut;//输出时的信息头

	bool _bidx; //是否是索引位图
	s32 _pattlesize; //调色板大小
	s32 _nowbits;

	u32* _ppattle; //调色板数组
	u8* _ppix; //象素的二维数组
	u8* _pidx; //索引数组

};

#endif //__YGGR_BITMAP_H__