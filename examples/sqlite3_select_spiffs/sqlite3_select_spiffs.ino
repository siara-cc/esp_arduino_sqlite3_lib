/*
    SQLite3 Select demo sketch

*/
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <vfs.h>
#include <SPI.h>
#include <FS.h>
extern "C" {
#include "user_interface.h"
}
#include <ESP8266WiFi.h>

void WiFiOff() {
    wifi_station_disconnect();
    wifi_set_opmode(NULL_MODE);
    wifi_set_sleep_type(MODEM_SLEEP_T);
    wifi_fpm_open();
    wifi_fpm_do_sleep(0xFFFFFFF);
}

const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   Serial.printf("%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++){
      Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   Serial.printf("\n");
   return 0;
}

void setup()
{
   Serial.begin(74880);
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;

  //system_update_cpu_freq(SYS_CPU_160MHZ);
  WiFiOff();

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {    
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %ld\n", fileName.c_str(), fileSize);
    }
    Serial.printf("\n");

   sqlite3_initialize();

   // Open database
   SPIFFS.remove("/test.db");
   rc = sqlite3_open("/FLASH/test.db", &db);

   if( rc ) {
      Serial.printf("Can't open database: %s\n", sqlite3_errmsg(db));
        return;
   } else {
      Serial.printf("Opened database successfully\n");
   }

   long start = micros();

   rc = sqlite3_exec(db, "CREATE TABLE test (id INTEGER, content);", callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ) {
      Serial.printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      Serial.printf("Operation done successfully\n");
   }

   rc = sqlite3_exec(db, "INSERT INTO test VALUES (1, 'Hello, World'); COMMIT", callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ) {
      Serial.printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      Serial.printf("Operation done successfully\n");
   }

   rc = sqlite3_exec(db, "SELECT * from test", callback, (void*)data, &zErrMsg);
    Serial.print(F("Time taken:"));
    Serial.println(micros()-start);
   
   if( rc != SQLITE_OK ) {
      Serial.printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      Serial.printf("Operation done successfully\n");
   }
   sqlite3_close(db);
}

void loop() {
}