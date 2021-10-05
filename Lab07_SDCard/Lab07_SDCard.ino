// include the SD library:
#include <SPI.h>
#include <SD.h>



File root;
File current_file;
int a;
String list[200];
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
 
SPI.setModule(0);

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);

  if (!SD.begin(32)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");
  int dir_size = DirectorySize(root);
  //String list[dir_size];

  root = SD.open("/");
  int i = 0;
  while(i < dir_size){
    dir_entry(root, &list[i]);
    i++;
  }
}
void loop(void) {
  
  while(Serial.available()){
    
    a = Serial.read();
    
    switch(a){
      case 49:
        current_file = SD.open("ABU.TXT");
        if (current_file) {
          while(current_file.available()){
            Serial.write(current_file.read());
          }
          current_file.close();
        } else {
        // if the file didn't open, print an error:
            Serial.println("error opening file");
          }
          break;
      case 50:
        current_file = SD.open("PHOENIX.TXT");
        if (current_file) {
          while(current_file.available()){
            Serial.write(current_file.read());
          }
          current_file.close();
        } else {
        // if the file didn't open, print an error:
            Serial.println("error opening file");
          }
          break;
      case 51:
        current_file = SD.open("TENNIS.TXT");
        if (current_file) {
          while(current_file.available()){
            Serial.write(current_file.read());
          }
          current_file.close();
        } else {
        // if the file didn't open, print an error:
            Serial.println("error opening file");
          }
          break;
    }
    }
  }
int DirectorySize(File dir){
  int i = 0;
  while(true){
    File entry = dir.openNextFile();
    if(!entry){
      return i;
    }
    i++;
    entry.close();
  }
}

void dir_entry(File dir, String*index){
  File entry = dir.openNextFile();
  Serial.println(entry.name());
  *index = entry.name();
  entry.close();
}
