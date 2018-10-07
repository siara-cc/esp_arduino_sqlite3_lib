# Sqlite3 Arduino library for ESP8266
This library enables access to SQLite database files from SPIFFS or Micro SD Card through ESP8266 SoC using WeMos D1 mini and MicroSD Shield (shown below) or NodeMCU v1.0 with separate MicroSD Module.

![](d1_mini_msd_shield_strip.png?raw=true)

This library was developed by modifying the VFS layer developed by [[https://github.com/luizfeliperj]Luiz Felipe Silva] for Sqlite3 - NodeMCU integration.

## Usage
Sqlite3 C API such as `sqlite3_open` can be directly invoked. Before calling please invoke:

```c++
   vfs_mount("/SD0", SS); // for Micro SD Shield
   File db_file_obj_1; vfs_set_spiffs_file_obj(&db_file_obj_1); // For SPIFFS

apart from `SPI.begin()` or `SPIFFS.begin()` as appropriate.

The SS Pin is D8 on the Micro SD Shield for WeMos D1 mini.  It can be changed accordingly.

Please see the examples for full illustration of usage for the two file systems.

## Dependencies
The SdFat library is required for accessing MicroSD card.  This library can be donwloaded from https://github.com/greiman/SdFat
The Sqlite3 code is included with the library.

## Installation
Please download this library, unzip it to the libraries folder of your ESP8266 sdk location. The location varies according to your OS.  For example, it is usually found in the following locations:
Windows: App

If you do not have the ESP8266 sdk for Arduino, please see http://esp8266.github.io/Arduino/versions/2.0.0/doc/installing.html for installing it.

## Limitations on ESP8266
* The default page size of 4096 leads to "Out of memory" as the size increases over 500 records. Please use page size of 512 using the commands "PRAGMA page_size=512; VACUUM;", if you are planning to use your own sqlite3 files.
* Inserting records over a 1000 records gives "Out of memory"
* These problems exist on NodeMCU as well due to low memory on ESP8266
* Retrieving from db having 10 million records has been tested. But it needs stack space to be increased to atlease 6144 bytes.  Please modify cores/esp8266/cont.h to increase stack size.

## Acknowledgements
* This library was developed by modifying the VFS layer developed by https://github.com/luizfeliperj
* The census2000 and baby names databases were taken from here: http://2016.padjo.org/tutorials/sqlite-data-starterpacks/. But no license information is available and a query to the author through email bounces.
* The mdr512.db (Million Domain Rank database) was created with data from https://majestic.com/reports/majestic-million and is provided under CC 3.0 Attribution license.

