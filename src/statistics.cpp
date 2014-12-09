#include "statistics.h"
//#include

char* CStatistic::getTimeString(uint32_t timeStamp)
{
    static char scratch[1024];
    time_t t(timeStamp);
    struct tm *gtm = gmtime(&t);
    strftime(scratch, 1024, "%m/%d/%Y %H:%M:%S", gtm);
    return scratch;
}

string CStatistic::getDateString(uint32_t timeStamp)
{
    static char scratch[16];
    time_t t(timeStamp);
    struct tm *gtm = gmtime(&t);
    strftime(scratch, 16, "%m/%d/%Y", gtm);
    return scratch;
}

bool CStatistic::LoadExternalBlockFile2(FILE *fileIn, CDiskBlockPos *dbp)
{
 boost::this_thread::interruption_point();
    int nLoaded = 0;

        CBufferedFile blkdat(fileIn, 2 * MAX_BLOCK_SIZE, MAX_BLOCK_SIZE + 8, SER_DISK, CLIENT_VERSION);

        uint64_t nRewind = blkdat.GetPos();
        while (blkdat.good() && !blkdat.eof())
        {

            boost::this_thread::interruption_point();

            blkdat.SetPos(nRewind);
            nRewind++; // start one byte further next time, in case of failure
            blkdat.SetLimit(); // remove former limit
            unsigned int nSize = 0;
            try
            {
                // locate a header

                unsigned char buf[MESSAGE_START_SIZE];
                blkdat.FindByte(Params().MessageStart()[0]);
                nRewind = blkdat.GetPos() + 1;
                blkdat >> FLATDATA(buf);
                if (memcmp(buf, Params().MessageStart(), MESSAGE_START_SIZE))
                    continue;
                // read size

                blkdat >> nSize;
                if (nSize < 80 || nSize > MAX_BLOCK_SIZE)
                    continue;
            }
            catch (std::exception &e)
            {
                //no valid block header found; don't complain
                break;
            }
            try
            {

                // read block
                uint64_t nBlockPos = blkdat.GetPos();
                blkdat.SetLimit(nBlockPos + nSize);
                CBlock block;
                blkdat >> block;
                nRewind = blkdat.GetPos();



                float k=0;

                BOOST_FOREACH(CTransaction & tx, block.vtx)
                {
                    k+=tx.GetValueOut();
                }

                string date =getDateString(block.nTime);
                map<string ,float >::iterator l_it;
                l_it=storage.find(date);
                if(l_it==storage.end())
                    storage[date]=0;

                storage[date]+=k;
                Latestone = block;


            }
            catch (std::exception &e)
            {
                //printf("%s : Deserialize or I/O error - %s", __func__, e.what());
            }
        }


        fclose(fileIn);

    return nLoaded > 0;
}




FILE *CStatistic::OpenDiskFile2(const CDiskBlockPos &pos, const char *prefix, bool fReadOnly,string dataDir)
{
    if (pos.IsNull())
        return NULL;



    boost::filesystem::path p1=GetDataDir()/"blocks";
    if(dataDir!="")
    {
        boost::filesystem::path dataPath(dataDir);
        p1=dataPath;
    }
    boost::filesystem::path path = p1 / strprintf("%s%05u.dat", prefix, pos.nFile);
    boost::filesystem::create_directories(path.parent_path());
    FILE *file = fopen(path.string().c_str(), "rb+");
    if (!file && !fReadOnly)
        file = fopen(path.string().c_str(), "wb+");
    if (!file)
    {

        return NULL;
    }
    if (pos.nPos)
    {
        if (fseek(file, pos.nPos, SEEK_SET))
        {

            fclose(file);
            return NULL;
        }
    }
    return file;
}
void CStatistic::reloadBlockForStatistics(string dataDir,int startNFile)
{
    while (true)
    {
        CDiskBlockPos pos(startNFile, 0);

        FILE *file = OpenDiskFile2(pos, "blk", true,dataDir);
        if (!file)
            break;
        CBlockFileInfo info;
        pblocktree->ReadBlockFileInfo((&pos)->nFile, info);

        LoadExternalBlockFile2(file, &pos);
        startNFile++;
    }

}

