#include "bitmap.h"
#include <memory.h>
#include <fstream>
#include <algorithm>

Bitmap::Bitmap(void)
	: _ppattle(0), _ppix(0), _pidx(0), _pattlesize(0), _nowbits(32), _bidx(false)
{
	memset(&_bfh, 0, sizeof(BitmapFileHeader));
	memset(&_bih, 0, sizeof(BitmapInfoHeader));
}

Bitmap::Bitmap(const std::string& fname)
	: _ppattle(0), _ppix(0), _pidx(0), _pattlesize(0), _nowbits(32), _bidx(false)
{
	memset(&_bfh, 0, sizeof(BitmapFileHeader));
	memset(&_bih, 0, sizeof(BitmapInfoHeader));
	read(fname);
}

Bitmap::~Bitmap(void)
{
	clear();
}

bool Bitmap::read(const std::string& fname)
{
	clear();
	_fname = fname;
	std::ifstream ifs(_fname.c_str(), std::ios_base::in | std::ios_base::binary);

	ifs.read((char*)&_bfh, sizeof(BitmapFileHeader)); //读取文件头
	ifs.read((char*)&_bih, sizeof(BitmapInfoHeader)); //读取信息头

	memcpy(&_bfhOut, &_bfh, sizeof(BitmapFileHeader));
	memcpy(&_bihOut, &_bih, sizeof(BitmapInfoHeader));
	
	if(_bfh.type != 'MB')
	{
		ifs.close();
		return false;
	}
	
	//文件头和信息头后面到数据段之前 为调色板数据
	_pattlesize = _bfh.offbits - (sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader));
	if(_pattlesize < 0 || (_pattlesize && _bih.bitCount >= 24)
		|| (!_bih.w) || (!_bih.h))
	{
		return false;
	}
	
	if(_pattlesize > 0) //读取调色版数据
	{
		_ppattle = new u32[_pattlesize / 4];
		memset(_ppattle, 0, _pattlesize);
		ifs.read((char*)_ppattle, _pattlesize);
		_bidx = true;
	}

	bool bread =  loadBitmap(ifs); //读取位图

	ifs.close();
	return bread;
}

bool Bitmap::loadBitmap(std::ifstream& ifs)
{
	s32 size = (_bih.w * _bih.h * _bih.bitCount + 8 - 1) / 8;
	s32 rowsize = (size  + _bih.h - 1) / _bih.h;

	//处理填料数据（4字节对齐）
	s32 nlast = (_bih.w * _bih.bitCount + 8 - 1) / 8 % 4;
	s32 nlfill = nlast? 4 - nlast : nlast;

	_ppix = new u8[rowsize * _bih.h];
	memset(_ppix, 0, rowsize * _bih.h);

	if(!_ppix)
	{
		return false;
	}
	
	//定位到数据段开始位置
	ifs.seekg(_bfh.offbits, std::ios_base::beg);

	//delete []_ppix;
	
	for(s32 i = 0; i < _bih.h; ++i) // 注意宽度一定要被4整除
	{
		ifs.read((char*)(_ppix + i * rowsize) , rowsize);
		ifs.seekg(nlfill, std::ios_base::cur); //跳过填料数据
	}

	changeBits();
	reverseRow((u32*)_ppix);
	return true;
}


void Bitmap::changeBits(void)
{
	if(!_bidx)
	{
		if(_nowbits == 32 && _bih.bitCount == 32)
		{
			change32To32();
			return;
		}

		if(_nowbits == 32 && _bih.bitCount == 24)
		{
			change24To32();
			return;
		}

		if(_nowbits == 32 && _bih.bitCount == 16)
		{
			change16To32();
			return;
		}
	}
	else
	{
		if(_nowbits == 32 && _bih.bitCount == 16)
		{
			change16To32Idx();
			return;
		}

		if(_nowbits == 32 && _bih.bitCount == 8)
		{
			change8To32Idx();
			return;
		}

		if(_nowbits == 32 && _bih.bitCount == 4)
		{
			change4To32Idx();
			return;
		}

		if(_nowbits == 32 && _bih.bitCount == 1)
		{
			change1To32Idx();
			return;
		}
	}
}

void Bitmap::change32To32(void)
{
	s32 size = _bih.w * _bih.h;
	std::reverse((u32*)_ppix,(u32*)_ppix + size);
}

void Bitmap::change24To32(void)
{
	s32 size = _bih.w * _bih.h;
	u8* p = _ppix;

	u32 *p2 = new u32[size];
	memset(p2, 0, sizeof(u32) * size);
	u8 *tp = 0;
	{
		for(s32 i = 0; i < size; ++i)
		{
			tp = (u8*)(p2 + i);
			tp[0] = *(p + i * 3);
			tp[1] = *(p + i * 3 + 1);
			tp[2] = *(p + i * 3 + 2);
		}
	}

	std::reverse(p2, p2 + size);
	delete[] _ppix;
	_ppix = (u8*)p2;
}

