#include "sh.h"

void sh_Ceaser(std::ifstream *in, std::ofstream *out, char key)
{
	char buf;
	in->seekg(0, std::ios::end);
	long size = in->tellg();
	in->seekg(0, std::ios::beg);
	int i = 0;
	while(size--)
	{
		in->read(&buf, 1);
		buf += key;
		out->write(&buf, 1);
		if(size == 100)
			i++;
	}
}

void ds_Ceaser(std::ifstream *in, std::ofstream *out, char key)
{
	char buf;
	in->seekg(0, std::ios::end);
	long size = in->tellg();
	in->seekg(0, std::ios::beg);
	int i = 0;
	while(size--)
	{
		in->read(&buf, 1);
		buf -= key;
		out->write(&buf, 1);
		if(size == 47)
			i++;
	}
}

void sh_Vegener(std::ifstream *in, std::ofstream *out, QString *key)
{
	char buf;
	in->seekg(0, std::ios::end);
	long size = in->tellg();
	in->seekg(0, std::ios::beg);
	int i = 0;
	while(size--)
	{
		in->read(&buf, 1);
		buf += key->toLatin1().data()[i];
		out->write(&buf, 1);
		i++;
		if(i > key->length())
			i = 0;
	}
}

void ds_Vegener(std::ifstream *in, std::ofstream *out, QString *key)
{
	char buf;
	in->seekg(0, std::ios::end);
	long size = in->tellg();
	in->seekg(0, std::ios::beg);
	int i = 0;
	while(size--)
	{
		in->read(&buf, 1);
		buf -= key->toLatin1().data()[i];
		out->write(&buf, 1);
		i++;
		if(i > key->length())
			i = 0;
	}
}

bool sh_Stegan(std::ifstream *in, std::ifstream *mp3, std::ofstream *out)
{
	in->seekg(0, std::ios_base::end);
	long in_size = in->tellg();
	in->seekg(0, std::ios_base::beg);
	in_size -= in->tellg();
	mp3->seekg(0, std::ios_base::end);
	long mp3_size = mp3->tellg();
	mp3->seekg(0, std::ios_base::beg);
	mp3_size -= mp3->tellg();
	if(in_size*4 + 64 > mp3_size)
		return false;
	unsigned char Mask[5] = {3, 12, 48, 192, 252};
	char buf, buf_mp3, buf_work, *content = (char*)malloc(64);
	int l = 0;
	mp3->read(content, 64);
	out->write(content, 64);
	free(content);
	unsigned char lengTxt[4];
	int bufLen, mask_count = 255;
	for(int i=0; i<4; i++)
	{
		bufLen = in_size & mask_count;
		mask_count *= 256;
		for(int j=0; j<i; j++)
			bufLen>>=8;
		lengTxt[i] = bufLen;
	}
	for (int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			buf = lengTxt[i] & Mask[j];
			for(int k=0; k < j; k++)
				buf>>=2;
			mp3->read(&buf_mp3, 1);
			buf_mp3 &= Mask[4];
			buf_mp3 |= buf;
			out->write(&buf_mp3, 1);
		}
	}
	int k = out->tellp();
	while(l < in_size)
	{
		in->read(&buf, 1);
		for(int i=0; i<4; i++)
		{
			buf_work = buf & Mask[i];
			for(int j=0; j < i; j++)
				buf_work>>=2;
			mp3->read(&buf_mp3, 1);
			buf_mp3 &= Mask[4];
			buf_mp3 |= buf_work;
			out->write(&buf_mp3, 1);
		}
		l++;
	}
	k = out->tellp();
	content = new char[mp3_size - (in_size*4 + 64)];
	mp3->read(content, mp3_size - (in_size*4 + 64));
	out->write(content, mp3_size - (in_size*4 + 64));
	free(content);
	return true;
}

void ds_Stegan(std::ifstream *in, std::ofstream *out)
{
	unsigned char Mask[5] = {3, 12, 48, 192, 252}, charTxt[4], buf_work;
	char buf, buf_mp3;
	int lenght = 0, bufLen, len = 0;
	in->seekg(64, std::ios_base::beg);
	for(int i=0; i<4; i++)
	{
		charTxt[i] = 0;
		for(int j=0; j<4; j++)
		{
			in->read(&buf, 1);
			buf =  buf & Mask[0];
			for(int k=0; k < j; k++)
				buf <<= 2;
			charTxt[i] |= buf;
		}
	}
	for(int i=0; i<4; i++)
	{
		bufLen = charTxt[i];
		for(int j=0; j<i; j++)
			bufLen<<=8;
		lenght |= bufLen;
	}
	while(len < lenght)
	{
		buf = 0;
		for(int j=0; j<4; j++)
		{
			in->read(&buf_mp3, 1);
			buf_work = buf_mp3 & Mask[0];
			for(int k=0; k < j; k++)
				buf_work <<= 2;
			buf |=buf_work;
		}
		len++;
		out->write(&buf, 1);
	}
}

