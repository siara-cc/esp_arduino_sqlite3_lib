/*
    This example shows how Short Strings can
    be compressed and stored in database.
    This is based on Shox96 - Guaranteed compression
    for Short Strings found at
    https://github.com/siara-cc/Shox96
    Increase stack size in cores/esp8266/cont.h
    to atleast 6144 (from 4096)
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
   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   Serial.printf("\n");
   return 0;
}

int openDb(char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if (rc) {
       Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
       return rc;
   } else {
       Serial.printf("Opened database successfully\n");
   }
   return rc;
}

char *zErrMsg = 0;
int db_exec(sqlite3 *db, const char *sql) {
   Serial.println(sql);
   long start = micros();
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
       Serial.printf("SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
   } else {
       Serial.printf("Operation done successfully\n");
   }
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   return rc;
}

void setup() {
   Serial.begin(74880);
   sqlite3 *db1;
   char *zErrMsg = 0;
   int rc;
   char *sql;

   system_update_cpu_freq(SYS_CPU_160MHZ);
   WiFiOff();

   SPI.begin();
   vfs_mount("/SD0", SS);

   sqlite3_initialize();

   // Open database 1
   if (openDb("/SD0/test.db", &db1))
       return;

   rc = db_exec(db1, "CREATE TABLE test (b1 blob)");
   //if (rc != SQLITE_OK) {
   //    sqlite3_close(db1);
   //    return;
   //}
   rc = db_exec(db1, "INSERT INTO test VALUES (shox96_0_2c('Hello World'))");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       return;
   }
   rc = db_exec(db1, "SELECT shox96_0_2d(b1) FROM test");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       return;
   }

   sqlite3_close(db1);

}

void loop() {
}
