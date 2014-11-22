#ifndef BITCOIN_STATISTICS_H
#define BITCOIN_STATISTICS_H
#include "main.h"


#include "auxpow.h"
#include "txdb.h"


#include <sstream>

#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
class CDiskBlockPos;
#include <boost/filesystem.hpp>
class CStatistic{
public:
void reloadBlockForStatistics(string dataDir="",int startNFile=0);
char* getTimeString(uint32_t timeStamp);
string getDateString(uint32_t timeStamp);

bool LoadExternalBlockFile2(FILE *fileIn, CDiskBlockPos *dbp);

FILE* OpenDiskFile2(const CDiskBlockPos &pos, const char *prefix, bool fReadOnly,string dataDir="");
map<string,float> storage;
private:
    bool IsGather;
    time_t gatherTime;


};
//%d.%08d
#endif
