#ifndef __BASE_DATA_H__
#define __BASE_DATA_H__

#include "platform/PlatformMacros.h"
#include <stdint.h>
#include <string.h>

NS_BEGIN

class DLL Data
{
public:
	Data();

	Data(const Data& other);

	Data(Data&& other);

	~Data();

	Data& operator= (const Data& other);

	Data& operator= (Data&& other);

	unsigned char* getBytes() const;

	ssize_t getSize() const;


};

NS_END

#endif // !__BASE_DATA_H__
