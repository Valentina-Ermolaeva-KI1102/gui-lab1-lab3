#include "kr4.h"

kr4::kr4(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	key = new char[50];
	key_ceaser = 0;
	mode = 1;
	int_valid = new QIntValidator();
	ui.KeyEdit->setValidator(int_valid);
	ui.CheckMode->addItems(QStringList() << "Ceaser" << "Viginer" << "Steganography" << "GOST");
	ui.CheckMP3_but->setEnabled(false);
	ui.MusFileEdit->setEnabled(false);
	ui.groupBox_2->setEnabled(false);
	connect(ui.CheckMode, SIGNAL(currentIndexChanged(int)), this, SLOT(check_MODE(int)));
	connect(ui.CheckIN_but, SIGNAL(clicked()), this, SLOT(check_IN()));
	connect(ui.CheckMP3_but, SIGNAL(clicked()), this, SLOT(check_MP3()));
	connect(ui.Encrip_but, SIGNAL(clicked()), this, SLOT(encript()));
	connect(ui.Decrip_but, SIGNAL(clicked()), this, SLOT(decript()));
}

void kr4::check_IN()
{
	if(mode < 3)
		in_fname = QFileDialog::getOpenFileName(this, tr("Open Input File"), "C:\ ", tr("Input Files (*.txt)"));
	else
		in_fname = QFileDialog::getOpenFileName(this, tr("Open Input File"), "C:\ ", tr("Input Files (*.*)"));
	ui.InFileEdit->setText(in_fname);
}

void kr4::check_MP3()
{
	mp3_fname = QFileDialog::getOpenFileName(this, tr("Open MP3 File"), "C:\ ", tr("MP3 Files (*.mp3)"));
	ui.MusFileEdit->setText(mp3_fname);
}

void kr4::check_OUT()
{
	out_fname = ui.OutFileEdit->text();
	ui.OutFileEdit->clear();
}

void kr4::check_MODE(int a)
{
	mode = a + 1;
	ui.KeyEdit->setText("");
	switch(mode)
	{
	case 1:
		ui.KeyEdit->setEnabled(true);
		ui.KeyEdit->setValidator(int_valid);
		ui.CheckMP3_but->setEnabled(false);
		ui.MusFileEdit->setEnabled(false);
		break;
	case 2:
		ui.KeyEdit->setEnabled(true);
		ui.KeyEdit->setValidator(NULL);
		ui.CheckMP3_but->setEnabled(false);
		ui.MusFileEdit->setEnabled(false);
		break;
	case 3:
		ui.KeyEdit->setEnabled(false);
		ui.CheckMP3_but->setEnabled(true);
		ui.MusFileEdit->setEnabled(true);
		break;
	case 4:
		ui.CheckMP3_but->setEnabled(false);
		ui.MusFileEdit->setEnabled(false);
		ui.KeyEdit->setEnabled(false);
		break;
	}
}

void kr4::check_KEY()
{
	switch(mode)
	{
	case 1:
		key_ceaser = ui.KeyEdit->text().toInt()%255;
		break;
	case 2:
		key = ui.KeyEdit->text();
	default:
		break;
	}
}

void kr4::encript()
{
	switch(mode)
	{
	case 1:
		sh_Ceaser();
		break;
	case 2:
		sh_Vegener();
		break;
	case 3:
		sh_Stegan();
		break;
	case 4:
		dsh_GOST(true);
		break;
	}
}

void kr4::decript()
{
	switch(mode)
	{
	case 1:
		ds_Ceaser();
		break;
	case 2:
		ds_Vegener();
		break;
	case 3:
		ds_Stegan();
		break;
	case 4:
		dsh_GOST(false);
		break;
	}
}

///////////////////////////////////////////