void Bitmap::change16To32(void)
{

	//if(!_bih.compression) // BI_RGB
	//{
	s32 size  = _bih.w * _bih.h;
	std::reverse((u16*)_ppix,(u16*)_ppix + size);
	u16 *p = (u16*)_ppix;

	u32 *p2 = new u32[size];
	memset(p2, 0, sizeof(u32) * size);

	u8 *tp = 0;
	u16 r = 0, g = 0, b = 0, clr = 0;
	for(s32 i = 0; i < size; ++i)
	{
		tp = (u8*)(p2 + i);
		clr = *(p + i);
	

		r = clr << 1;
		r >>= 11;
		g = clr << 6;
		g >>= 11;
		b = clr << 11;
		b >>= 11;

		//u16 nt = 0x7c00;
		//u8 kk= 1.0f / 32.0f * 255.0f * ((nt << 6) >> 11); //注意这样算是不行的
		tp[0] = (float)b / 32.0f * 255.0f; 
		tp[1] = (float)g / 32.0f * 255.0f;
		tp[2] = (float)r / 32.0f * 255.0f;
		
	}

	delete[] _ppix;
	_ppix = (u8*)p2;
	return;
	/*}*/
}

void Bitmap::change16To32Idx(void)
{
	s32 size =  _bih.w * _bih.h;
	std::reverse((u16*)_ppix,(u16*)_ppix + size);
	u16 *p = (u16*)_ppix;

	u32 *p2 = new u32[size];
	memset(p2, 0, sizeof(u32) * size);
	
	u8 *tp = 0;
	for(s32 i = 0; i < size; ++i)
	{	
		tp = (u8*)(p2 + i);
		tp[2] = (float) (p[i] & _ppattle[0]) / _ppattle[0] * 255.0f;
		tp[1] = (float) (p[i] & _ppattle[1]) / _ppattle[1] * 255.0f;
		tp[0] = ((float)(p[i] & _ppattle[2])) /_ppattle[2] * 255.0f;
	}

	delete[] _ppix;
	_ppix = (u8*)p2;
	return;
}

void Bitmap::change8To32Idx(void)
{
	s32 size =  _bih.w * _bih.h;
	std::reverse(_ppix,_ppix + size);
	u8 *p = (u8*)_ppix;

	u32 *p2 = new u32[size];
	memset(p2, 0, sizeof(u32) * size);
	
	for(s32 i = 0; i < size; ++i)
	{	
		p2[i] = _ppattle[p[i]];
	}

	delete[] _ppix;
	_ppix = (u8*)p2;
	return;
}

void Bitmap::change4To32Idx(void)
{
	s32 size = _bih.w * _bih.h;
	//std::reverse(_ppix, _ppix + size);
	u8 *p = (u8*)_ppix;

	u32 *p2 = new u32[size];
	memset(p2, 0, sizeof(u32) * size);
	
	u8 mask = 0xf0;
	u8 tm = 0;
	u8 move = 0;

	int idx = 0;

	u8 *tp = p;
	{
		for(s32 i = 0; i < size; ++i)
		{	
			tm = mask;
			tm >>= move? 4 : 0;

			idx = (*tp) & tm;
			idx >>= move? 0 : 4;
			
			p2[i] = _ppattle[idx];

			if(((i + 1) % _bih.w))
			{
				move = (move + 1) % 2;
				if(!move)
				{
					++tp;
				}
			}
			else
			{
				if(!move)
				{
					++tp;
					move = 0;
				}
			}
		}
	}

	delete[] _ppix;
	std::reverse(p2,p2 + size);
	_ppix = (u8*)p2;
	return;

}

void Bitmap::change1To32Idx(void)
{
	s32 size =  _bih.w * _bih.h;
	u8 *p = (u8*)_ppix;

	u32 *p2 = new u32[size];
	memset(p2, 0, sizeof(u32) * size);
	
	u8 mask = 0x80;
	u8 tm = 0;
	u8 move = 0;

	u8 *tp = p;
	{
		for(s32 i = 0; i < size; ++i)
		{	
			tm = mask;
			tm >>= move;
			p2[i] = _ppattle[((*tp)& tm) / tm];

			if(((i + 1) % _bih.w))
			{
				move = (move + 1) % 8;
				if(!move)
				{
					++tp;
				}
			}
			else
			{
				++tp;
				move = 0;
			}
		}
	}

	delete[] _ppix;
	std::reverse(p2,p2 + size);
	_ppix = (u8*)p2;
	return;
}

#include <iostream>

void Bitmap::reverseRow(u32 *ppix) const
{
	for(s32 i = 0; i < _bih.h; ++i)
	{
		std::reverse(ppix + i * _bih.w, ppix + i * _bih.w + _bih.w);
	}
}


