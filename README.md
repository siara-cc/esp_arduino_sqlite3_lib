# Sqlite3 Arduino library for ESP8266
This library enables access to SQLite database files from SPIFFS or Micro SD Card through ESP8266 SoC using [WeMos D1 mini](https://wiki.wemos.cc/products:d1:d1_mini) and [Micro SD Card Shield](https://wiki.wemos.cc/products:d1_mini_shields:micro_sd_card_shield) (shown below) or [NodeMCU v1.0](https://en.wikipedia.org/wiki/NodeMCU) with separate MicroSD Module.

![](d1_mini_msd_shield_strip.png?raw=true)

## Why Sqlite on ESP8266 is exciting?

[Sqlite3](http://sqlite.org) is the favourite database of all that is portable and widely used.  Availability in ESP8266 makes it even more portable.  Sqlite can handle terrabyte sized data, ACID compliant and guaranteed to be stable.

So far IoT systems could offer SD Card access, which enabled gigabyte size files accessible, but was not fully utilized because the libraries that were available so far (such as [Arduino Extended Database Library](https://github.com/jwhiddon/EDB) or [SimpleDB](http://www.kendziorra.nl/arduino/103-simpledb-simple-flexible-and-smal)) offered only record number based or linear search and are terribly slow for key based indexed search.

Sqlite stores data in [B+Tree pages](https://en.wikipedia.org/wiki/B%2B_tree) and so can locate data from millions of records using variable length keys without exerting stress on CPU or RAM.  This is demonstrated using the sample databases provided in the example sections.

The Sqlite implementation occupies considerable amount of memory and should be used with caution if amount of free RAM is less than 20k.  Further, due to low RAM available on ESP8266, only Sqlite databases having page size 512 bytes can be supported.  Even with such limitation millions of records and gigabyte sized databases can be accessed.

## Usage

Sqlite3 C API such as `sqlite3_open` can be directly invoked. Before calling please invoke:

```c++
   vfs_mount("/SD0", SS); // for Micro SD Shield
   File db_file_obj_1; vfs_set_spiffs_file_obj(&db_file_obj_1); // For SPIFFS
```
apart from `SPI.begin()` or `SPIFFS.begin()` as appropriate.

The SS Pin is D8 on the Micro SD Shield for WeMos D1 mini.  It can be changed accordingly.

Please see the examples for full illustration of usage for the two file systems. The databases need to be copied to the Micro SD card root folder before the SD example can be used.  Please see the comments section of the example.

## Installation
Please download this library, unzip it to the libraries folder of your ESP8266 sdk location. The location varies according to your OS.  For example, it is usually found in the following locations:
```
Windows: C:\Users\(username)\AppData\Roaming\Arduino15
Linux: /home/<username>/.arduino15
MacOS: /home/<username>/Library/Arduino15
```
Under Arduino15 folder please navigate to `packages/esp8266/hardware/esp8266/<version>/libraries`

If you do not have the ESP8266 sdk for Arduino, please see http://esp8266.github.io/Arduino/versions/2.0.0/doc/installing.html for installing it.

## Dependencies / pre-requisites
The SdFat library is required for accessing MicroSD card.  This library can be donwloaded from https://github.com/greiman/SdFat.

The Sqlite3 code is included with the library.

The stack size of ESP8266 Arduino core is to be increased to atleast 6144 bytes to retrieve data from the sample databases as they are fairly large.  Please change CONT_STACKSIZE in `cont.h` under `cores/esp8266`.

## Limitations on ESP8266
* The default page size of 4096 leads to "Out of memory" as the size increases over 500 records. Please use page size of 512 using the commands `PRAGMA page_size=512; VACUUM;`, if you are planning to use your own sqlite3 files.
* Retrieving from db having 10 million records has been tested. But it needs stack space to be increased to atleast 6144 bytes.  Please modify cores/esp8266/cont.h to increase stack size.
* It takes around 1 second to retrieve from such dataset, even using the index.

## Limitations of this library
* Although multiple SD Cards are feasible (using multiple CS Pins), as of now only one SD Card is supported (`/SD0`).
* Before opening database files from SPIFFS, the `vfs_set_spiffs_file_obj()` should be called with a reference to SPIFFS file object
* A prefix (in front of filenames) such as `/FLASH/` is to be used for SPIFFS and `/SD0/` is to be used for Micro SD, for opening databases.

## Acknowledgements
* This library was developed by modifying the VFS layer for Sqlite3 developed by [Luiz Felipe Silva](https://github.com/luizfeliperj). The documentation can be found [here](https://nodemcu.readthedocs.io/en/master/en/modules/sqlite3/).
* The census2000 and baby names databases were taken from here: http://2016.padjo.org/tutorials/sqlite-data-starterpacks/. But no license information is available.
* The mdr512.db (Million Domain Rank database) was created with data from [The Majestic Million](https://majestic.com/reports/majestic-million) and is provided under CC 3.0 Attribution license.
* The [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)
* [The Arduino platform](https://arduino.cc)

## Screenshots

Output of Micro SD example:

![](output_screenshot.png?raw=true)

Output of SD Card database query through WebServer example:

![](output_web_1.png?raw=true)
![](output_web_2.png?raw=true)

## Issues
Please contact the author (Arundale Ramanathan) at arun@siara.cc if you find any problem (or create issue here).
