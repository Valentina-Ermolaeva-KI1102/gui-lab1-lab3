#include "kr4.h"
#include "sh.h"

kr4::kr4(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	key = new char[50];
	key_ceaser = 0;
	mode = 1;
	inFile = new std::ifstream;
	inMP3 = new std::ifstream;
	outFile = new std::ofstream;
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
        in_fname = QFileDialog::getOpenFileName(this, tr("Open Input File"), "C:\ "/*, tr("Input Files (*.txt)")*/);
	else
        in_fname = QFileDialog::getOpenFileName(this, tr("Open Input File"), "C:\ "/*, tr("Input Files (*.*)")*/);
	ui.InFileEdit->setText(in_fname);
}

void kr4::check_MP3()
{
    mp3_fname = QFileDialog::getOpenFileName(this, tr("Open MP3 File"), "C:\ "/*, tr("MP3 Files (*.mp3)")*/);
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
		set_Ceaser();
		break;
	case 2:
		set_Vegener();
		break;
	case 3:
		set_Stegan();
		break;
	case 4:
		set_GOST();
		break;
	}
}

void kr4::decript()
{
	switch(mode)
	{
	case 1:
		get_Ceaser();
		break;
	case 2:
		get_Vegener();
		break;
	case 3:
		get_Stegan();
		break;
	case 4:
		get_GOST();
		break;
	}
}

///////////////////////////////////////////

void kr4::set_Ceaser()
{	
	check_OUT();
	check_KEY();
	if(in_fname == "" || out_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inFile->open(in_fname.toLatin1(), std::ios_base::in | std::ios_base::binary);
	inFile->seekg(std::ios::beg);
	outFile->open(out_fname.toLatin1(), std::ios_base::out | std::ios_base::binary);
	sh_Ceaser(inFile, outFile, key_ceaser);
	inFile->clear();
	outFile->clear();
	inFile->close();
	outFile->close();
}

void kr4::get_Ceaser()
{
	check_OUT();
	check_KEY();
	if(in_fname == "" || out_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inFile->open(in_fname.toLatin1(), std::ios_base::in | std::ios_base::binary);
	inFile->seekg(std::ios::beg);
	outFile->open(out_fname.toLatin1(), std::ios_base::out | std::ios_base::binary);
	ds_Ceaser(inFile, outFile, key_ceaser);
	inFile->clear();
	outFile->clear();
	inFile->close();
	outFile->close();
}

///////////////////////////////////////////

void kr4::set_Vegener()
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
	inFile->open(in_fname.toLatin1(), std::ios_base::in | std::ios_base::binary);
	inFile->seekg(std::ios::beg);
	outFile->open(out_fname.toLatin1(), std::ios_base::out | std::ios_base::binary);
	sh_Vegener(inFile, outFile, &key);
	inFile->clear();
	outFile->clear();
	inFile->close();
	outFile->close();
}

void kr4::get_Vegener()
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
	inFile->open(in_fname.toLatin1(), std::ios_base::in | std::ios_base::binary);
	inFile->seekg(std::ios::beg);
	outFile->open(out_fname.toLatin1(), std::ios_base::out | std::ios_base::binary);
	ds_Vegener(inFile, outFile, &key);
	inFile->clear();
	outFile->clear();
	inFile->close();
	outFile->close();
}

//////////////////////////////////////////

void kr4::set_Stegan()
{
	check_OUT();
	if(in_fname == "" || out_fname == "" || mp3_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inFile->open(in_fname.toLatin1(), std::ios_base::in | std::ios_base::binary);
	inFile->seekg(std::ios::beg);
	inMP3->open(mp3_fname.toLatin1(), std::ios_base::in | std::ios_base::binary);
	outFile->open(out_fname.toLatin1(), std::ios_base::binary | std::ios_base::out);
	if(!sh_Stegan(inFile, inMP3, outFile))
		QMessageBox::warning(this, tr("Ошибка"), tr("MP3 файл слишком мал."), QMessageBox::Ok);
	inFile->clear();
	inMP3->clear();
	outFile->clear();
	inFile->close();
	inMP3->close();
	outFile->close();
}

void kr4::get_Stegan()
{
	check_OUT();
	if(out_fname == "" || mp3_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inMP3->open(mp3_fname.toLatin1(), std::ios_base::binary | std::ios_base::in);
	outFile->open(out_fname.toLatin1(), std::ios_base::out | std::ios_base::binary);
	ds_Stegan(inMP3, outFile);
	inMP3->clear();
	outFile->clear();
	inMP3->close();
	outFile->close();
}

/////////////////////////////////////////

// функция, реализующая работу ГОСТ 28147-89 в режиме простой замены
void kr4::set_GOST()
{
	check_OUT();
	if(in_fname == "" || out_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inFile->open(in_fname.toLatin1(), std::ios_base::binary | std::ios_base::in);
	outFile->open(out_fname.toLatin1(), std::ios_base::binary | std::ios_base::out);
	dsh_GOST(inFile, outFile, true);
	inFile->clear();
	outFile->clear();
	inFile->close();
	outFile->close();
}

void kr4::get_GOST()
{
	check_OUT();
	if(in_fname == "" || out_fname == "")
	{
		QMessageBox::warning(this, tr("Error"), tr("Unknown one of the files being processed."), QMessageBox::Ok);
		return;
	}
	inFile->open(in_fname.toLatin1(), std::ios_base::binary | std::ios_base::in);
	outFile->open(out_fname.toLatin1(), std::ios_base::binary | std::ios_base::out);
	dsh_GOST(inFile, outFile, false);
	inFile->clear();
	outFile->clear();
	inFile->close();
	outFile->close();
}

/////////////////////////////////////////

kr4::~kr4()
{

}