bool Bitmap::write(const std::string& fname)
{
	std::ofstream ofs(fname.c_str(), std::ios_base::out | std::ios_base::binary);
	ofs.write((char*)&_bfh, sizeof(BitmapFileHeader));
	ofs.write((char*)&_bih, sizeof(BitmapInfoHeader));

	if(_pattlesize)
	{
		if(_ppattle)
		{
			ofs.write((char*)_ppattle, _pattlesize);
		}
		else
		{
			return false;
		}
	}
	bool bwrite = saveBitmap(ofs);
	ofs.close();
	return bwrite;
}
//
//bool Bitmap::saveBitmap(std::ofstream& ofs)
//{
//	s32 size = _bihOut.w * _bihOut.h;
//	s32 nlfill = (_bihOut.w * _bihOut.h * _bihOut.bitCount / 8) % 4;
//
//	/*s32 size = (_bih.w * _bih.h * _bih.bitCount + 8 - 1) / 8;
//	s32 rowsize = (size  + _bih.h - 1) / _bih.h;
//
//	s32 nlast = (_bih.w * _bih.bitCount + 8 - 1) / 8 % 4;
//	s32 nlfill = nlast? 4 - nlast : nlast;*/
//
//
//	u8 *psave = new u8[size * _bihOut.bitCount / 8 + _bihOut.h * nlfill];
//	if(!psave)
//	{
//		return false;
//	}
//	memset(psave, 0, sizeof(u8) * size * _bihOut.bitCount / 8);
//
//	u32 *ppix = new u32[size];
//	if(!ppix)
//	{
//		return false;
//	}
//	memcpy(ppix, _ppix, sizeof(u32) * size);
//
//	reverseRow((u32*)ppix);
//	std::reverse((u32*)ppix, (u32*)ppix + size);
//
//	changeBits(psave, (u8*)ppix);
//
//	s32 rowsize = _bihOut.w * _bihOut.bitCount / 8;
//	
//	u8* pfill = new u8[nlfill];
//	memset(pfill, 0, nlfill);
//	for(s32 i = 0; i < _bih.h; ++i)
//	{
//		ofs.write((char*)(psave + i * rowsize), rowsize);
//		ofs.write((char*)pfill, nlfill);
//	}
//
//	u16 end = 0;
//	ofs.write((char*)&end, sizeof(u16));
//	delete[] psave;
//	delete[] ppix;
//	delete[] pfill;
//	return true;
//}


bool Bitmap::saveBitmap(std::ofstream& ofs)
{
	s32 size = _bihOut.w * _bihOut.h;
	s32 rowsize = (((_bihOut.w * _bihOut.h * _bihOut.bitCount + 8 - 1) / 8)  + _bihOut.h - 1) / _bihOut.h;

	s32 nlast = (_bihOut.w * _bihOut.bitCount + 8 - 1) / 8 % 4;
	s32 nlfill = nlast? 4 - nlast : nlast;

	//u8 *psave = new u8[size * _bihOut.bitCount / 8 + _bihOut.h * nlfill];
	u8 *psave = new u8[(rowsize  + nlfill) * _bihOut.h];
	if(!psave)
	{
		return false;
	}
	//memset(psave, 0, sizeof(u8) * size * _bihOut.bitCount / 8);
	memset(psave, 0, sizeof(u8) * (rowsize  + nlfill) * _bihOut.h);

	u32 *ppix = new u32[size];
	if(!ppix)
	{
		return false;
	}
	memcpy(ppix, _ppix, sizeof(u32) * size);

	reverseRow((u32*)ppix);
	std::reverse((u32*)ppix, (u32*)ppix + size);

	changeBits(psave, (u8*)ppix);
	
	u8* pfill = new u8[nlfill];
	memset(pfill, 0, nlfill);
	for(s32 i = 0; i < _bih.h; ++i)
	{
		ofs.write((char*)(psave + i * rowsize), rowsize);
		ofs.write((char*)pfill, nlfill);
	}

	u16 end = 0;

	//if(!nlfill)
	//{
		ofs.write((char*)&end, sizeof(u16));
	//}
	delete[] psave;
	delete[] ppix;
	delete[] pfill;
	return true;
}

void Bitmap::changeBits(u8* pdst, const u8* psrc) const
{
	if(!_bidx)
	{
		if(_nowbits == 32 && _bihOut.bitCount == 32)
		{
			change32To32(pdst, psrc);
			return;
		}

		if(_nowbits == 32 && _bihOut.bitCount == 24)
		{
			change32To24(pdst, psrc);
			return;
		}

		if(_nowbits == 32 && _bihOut.bitCount == 16)
		{
			change32To16(pdst, psrc);
			return;
		}
	}
	else
	{
		if(_nowbits == 32 && _bihOut.bitCount == 16)
		{
			change32To16Idx(pdst, psrc);
			return;
		}

		if(_nowbits == 32 && _bihOut.bitCount == 8)
		{
			change32To8Idx(pdst, psrc);
			return;
		}

		if(_nowbits == 32 && _bihOut.bitCount == 4)
		{
			change32To4Idx(pdst, psrc);
			return;
		}

		if(_nowbits == 32 && _bihOut.bitCount == 1)
		{
			change32To1Idx(pdst, psrc);
			return;
		}
	}
}

