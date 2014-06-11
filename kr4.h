#ifndef KR4_H
#define KR4_H

#include <QtWidgets/QWidget>
#include <QFileDialog>
#include <QIntValidator>
#include <QMessageBox>
#include "ui_kr4.h"
#include <fstream>
#include <stdlib.h>
class kr4 : public QWidget
{
	Q_OBJECT
	std::ifstream inFile, inMP3;
	std::ofstream outFile;
	QString key;
	char key_ceaser;
	char buf, buf_mp3;
	QString in_fname, out_fname, mp3_fname;
	short int mode; //1-Ceaser, 2-Viginer, 3-Stenography, 4-GOST
	QIntValidator *int_valid;

public:
	kr4(QWidget *parent = 0);
	~kr4();

private slots:
	void check_IN();
	void check_MP3();
	void check_MODE(int a);
	void encript();
	void decript();

private:
	long filesize();
	void check_OUT();
	void check_KEY();
	void sh_Ceaser();
	void ds_Ceaser();
	void sh_Vegener();
	void ds_Vegener();
	void sh_Stegan();
	void ds_Stegan();
	void dsh_GOST(bool Mode);
	Ui::kr4Class ui;
};

#endif // KR4_H
