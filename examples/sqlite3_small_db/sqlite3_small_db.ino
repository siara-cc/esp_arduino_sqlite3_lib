/*
    This example opens Sqlite3 databases from SD Card and
    retrieves data from them.
    Before running please copy following files to SD Card:
    data/babyname.db

    Also increase stack size in cores/esp8266/cont.h
    to atleast 6144 (from 4096).

    Please see https://github.com/siara-cc/esp_arduino_sqlite3_lib/
    for more information.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <vfs.h>
#include <SPI.h>
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

int openDb(const char *filename, sqlite3 **db) {
  int rc = sqlite3_open(filename, db);
  if (rc) {
      Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
      return rc;
  } else {
      Serial.printf("Opened database successfully\n");
  }
  return rc;
}

void setup ( void ) {

    sqlite3 *db1;
    int rc;
    sqlite3_stmt *res;
    int rec_count = 0;
    const char *tail;

    Serial.begin(74880);

    system_update_cpu_freq(SYS_CPU_160MHZ);
    WiFiOff();

    SPI.begin();
    vfs_mount("/SD0", SS);

    sqlite3_initialize();

    // Open database 1
    if (openDb("/SD0/babyname.db", &db1))
        return;

    String sql = "Select year, state, name, total_babies, primary_sex, primary_sex_ratio, \
                    per_100k_in_state from gendered_names \
                    where name between 'Bob' and 'Bobby'";
    rc = sqlite3_prepare_v2(db1, sql.c_str(), 1000, &res, &tail);
    if (rc != SQLITE_OK) {
        String resp = "Failed to fetch data: ";
        resp += sqlite3_errmsg(db1);
        resp += "<br><br><a href='/'>back</a>";
        Serial.println(resp.c_str());
        return;
    }

    while (sqlite3_step(res) == SQLITE_ROW) {
        Serial.printf("Year: %d\n", sqlite3_column_int(res, 0));
        Serial.printf("State: %s\n", (const char *) sqlite3_column_text(res, 1));
        Serial.printf("Name: %s\n", (const char *) sqlite3_column_text(res, 2));
        Serial.printf("Total babies: %d\n", sqlite3_column_int(res, 3));
        Serial.printf("Primary sex: %s\n", (const char *) sqlite3_column_text(res, 4));
        Serial.printf("Ratio: %lf\n", sqlite3_column_double(res, 5));
        Serial.printf("Per 100k in state: %lf\n\n", sqlite3_column_double(res, 6));
        rec_count++;
    }
    Serial.printf("No. of records: %d", rec_count);

    sqlite3_finalize(res);

    sqlite3_close(db1);

}

void loop () {
}