void Bitmap::change32To32(u8* pdst, const u8* psrc) const
{
	memcpy(pdst, psrc, _bih.w * _bih.h * _bih.bitCount / 8);
}

void Bitmap::change32To24(u8* pdst, const u8* psrc) const
{
	s32 size = _bih.w * _bih.h;

	u8* tpdst = pdst;
	const u8* tpsrc = psrc;

	for(s32 i = 0; i < size; ++i)
	{
		tpdst = pdst + i * 3;
		tpsrc = psrc + i * 4;

		tpdst[0] = tpsrc[0];
		tpdst[1] = tpsrc[1];
		tpdst[2] = tpsrc[2];
	}
}

void Bitmap::change32To16(u8* pdst, const u8* psrc) const
{
	s32 size = _bih.w * _bih.h;
	
	u16 r16 = 0, g16 = 0, b16 = 0;
	const u8 *tp = 0;
	for(s32 i = 0; i < size; ++i)
	{
		tp = psrc + i * 4;
		b16 = (float)tp[0] / 255.0f * 32.0f;
		g16 = (float)tp[1] / 255.0f * 32.0f;
		g16 <<= 5;
		r16 = (float)tp[2] / 255.0f * 32.0f;
		r16 <<= 10;

		*((u16*)pdst +i) = r16 | g16 | b16;
	}
}

void Bitmap::change32To16Idx(u8* pdst, const u8* psrc) const
{
	s32 size = _bih.w * _bih.h;

	const u8* tp = 0;
	u16 r = 0, g = 0, b = 0;
	for(s32 i = 0; i < size; ++i)
	{
		tp = psrc + i * 4;
		b = (float)tp[0] / 255.0f * _ppattle[2] + 0.5f;
		g = (float)tp[1] / 255.0f * _ppattle[1] + 0.5f;
		r = (float)tp[2] / 255.0f * _ppattle[0] + 0.5f;

		*((u16*)pdst + i) = r | g | b;
 	}
}

void Bitmap::change32To8Idx(u8* pdst, const u8* psrc) const
{
	s32 size = _bih.w * _bih.h;

	const u32* tp = (u32*)psrc;
	u8 idx = 0;
	for(int i = 0; i < size; ++i)
	{
		idx = std::find(_ppattle, _ppattle + _pattlesize, tp[i]) - _ppattle;

		if(idx >= _pattlesize)
		{
			return;
		}

		pdst[i] = idx;
	}
}

void Bitmap::change32To4Idx(u8* pdst, const u8* psrc) const
{
	s32 size = _bih.w * _bih.h;

	const u32* tpsrc = (u32*)psrc;
	u8 idx = 0;

	u8 move = 0;
	u8* tp = pdst;

	int iii = 0;
	for(int i = 0; i < size; ++i)
	{
		idx = std::find(_ppattle, _ppattle + _pattlesize, tpsrc[i]) - _ppattle;

		if(idx >= _pattlesize)
		{
			return;
		}

		idx <<= 4;
		/*u8 kk =  (idx >> (move * 4));

		u8 kkk = *tp;*/
		(*tp) |= (idx >> (move * 4));
		if(!((i + 1) % 15))
		{
			int tttt = 0;
		}

	/*	if(*tp > 0)
		{
			int nnn = 0;
		}*/
		if((i + 1) % _bihOut.w)
		{
			move = (move + 1) % 2;
			if(!move)
			{
				++tp;
				++iii;
			}
		}
		else
		{
			if(!move)
			{
				move = 0;
				++tp;
				++iii;
			}
		}
	}
}

void Bitmap::change32To1Idx(u8* pdst, const u8* psrc) const
{
	s32 size = _bih.w * _bih.h;

	const u32* tpsrc = (u32*)psrc;
	u8 idx = 0;

	u8 move = 0;
	u8* tp = pdst;
	for(int i = 0; i < size; ++i)
	{
		idx = std::find(_ppattle, _ppattle + _pattlesize, tpsrc[i]) - _ppattle;

		if(idx >= _pattlesize)
		{
			return;
		}

		idx <<= 7;
		(*tp) |= (idx >> move);
		if((i + 1) % _bihOut.w)
		{
			move = (move + 1) % 8;
			if(!move)
			{
				++tp;
			}
		}
		else
		{
			move = 0;
			++tp;
		}
	}
}