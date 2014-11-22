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

    int nLoaded = 0;

        CBufferedFile blkdat(fileIn, 2 * MAX_BLOCK_SIZE, MAX_BLOCK_SIZE + 8, SER_DISK, CLIENT_VERSION);
        if (dbp)
        {
            // (try to) skip already indexed part
            CBlockFileInfo info;
            if (pblocktree->ReadBlockFileInfo(dbp->nFile, info))
            {

                blkdat.Seek(info.nSize);
            }
        }
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
                // no valid block header found; don't complain
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
                //printf("running block:%s:%lld\n",getTimeString(block.nTime),k);
                string date =getDateString(block.nTime);
                map<string ,float >::iterator l_it;
                l_it=storage.find(date);
                if(l_it==storage.end())
                    storage[date]=0;

                storage[date]+=k;


                block.print();


            }
            catch (std::exception &e)
            {
                printf("%s : Deserialize or I/O error - %s", __func__, e.what());
            }
        }
        fclose(fileIn);
        IsGather = nLoaded;
        struct tm tm;
        gatherTime= timegm(&tm);
        printf("gatherTime =%s\n ",getTimeString(gatherTime));
    return nLoaded > 0;
}




FILE *CStatistic::OpenDiskFile2(const CDiskBlockPos &pos, const char *prefix, bool fReadOnly,string dataDir)
{
    if (pos.IsNull())
        return NULL;



    boost::filesystem::path p1=GetDataDir();
    if(dataDir!="")
    {
        boost::filesystem::path dataPath(dataDir);
        p1=dataPath;
    }
    boost::filesystem::path path = p1 / strprintf("%s%05u.dat", prefix, pos.nFile);
    printf("%s\n", path.parent_path().string().c_str());
    boost::filesystem::create_directories(path.parent_path());
    FILE *file = fopen(path.string().c_str(), "rb+");
    if (!file && !fReadOnly)
        file = fopen(path.string().c_str(), "wb+");
    if (!file)
    {
        printf("Unable to open file %s\n", path.string().c_str());
        return NULL;
    }
    if (pos.nPos)
    {
        if (fseek(file, pos.nPos, SEEK_SET))
        {
            printf("Unable to seek to position %u of %s\n", pos.nPos, path.string().c_str());
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
        if (!pblocktree)
            printf("pblocktree is null\n" );
        FILE *file = OpenDiskFile2(pos, "blk", true,dataDir);
        if (!file)
            break;
        printf("Reindexing block file blk%05u.dat...\n", (unsigned int)startNFile);
        CBlockFileInfo info;
        pblocktree->ReadBlockFileInfo((&pos)->nFile, info);

        LoadExternalBlockFile2(file, &pos);
        startNFile++;
    }

    fReindex = false;

    for (map<uint256, CBlockIndex *>::iterator mi = mapBlockIndex.begin(); mi != mapBlockIndex.end(); ++mi)
    {
        CBlockIndex *pindex = (*mi).second;
        CBlock block;
        ReadBlockFromDisk(block, pindex);
        block.BuildMerkleTree();
        block.print();

        LogPrintf("\n");
    }
}
