#include <time.h>


#include "AdvancedOTA.h"
#include "CiniParser.h"
#include "pageRoot.h"
#include "pageSetting.h"
#include "CirRemote.h"

#define INIFNM "/config.ini"

//X がプルダウンされている
#define X1 14
#define X2 12
#define X3 13
#define X4 16
#define Y1 4
#define Y2 5
#define Y3 3  //TXD

#define irLed 15

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

CirRemote ir(irLed);



void setup() {
  Serial.begin(115200);
//  Serial.end();


  digitalWrite(Y1, HIGH);
  digitalWrite(Y2, HIGH);
  digitalWrite(Y3, HIGH); //TXD
  pinMode(Y1, OUTPUT);
  pinMode(Y2, OUTPUT);
  pinMode(Y3, OUTPUT);    //TXD

  pinMode(X1, INPUT);
  pinMode(X2, INPUT);
  pinMode(X3, INPUT);
  pinMode(X4, INPUT);

  if(initialize()){
    //されていなかったら
//    setServoParam(false);
  }

  wifiSetup(&WiFiMulti, &server);
  server.on("/", handleRoot);
  server.on("/setting", handleSetting);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/turn", handleTurn);

//--- ini File Sample ---
  server.on("/ini", handleIniSample);
  server.on("/writeini", handleWriteIni);
  server.on("/setini", handleSetIni);

  //for debug
  server.on("/procini", handleProcIni);
  server.on("/delini", handleDelIni);


  //NTP設定
  configTime( 3600*9 , 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");

}


void loop() {
  listener();

  if(digitalRead(X1) || digitalRead(X2) || digitalRead(X3) || digitalRead(X4) ){
    checkBtn();
  }

  delay(100);
}

void sendMitsubishi(char *data, int len){
    ir.irOut(data, len);
    delayMicroseconds(27000);
    ir.irOut(data, len);
    delayMicroseconds(27000);
    ir.irOut(data, len);
    delayMicroseconds(27000);
    ir.irOut(data, len);
    delayMicroseconds(27000);
}

void checkBtn(){
  digitalWrite(Y1, HIGH);
  digitalWrite(Y2, LOW);
  digitalWrite(Y3, LOW);

  if(digitalRead(X1) == HIGH){
    //電源ボタン
//    Serial.print("Y1:X1");
    sendMitsubishi("E2400", 17);
  }
  if(digitalRead(X2) == HIGH){
      //ch6
//    Serial.print("Y1:X2");
    sendMitsubishi("E2140", 17);
  }
  if(digitalRead(X3) == HIGH){
      //ch3
//    Serial.print("Y1:X3");
    sendMitsubishi("E2080", 17);
  }
  if(digitalRead(X4) == HIGH){
      //ch4
//    Serial.print("Y1:X4");
    sendMitsubishi("E2180", 17);
  }


  digitalWrite(Y1, LOW);
  digitalWrite(Y2, HIGH);
  digitalWrite(Y3, LOW);

  if(digitalRead(X1) == HIGH){
      //ch5
//    Serial.print("Y2:X1");
    sendMitsubishi("E2040", 17);
  }
  if(digitalRead(X2) == HIGH){
      //音量下
//    Serial.print("Y2:X2");
    sendMitsubishi("E2540", 17);
  }
  if(digitalRead(X3) == HIGH){
      //ch8
//    Serial.print("Y2:X3");
    sendMitsubishi("E21C0", 17);
  }
  if(digitalRead(X4) == HIGH){
      //ch10
//    Serial.print("Y2:X4");
    sendMitsubishi("E2900", 17);
  }

  digitalWrite(Y1, LOW);
  digitalWrite(Y2, LOW);
  digitalWrite(Y3, HIGH);

  if(digitalRead(X1) == HIGH){
      //ch1
//    Serial.print("Y3:X1");
    sendMitsubishi("E2000", 17);
  }
  if(digitalRead(X2) == HIGH){
      //ch2
//    Serial.print("Y3:X2");
    sendMitsubishi("E2100", 17);
  }
  if(digitalRead(X3) == HIGH){
      //音量上
//    Serial.print("Y3:X3");
    sendMitsubishi("E2440", 17);
  }
  if(digitalRead(X4) == HIGH){
//    Serial.print("Y3:X4");
  }



  digitalWrite(Y1, HIGH);
  digitalWrite(Y2, HIGH);
  digitalWrite(Y3, HIGH);
}



//- handle for HTTP ------------------------------------------------------------------------

// http://esp8266.local/
// ↑ここにアクセス

void handleRoot() {
  server.send(200, "text/html", ROOT_HTML );
}

void handleSetting() {
  server.send(200, "text/html", SETTING_HTML );
}

void handleOn() {
  swOn();
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleOff() {
  swOff();
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleTurn(){
  swTurn();
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

//- Control Servo ------------------------------------------------------------------------

// 取得完了 : 0
// 取得失敗 : 1
int initialize(){
  ////// ini Update
  CiniParser ini;
  String strMinimum, strMaximum;
  int res1=0,res2=0;

  if(ini.setIniFileName( INIFNM )){
    //Serial.println("File not exist");
  }

  res1 = ini.rwIni("KabeCon", "minimum1", &strMinimum, READ);
  res2 = ini.rwIni("KabeCon", "maximum1", &strMaximum, READ);

  if(res1 == 3 && res2 == 3){
//    minimum = strMinimum.toInt();
//    maximum = strMaximum.toInt();
    return 0;
  }
  return 1;
}

void setServoParam(bool wifiSetup) {

  ////// ini Update
  if(0){
    CiniParser ini;
    String strMinimum;
    String strMaximum;
    if(ini.setIniFileName( INIFNM )){
      //Serial.println("File not exist");
    }
    ini.rwIni("KabeCon", "minimum1", &strMinimum, WRITE);
    ini.rwIni("KabeCon", "maximum1", &strMaximum, WRITE);
  }

}

void swOn() {

}

void swOff() {
}

void swTurn(){

}


//- ini File Access Sample ------------------------------------------------------------------------

void handleIniSample(){
  String message = "<HTML><BODY>";

  CiniParser testini;
  if(testini.setIniFileName( INIFNM )){
    message += "File not exist";
    message += testini.createIniFile();
  }

  message += "<CENTER><FONT SIZE=5>";
  message += "ini Test sample page</FONT></CENTER><BR>";


//SPIFFS 情報表示
  message += "<B>SPIFFS information </B><BR>";
  SPIFFS.begin();
  FSInfo fsinfo;
  SPIFFS.info(fsinfo);
  message += "<TABLE border=1>";
  message += "<TR><TD>totalBytes</TD><TD>";
  message += fsinfo.totalBytes;
  message += "</TD></TR>";
  message += "<TR><TD>usedBytes</TD><TD>";
  message += fsinfo.usedBytes;
  message += "</TD></TR>";
  message += "<TR><TD>blockSize</TD><TD>";
  message += fsinfo.blockSize;
  message += "</TD></TR>";
  message += "<TR><TD>pageSize</TD><TD>";
  message += fsinfo.pageSize;
  message += "</TD></TR>";
  message += "<TR><TD>maxOpenFiles</TD><TD>";
  message += fsinfo.maxOpenFiles;
  message += "</TD></TR>";
  message += "<TR><TD>maxPathLength</TD><TD>";
  message += fsinfo.maxPathLength;
  message += "</TD></TR>";
  message += "<TR><TD>use rate</TD><TD>";
  message += (fsinfo.usedBytes / fsinfo.totalBytes * 100.00);
  message += "%";
  message += "</TD></TR>";
  message += "</TABLE><BR><BR>";


//ファイル一覧を表示
  message += "<B>Directry information </B><BR>";
  message += "<TABLE border=1>";
  Dir dir = SPIFFS.openDir("/");
  while ( dir.next() ){
    message += "<TR><TD>";
    message += dir.fileName();
    message += "</TD><TD>";
    File f = dir.openFile("r");
    message += f.size();
    message += "</TD></TR>";
    f.close();
  }
  message += "</TABLE><BR><BR>";


//iniファイルの情報表示
  message += "<B>ini information </B><BR>";
  message += "fileName: ";
  message += INIFNM;
  message += "<BR>filesize: ";
  message += testini.getIniFileSize();
  message += " byte<BR><BR>";

  String sect = server.arg("Section");
  String name = server.arg("Name");
  String valu;

  if(sect.length()*name.length()){
    testini.rwIni(sect, name, &valu, READ);
  }

//ini ファイル書き込み
  message += "<B>write ini test </B><BR>";
  message += "<FORM method=POST action=/writeini>";
  message += "Section: <INPUT type=text name='Section' value='";
  message += sect;
  message += "'>Name: <INPUT type=text name='Name' value='";
  message += name;
  message += "'>Value: <INPUT type=text name='Value' value='";
  message += valu;
  message += "'><INPUT type=submit value='Write'></FORM>";


//ini ファイル取得
  message += "<B>read ini test </B><BR>";
  message += "<FORM method=POST action=/ini>";
  message += "Section: <INPUT type=text name='Section' value='";
  message += sect;
  message += "'>Name: <INPUT type=text name='Name' value='";
  message += name;
  message += "'>Value: <INPUT type=text name='Value' value='";
  message += valu;
  message += "'disabled='disabled'><INPUT type=submit value='Read'></FORM>";


//ini ファイルの内容を表示
  message += "<B>ini file viewer </B><BR>";
  message += "<FORM method=POST action=/setini><TEXTAREA NAME=ini cols=100 rows=40>";
  testini.readIniFile(&message);
  message += "</TEXTAREA>";
  message += "<INPUT type=submit value='submit'></FORM><BR>";


//WiFi ini 設定
  message += "<B>WiFi ini Setting</B><BR>";
  message += "<FORM method=POST action=/writeini>";
  message += "Section: <INPUT type=text name='Section' value='WiFi' disabled='disabled'>";
  message += "Name: <INPUT type=text name='Name' value='SSID' disabled='disabled'>";
  message += "<INPUT type=hidden name='Section' value='WiFi'>";
  message += "<INPUT type=hidden name='Name' value='SSID' >";
  message += "Value: <INPUT type=text name='Value'>";
  message += "<INPUT type=submit value='Update SSID'></FORM>";
  message += "<FORM method=POST action=/writeini>";
  message += "Section: <INPUT type=text name='Section' value='WiFi' disabled='disabled'>";
  message += "Name: <INPUT type=text name='Name' value='PASS' disabled='disabled'>";
  message += "<INPUT type=hidden name='Section' value='WiFi'>";
  message += "<INPUT type=hidden name='Name' value='PASS' >";
  message += "Value: <INPUT type=text name='Value'>";
  message += "<INPUT type=submit value='Update PASS'></FORM>";

  message += "</BODY></HTML>";
  server.send(200, "text/html", message);
}


void handleWriteIni(){
  CiniParser testini;
  if(testini.setIniFileName( INIFNM )){
    Serial.println( "File not exist");
    handleIniSample();
    return;
  }

  String sect = server.arg("Section");
  String name = server.arg("Name");
  String valu = server.arg("Value");

  if(sect.length()*name.length()){
    testini.rwIni(sect, name, &valu, WRITE);
  }

  server.sendHeader("Location", "/ini", true);
  server.send(301, "text/plain", "");
}

void handleSetIni(){
  String ini = server.arg("ini");

  CiniParser testini;
  if(testini.setIniFileName( INIFNM )){
    Serial.println("File not exist");
  }else{
    testini.writeIniFile(&ini);
  }

  server.sendHeader("Location", "/ini", true);
  server.send(301, "text/plain", "");
}


void handleProcIni(){
  String message = "<HTML><BODY>";

  SPIFFS.begin();

  FSInfo fsinfo;
  SPIFFS.info(fsinfo);

  message += "<TABLE border=1>";
  message += "<TR><TD>totalBytes</TD><TD>";
  message += fsinfo.totalBytes;
  message += "</TD></TR>";
  message += "<TR><TD>usedBytes</TD><TD>";
  message += fsinfo.usedBytes;
  message += "</TD></TR>";
  message += "<TR><TD>blockSize</TD><TD>";
  message += fsinfo.blockSize;
  message += "</TD></TR>";
  message += "<TR><TD>pageSize</TD><TD>";
  message += fsinfo.pageSize;
  message += "</TD></TR>";
  message += "<TR><TD>maxOpenFiles</TD><TD>";
  message += fsinfo.maxOpenFiles;
  message += "</TD></TR>";
  message += "<TR><TD>maxPathLength</TD><TD>";
  message += fsinfo.maxPathLength;
  message += "</TD></TR>";
  message += "<TR><TD>use rate</TD><TD>";
  message += (fsinfo.usedBytes / fsinfo.totalBytes * 100.00);
  message += "%";
  message += "</TD></TR>";
  message += "</TABLE><BR><BR>";

  message += "<TABLE border=1>";
  Dir dir = SPIFFS.openDir("/");
  while ( dir.next() ){
    message += "<TR><TD>";
    message += dir.fileName();
    message += "</TD><TD>";
    File f = dir.openFile("r");
    message += f.size();
    message += "</TD></TR>";
    message += "<TR><TD>";
            CiniParser testini;
            if(testini.setIniFileName( (char*)f.name() )){
              message += "didn't open";
            }
            testini.readIniFile(&message);
    message += "</TD></TR>";
    f.close();

  }
  message += "</TABLE><BR><BR>";


  message += "<FORM method=POST action=/setini><TEXTAREA NAME=ini cols=100 rows=40>";

            CiniParser testini;
            if(testini.setIniFileName( INIFNM )){
              message += "didn't open";
            }
            testini.readIniFile(&message);

  message += "</TEXTAREA>";
  message += "<INPUT type=submit value='submit'></FORM>";

  message += "</BODY></HTML><FORM>";

  SPIFFS.end();
  server.send(200, "text/html", message);
}

void handleDelIni(){
  String message = "<HTML><BODY>";

  CiniParser testini;
  if(testini.setIniFileName( INIFNM )){
    message += "File not exist";
  }
  message += testini.deleteIniFile();

  message += "<FORM method=POST action=/setini><TEXTAREA NAME=ini cols=100 rows=40>";

  testini.readIniFile(&message);

  message += "</TEXTAREA>";
  message += "<INPUT type=submit value='submit'></FORM>";
  message += "</BODY></HTML><FORM>";
  server.send(200, "text/html", message);
}
