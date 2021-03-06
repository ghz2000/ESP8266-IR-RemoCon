#include <Arduino.h>
#include <FS.h>

#ifndef __CiniParser__
#define __CiniParser__

#define DIR_LENGTH 15
#define READ 0
#define WRITE 1

class CiniParser
{

private:
  char iniFileName[DIR_LENGTH];
  char tmpFileName[DIR_LENGTH];

  int openFile(File *file, char *openMode);
  int openFile(File *file, char *openMode, char *fnm);
  int searchSection(File *file, String section);
  int getValue(File *file, unsigned long *startPos, unsigned long *endPos, String paraName, String *paraValue);

  int copyFile(char *srcFnm, char *dstFnm, unsigned long startPos = 0, unsigned long endPos = 0);
  int writeLine(char *dstFnm, String line);

  int copyIniFile(unsigned long startPos = 0, unsigned long endPos = 0);

public:
  int  setIniFileName(char *fnm);

  int  createIniFile();
  int  deleteIniFile();
  int  writeIniFile(String *data);
  void readIniFile(String *res);
  int  getIniFileSize();

  // Section, paraName, paraValue を確認する
  // 3: paraName まで一致があれば その行を作り直してコピー
  // 2: Section  まで一致があれば そのSectionの最後に追記コピー
  // 1: Section  も存在しなければ､ファイルの最後に追記コピー
  // 0: ファイルが存在しない
  int rwIni(String section, String paraName, String *paraValue, int rw);

};

#endif __CiniParser__