void kr4::sh_Ceaser()
{	
	check_OUT();
	check_KEY();
	if(in_fname == "" || out_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inFile.open(in_fname.toLatin1(), std::ios_base::in);
	inFile.seekg(std::ios::beg);
	outFile.open(out_fname.toLatin1(), std::ios_base::out);
	inFile.get(buf);
	while (!inFile.eof())
	{
		buf += key_ceaser;
		outFile<<buf;
		inFile.get(buf);
	}
	inFile.clear();
	outFile.clear();
	inFile.close();
	outFile.close();
}

void kr4::ds_Ceaser()
{
	check_OUT();
	check_KEY();
	if(in_fname == "" || out_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inFile.open(in_fname.toLatin1(), std::ios_base::in);
	inFile.seekg(std::ios::beg);
	outFile.open(out_fname.toLatin1(), std::ios_base::out);
	inFile.get(buf);
	while (!inFile.eof())
	{
		buf -= key_ceaser;
		outFile<<buf;
		inFile.get(buf);
	}
	inFile.clear();
	outFile.clear();
	inFile.close();
	outFile.close();
}

///////////////////////////////////////////
void kr4::sh_Vegener()
{
	check_OUT();
	check_KEY();
	if(in_fname == "" || out_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	if(key == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown keyword."), QMessageBox::Ok);
		return;
	}
	int i = 0;
	const char *a = in_fname.toUtf8();
	inFile.open(in_fname.toLatin1(), std::ios_base::in);
	inFile.seekg(std::ios::beg);
	outFile.open(out_fname.toLatin1(), std::ios_base::out);
	inFile.get(buf);
	while (!inFile.eof())
	{
		buf += key.toLatin1().data()[i];
		outFile<<buf;
		inFile.get(buf);
		i++;
		if(i > key.length())
			i = 0;
	}
	inFile.clear();
	outFile.clear();
	inFile.close();
	outFile.close();
}

void kr4::ds_Vegener()
{
	check_OUT();
	check_KEY();
	if(in_fname == "" || out_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	if(key == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown keyword."), QMessageBox::Ok);
		return;
	}
	inFile.open(in_fname.toLatin1(), std::ios_base::in);
	inFile.seekg(std::ios::beg);
	outFile.open(out_fname.toLatin1(), std::ios_base::out);
	int i=0;
	inFile.get(buf);
	while(!inFile.eof())
	{
		buf -= key.toLatin1().data()[i];
		outFile<<buf;
		inFile.get(buf);
		i++;
		if(i > key.length())
			i = 0;
	}
	inFile.clear();
	outFile.clear();
	inFile.close();
	outFile.close();
}

//////////////////////////////////////////

void kr4::sh_Stegan()
{
	check_OUT();
	if(in_fname == "" || out_fname == "" || mp3_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inFile.open(in_fname.toLatin1(), std::ios_base::in);
	inFile.seekg(std::ios::beg);
	inMP3.open(mp3_fname.toLatin1(), std::ios_base::in | std::ios_base::binary);
	outFile.open(out_fname.toLatin1(), std::ios_base::binary | std::ios_base::out);
	inFile.seekg(0, std::ios_base::end);
	long in_size = inFile.tellg();
	inFile.seekg(0, std::ios_base::beg);
	in_size -= inFile.tellg();
	inMP3.seekg(0, std::ios_base::end);
	long mp3_size = inMP3.tellg();
	inMP3.seekg(0, std::ios_base::beg);
	mp3_size -= inMP3.tellg();
	if(in_size*4 + 64 > mp3_size)
	{
		QMessageBox::warning(this, tr("Ошибка"), tr("MP3 файл слишком мал."), QMessageBox::Ok);
		return;
	}
	unsigned char Mask[5] = {3, 12, 48, 192, 252};
	char buf_work, *content = (char*)malloc(64);
	int l = 0;
	inMP3.read(content, 64);
	outFile.write(content, 64);
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
			inMP3.read(&buf_mp3, 1);
			buf_mp3 &= Mask[4];
			buf_mp3 |= buf;
			outFile.write(&buf_mp3, 1);
		}
	}
	int k = outFile.tellp();
	while(l < in_size)
	{
		inFile.get(buf);
		for(int i=0; i<4; i++)
		{
			buf_work = buf & Mask[i];
			for(int j=0; j < i; j++)
				buf_work>>=2;
			inMP3.read(&buf_mp3, 1);
			buf_mp3 &= Mask[4];
			buf_mp3 |= buf_work;
			outFile.write(&buf_mp3, 1);
		}
		l++;
	}
	k = outFile.tellp();
	content = new char[mp3_size - (in_size*4 + 64)];
	inMP3.read(content, mp3_size - (in_size*4 + 64));
	outFile.write(content, mp3_size - (in_size*4 + 64));
	free(content);
	inFile.clear();
	inMP3.clear();
	outFile.clear();
	inFile.close();
	inMP3.close();
	outFile.close();
}

void kr4::ds_Stegan()
{
	check_OUT();
	if(out_fname == "" || mp3_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inMP3.open(mp3_fname.toLatin1(), std::ios_base::binary | std::ios_base::in);
	outFile.open(out_fname.toLatin1(), std::ios_base::out);
	unsigned char Mask[5] = {3, 12, 48, 192, 252}, charTxt[4], buf_work;
	int lenght = 0, bufLen, len = 0;
	inMP3.seekg(64, std::ios_base::beg);
	for(int i=0; i<4; i++)
	{
		charTxt[i] = 0;
		for(int j=0; j<4; j++)
		{
			inMP3.read(&buf, 1);
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
			inMP3.read(&buf_mp3, 1);
			buf_work = buf_mp3 & Mask[0];
			for(int k=0; k < j; k++)
				buf_work <<= 2;
			buf |=buf_work;
		}
		len++;
		outFile<<buf;
	}
	inMP3.clear();
	outFile.clear();
	inMP3.close();
	outFile.close();
}

/////////////////////////////////////////

long kr4::filesize()
{
	long curpos, length;
	curpos = inFile.tellg();
	inFile.seekg(0, std::ios_base::end);
	length = inFile.tellg();
	inFile.seekg(curpos, std::ios_base::beg);
	return length;
}

// функция, реализующая работу ГОСТ 28147-89 в режиме простой замены
void kr4::dsh_GOST(bool Mode)
{
	check_OUT();
	if(in_fname == "" || out_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inFile.open(in_fname.toLatin1(), std::ios_base::binary | std::ios_base::in);
	outFile.open(out_fname.toLatin1(), std::ios_base::binary | std::ios_base::out);

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

	unsigned long key[8] =
	{
		0x0123,
		0x4567,
		0x89AB,
		0xCDEF,
		0x0123,
		0x4567,
		0x89AB,
		0xCDEF
	};

	char N[4]; // 32-разрядный накопитель,

	unsigned long N1=0, N2=0, SUM232=0; // накопители N1, N2, и сумматор

	// количество блоков
	float count_bloks;
	count_bloks = 8*filesize()/64.0;
	int block = count_bloks;
	if (count_bloks-block>0) 
		block++;

	int sh;
	if (filesize()>=4) 
		sh = 4; 
	else sh = filesize();
	int sh1 = 0;
	int end_file = 0;

	// считывание и преобразование блоков
	for (int i=0; i<block; i++)
	{
		// записываем в накопитель N1
		for (int q=0; q<4; q++) *((unsigned char*)&N+q) = 0x00;
		if ((sh1+sh)<filesize())
		{
			inFile.read(N, sh);
			sh1+=sh;
		}
		else
		{
			sh=filesize()-sh1;
			inFile.read(N, sh);
			end_file = 1;
		}
		N1 = *((unsigned long *)&N);
		// записываем в накопитель N2
		for (int q=0; q<4; q++) *((unsigned char*)&N+q) = 0x00;
		if ((sh1+sh)<filesize())
		{
			inFile.read(N, sh);
			sh1+=sh;
		}
		else
		{
			if (end_file == 0)
			{
				sh=filesize()-sh1;
				inFile.read(N, sh);
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
			outFile.write(&buf, sizeof(char));
		}
		for (int q=0; q<4; q++)
		{
			buf = *((unsigned char*)&N2+q);
			outFile.write(&buf, sizeof(char));
		}
	}
	inFile.clear();
	outFile.clear();
	inFile.close();
	outFile.close();
}

/////////////////////////////////////////

kr4::~kr4()
{

}
