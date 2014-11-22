#include <boost/test/unit_test.hpp>
#include "statistics.h"
#include "auxpow.h"
BOOST_AUTO_TEST_SUITE(statistics_tests)
BOOST_AUTO_TEST_CASE(statistics_testsC)
{
    CStatistic test;

    test.reloadBlockForStatistics("/home/parallels/.dogecoin/blocks",43);

    map <string, float>::iterator m1_Iter;
    test.storage.insert(pair<string,float>("31/03/2014",100));
    test.storage["30/03/2014"]=200;
    test.storage["30/03/2014"]+=100;
    test.storage["29/03/2014"]+=100;
    for ( m1_Iter = test.storage.begin( ); m1_Iter != test.storage.end( ); m1_Iter++ )
    {
        cout <<  m1_Iter->first<<" "<<setiosflags(ios::fixed)<<m1_Iter->second<<endl;
    }

}





BOOST_AUTO_TEST_SUITE_END()

