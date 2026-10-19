#include "SDManager.h"
#include "Logger/Logger.h"
#include <vector>

bool SDManager::init(const uint8_t sd_cs_pin, const uint8_t sd_clk_pin, const uint8_t sd_mosi_pin, const uint8_t sd_miso_pin) {
  bool spiResult = SPI.begin(sd_clk_pin, sd_miso_pin, sd_mosi_pin, sd_cs_pin);

  if (!spiResult) {
    INFO("[ER] SD SPI!!");
    return false;
  }

  bool beginResult = SD.begin(sd_cs_pin, SPI);

  if (!beginResult) {
    INFO("[ER] SD Begin!!");
    return false;
  }

  INFO("[OK] SD");

  const int inMb = 1024 * 1024;
  const double sdSize = this->size(inMb);
  const double sdFreeSpace = this->freeSpace(inMb);
  const double sdFreeSpacePercent = 100.0 * sdFreeSpace / sdSize;

  INFO("SDCard size: %.2f Mb", sdSize);
  INFO("Free SDCard space: %.2f Mb (~%0.2f %%)", sdFreeSpace, sdFreeSpacePercent);

  this->mounted = true;
  return true;
}

File SDManager::open(String path, const char *mode, const bool create) {
  if (!this->mounted) {
    ERROR("SD card system is not mounted yet!");
    return File();
  }

  File file = SD.open(path, mode, create);

  if (!file) {
    ERROR("Unable to read the provided file: %s", path.c_str());
    return File();
  }

  return file;
}

File SDManager::read(String path) {
  File file = this->open(path, FILE_READ, false);

  return file;
}

bool SDManager::write(String path, const uint8_t value, const bool create) {
  File file = this->open(path, FILE_WRITE, create);

  if (!file) {
    return false;
  }

  file.seek(0);
  file.printf("%d", value);
  file.close();

  return true;
}

bool SDManager::write(String path, const String value, const bool create) {
  File file = this->open(path, FILE_WRITE, create);

  if (!file) {
    return false;
  }

  file.seek(0);
  file.printf("%s", value.c_str());
  file.close();

  return true;
}

bool SDManager::append(String path, const String value, const bool create) {
  File file = this->open(path, FILE_APPEND, create);

  if (!file) {
    return false;
  }

  file.println(value);
  file.close();

  return true;
}

double SDManager::size(const int modifier) {
  return SD.totalBytes() / modifier;
}

double SDManager::usedSpace(const int modifier) {
  return SD.usedBytes() / modifier;
}

double SDManager::freeSpace(const int modifier) {
  const double totalSpace = this->size(modifier);
  const double freeSpace = totalSpace - this->usedSpace(modifier);

  return freeSpace;
}

bool SDManager::exists(String path) {
  return SD.exists(path);
}

std::vector<String> SDManager::list(String path, uint8_t mode) {
  std::vector<String> paths;
  File root = SD.open(path);
  File file = root.openNextFile();

  while (file) {
    if (file.isDirectory() && mode == SD_LS_DIRECTORY_MODE) {
      paths.push_back(file.name());
    }

    if (mode == SD_LS_FILE_MODE) {
      paths.push_back(file.name());
    }

    file = root.openNextFile();
  }

  return paths;
}
