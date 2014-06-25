#ifndef KR4_H
#define KR4_H

#include <QtWidgets/QWidget>
#include <QFileDialog>
#include <QIntValidator>
#include <QMessageBox>
#include <QEvent>
#include "ui_kr4.h"
#include <fstream>
#include <stdlib.h>
class kr4 : public QWidget
{
	Q_OBJECT
	std::ifstream *inFile, *inMP3;
	std::ofstream *outFile;
	QString key;
	char key_ceaser;
	QString in_fname, out_fname, mp3_fname;
	short int mode; //1-Ceaser, 2-Viginer, 3-Stenography, 4-GOST
	QIntValidator *int_valid;

public:
	kr4(QWidget *parent = 0);
	~kr4();

private slots:
	void check_IN();
	void check_MP3();
	void check_OUT();
	void check_MODE(int a);
	void encript();
	void decript();

private:
	void check_KEY();
	void set_Ceaser();
	void get_Ceaser();
	void set_Vegener();
	void get_Vegener();
	void set_Stegan();
	void get_Stegan();
	void set_GOST();
	void get_GOST();
	Ui::kr4Class ui;
};

#endif // KR4_H
