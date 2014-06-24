#include <fstream>
#include <QString>

void sh_Ceaser(std::ifstream *in, std::ofstream *out, char key);
void ds_Ceaser(std::ifstream *in, std::ofstream *out, char key);
void sh_Vegener(std::ifstream *in, std::ofstream *out, QString *key);
void ds_Vegener(std::ifstream *in, std::ofstream *out, QString *key);
bool sh_Stegan(std::ifstream *in, std::ifstream *mp3, std::ofstream *out);
void ds_Stegan(std::ifstream *in, std::ofstream *out);
long filesize(std::ifstream *in);
void dsh_GOST(std::ifstream *in, std::ofstream *out, bool Mode);
