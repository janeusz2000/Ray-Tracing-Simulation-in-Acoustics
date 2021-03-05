#include "main/trackers.h"
#include "gtest/gtest.h"

#include <iostream>

using Json = nlohmann::json;
using trackers::File;
using trackers::FileBuffer;
using trackers::FileInterface;

class FakeFile : public FileInterface {
public:
  void openFileWithOverwrite() override{};
  void open() override{};
  void write(const FileBuffer &buffer) override {
    std::cout << "ACQUIRED BUFFER:\n" << buffer.stream.rdbuf() << std::endl;
  }
  void writeWithoutFlush(const FileBuffer &buffer) override {
    std::cout << buffer.stream.rdbuf();
  }
  void printItself(std::ostream &os) const noexcept override {
    os << "Fake File";
  }
};

const float kSkipValue = 1000;
TEST(TrackersTest, AcquiringJsonFileToBuffer) {

  FileBuffer buffer;
  Json jsonTest = {"test", kSkipValue};
  buffer.acquireJsonFile(jsonTest);
  FakeFile file;
  file.write(buffer);
}