long filesize(std::ifstream *in)
{
	long curpos, length;
	curpos = in->tellg();
	in->seekg(0, std::ios_base::end);
	length = in->tellg();
	in->seekg(curpos, std::ios_base::beg);
	return length;
}

void dsh_GOST(std::ifstream *in, std::ofstream *out, QString *g_key, bool Mode)
{	
	//таблица замен
	unsigned char RepTable[8][16] =
	{
		0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
		0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
		0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
		0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
		0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
		0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
		0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
		0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF
	};
    unsigned long key[8];
    for(int i=0; i<8; i++)
        key[i] = g_key->toLatin1().data()[i];
	char N[4], buf; // 32-разрядный накопитель,
	unsigned long N1=0, N2=0, SUM232=0; // накопители N1, N2, и сумматор
	float count_bloks; //количество блоков
	count_bloks = 8*filesize(in)/64.0;
	int block = count_bloks;
	if (count_bloks-block>0) 
		block++;
	int sh;
	if (filesize(in)>=4) 
		sh = 4; 
	else sh = filesize(in);
	int sh1 = 0;
	int end_file = 0;

	// считывание и преобразование блоков
	for (int i=0; i<block; i++)
	{
		// записываем в накопитель N1
		for (int q=0; q<4; q++) *((unsigned char*)&N+q) = 0x00;
		if ((sh1+sh)<filesize(in))
		{
			in->read(N, sh);
			sh1+=sh;
		}
		else
		{
			sh=filesize(in)-sh1;
			in->read(N, sh);
			end_file = 1;
		}
		N1 = *((unsigned long *)&N);
		// записываем в накопитель N2
		for (int q=0; q<4; q++) *((unsigned char*)&N+q) = 0x00;
		if ((sh1+sh)<filesize(in))
		{
			in->read(N, sh);
			sh1+=sh;
		}
		else
		{
			if (end_file == 0)
			{
				sh=filesize(in)-sh1;
				in->read(N, sh);
			} 
		}
		N2 = *((unsigned long *)&N);

		// 32 цикла простой замены
		int c = 0;
		for (int k=0; k<32; k++)
		{
			if(Mode) 
			{
				if (k==24) 
					c = 7;
			}
			else if (k==8) 
				c = 7;

			// суммируем в сумматоре СМ1
            SUM232 = key[c] + N1;

			// заменяем по таблице замен
			unsigned char first_byte=0,second_byte=0,rep_symbol=0;
			int n = 7;
			for (int q=3; q>=0; q--)
			{
				rep_symbol = *((unsigned char*)&SUM232+q);
				first_byte = (rep_symbol & 0xF0) >> 4;
				second_byte = (rep_symbol & 0x0F);
				first_byte = RepTable[n][first_byte];
				n--;
				second_byte = RepTable[n][second_byte];
				n--;
				rep_symbol = (first_byte << 4) | second_byte;
				*((unsigned char*)&SUM232+q) = rep_symbol;
			} 

			SUM232 = (SUM232<<11)|(SUM232>>21); // циклический сдвиг на 11
			SUM232 = N2^SUM232; // складываем в сумматоре СМ2
			if (k<31)
			{
				N2 = N1;
				N1 = SUM232;
			}

			if (Mode)
			{
				if (k<24)
				{
					c++;
					if (c>7)
						c = 0;
				}
				else
				{
					c--;
					if (c<0)
						c = 7;
				}
			}
			else
			{
				if (k<8)
				{
					c++;
					if (c>7)
						c = 0;
				}
				else
				{
					c--;
					if (c<0)
						c = 7;
				}
			}
		}
		N2 = SUM232;
		// вывод результата в файл
		for (int q=0; q<4; q++)
		{
			buf = *((unsigned char*)&N1+q);
			out->write(&buf, sizeof(char));
		}
		for (int q=0; q<4; q++)
		{
			buf = *((unsigned char*)&N2+q);
			out->write(&buf, sizeof(char));
		}
	}
}
