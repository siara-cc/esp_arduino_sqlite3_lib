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

int openDb(char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if( rc ) {
      Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
      return rc;
   } else {
      Serial.printf("Opened database successfully\n");
   }
   return rc;
}

void setup() {
   Serial.begin(74880);
   sqlite3 *db1;
   sqlite3 *db2;
   char *zErrMsg = 0;
   int rc;
   char *sql;

  system_update_cpu_freq(SYS_CPU_160MHZ);
  WiFiOff();

  //if (!SPIFFS.begin()) {
  //  Serial.println("Failed to mount file system");
  //  return;
  //}

   SPI.begin();
   vfs_mount("/SD0", SS);

   sqlite3_initialize();

   // Open database 1
   if (openDb("/SD0/dbpedia.db", &db1))
       return;
   if (openDb("/SD0/mdr512.db", &db2))
       return;

   long start = micros();
   rc = sqlite3_exec(db1, "Select * from labels where label between 'Benthic' and 'Benthicz'", callback, (void*)data, &zErrMsg);
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   if( rc != SQLITE_OK ) {
      Serial.printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      Serial.printf("Operation done successfully\n");
   }

   start = micros();
   rc = sqlite3_exec(db2, "Select * from domain_rank where domain between 'google.com' and 'google.com.z'", callback, (void*)data, &zErrMsg);
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   if( rc != SQLITE_OK ) {
      Serial.printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      Serial.printf("Operation done successfully\n");
   }

   start = micros();
   rc = sqlite3_exec(db1, "Select * from labels where label = 'Benthoscope'", callback, (void*)data, &zErrMsg);
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   if( rc != SQLITE_OK ) {
      Serial.printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      Serial.printf("Operation done successfully\n");
   }

   start = micros();
   rc = sqlite3_exec(db2, "Select * from domain_rank where domain = 'zoho.com'", callback, (void*)data, &zErrMsg);
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   if( rc != SQLITE_OK ) {
      Serial.printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      Serial.printf("Operation done successfully\n");
   }

   sqlite3_close(db1);
   sqlite3_close(db2);
}

void loop() {
}