#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <mutex>
#include <fstream>
#include <string>
#include <vector>

#include "../mainSystem/application.hpp"

struct sourceStats
{
  size_t generates_, deleted_, sucsessfull_;
  double timeInSystem_, workingTime_;
  std::string name_;
  std::vector < double > appsTimes_;
};

struct handlerStats
{
  size_t sucsessfull_;
  double workingTime_;
  bool isWorking_;
};

class printer
{
  public:
    printer() = default;
    printer(const printer & prnt) = default;
    printer(printer && prnt) = default;
    printer & operator=(const printer & prnt) = default;
    printer & operator=(printer && prnt) = default;
    ~printer() = default;
    printer(size_t numOfSrc, const std::vector< std::string > & sourcesNames, size_t numOfBuffs, size_t numOfHands, size_t buffSize, std::ostream * out);

    void setMod(bool autoMod);
    void printStep(size_t step, double totalTime);
    void printRes(double totalTime);
    void endSource(size_t id, double fullTime, double workTime);
    void printSource(size_t id, size_t actualAppId, size_t priority);
    void printBufferIn(size_t id, application app);
    void printBufferOut(size_t id);
    void printBufferDel(size_t id, application app);
    void printHandlerIn(size_t id, application app);
    void printHandlerOut(size_t id, application app);
    void endHandler(size_t id, double workTime);

  private:
    size_t buffSize_;
    bool autoMod_;
    std::vector< sourceStats > srcStats_;
    std::vector< size_t > buffFullness_;
    std::vector< handlerStats > handStats_;
    std::mutex srcMutex_, buffMutex_, handMutex_, outMutex_;
    std::ostream * out_;
};

#endif
