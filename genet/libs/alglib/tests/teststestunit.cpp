
#include <stdafx.h>
#include <stdio.h>
#include "teststestunit.h"

static void generate(int n, double med, double width, ap::real_1d_array& r);
static void generatediscrete(int n, int med, int width, ap::real_1d_array& r);

bool testsigntest(bool silent)
{
    bool result;
    int passcount;
    bool detailedreport;
    double sigmathreshold;
    int n;
    int pass;
    int dtask;
    int npos;
    int ncnt;
    ap::integer_1d_array ntbl;
    int qcnt;
    ap::real_1d_array btqtbl;
    ap::real_1d_array lrqtbl;
    ap::real_1d_array btsigmatbl;
    ap::real_1d_array lrsigmatbl;
    ap::real_1d_array bttbl;
    ap::real_1d_array lttbl;
    ap::real_1d_array rttbl;
    double bt;
    double lt;
    double rt;
    double lpower;
    double rpower;
    double bpower;
    ap::real_1d_array x;
    bool tmptestok;
    bool dtestok;
    bool ctestok;
    bool powertestok;
    bool waserrors;
    int i;
    int j;
    int k;
    int med;
    double v;

    waserrors = false;
    detailedreport = true;
    passcount = 20000;
    sigmathreshold = 5;
    n = 100;
    
    //
    // Prepare place
    //
    x.setbounds(0, n-1);
    
    //
    // Prepare quantiles tables for both-tail test and left/right test.
    // Note that since sign test statistic has discrete distribution
    // we should carefully select quantiles. Equation P(tail<=alpha)=alpha
    // holds only for some specific alpha.
    //
    qcnt = 5;
    bttbl.setbounds(0, qcnt-1);
    lttbl.setbounds(0, qcnt-1);
    rttbl.setbounds(0, qcnt-1);
    btqtbl.setbounds(0, qcnt-1);
    lrqtbl.setbounds(0, qcnt-1);
    btsigmatbl.setbounds(0, qcnt-1);
    lrsigmatbl.setbounds(0, qcnt-1);
    btqtbl(0) = 0.272;
    btqtbl(1) = 0.195;
    btqtbl(2) = 0.134;
    btqtbl(3) = 0.057;
    btqtbl(4) = 0.036;
    for(i = 0; i <= qcnt-1; i++)
    {
        lrqtbl(i) = 0.5*btqtbl(i);
    }
    for(i = 0; i <= qcnt-1; i++)
    {
        btsigmatbl(i) = sqrt(btqtbl(i)*(1-btqtbl(i))/passcount);
        lrsigmatbl(i) = sqrt(lrqtbl(i)*(1-lrqtbl(i))/passcount);
    }
    
    //
    // Report header
    //
    if( !silent )
    {
        printf("TESTING SIGN TEST\n");
    }
    
    //
    // Continuous distribuiton test for validity.
    //
    
    //
    // Prepare p-table
    //
    for(i = 0; i <= qcnt-1; i++)
    {
        bttbl(i) = 0;
        lttbl(i) = 0;
        rttbl(i) = 0;
    }
    
    //
    // Fill p-table
    //
    for(pass = 1; pass <= passcount; pass++)
    {
        med = ap::randominteger(11)-5;
        generate(n, double(med), 1.0, x);
        onesamplesigntest(x, n, double(med), bt, lt, rt);
        for(i = 0; i <= qcnt-1; i++)
        {
            if( bt<=btqtbl(i) )
            {
                bttbl(i) = bttbl(i)+double(1)/double(passcount);
            }
            if( lt<=lrqtbl(i) )
            {
                lttbl(i) = lttbl(i)+double(1)/double(passcount);
            }
            if( rt<=lrqtbl(i) )
            {
                rttbl(i) = rttbl(i)+double(1)/double(passcount);
            }
        }
    }
    
    //
    // Check
    //
    tmptestok = true;
    for(i = 0; i <= qcnt-1; i++)
    {
        if( fabs(bttbl(i)-btqtbl(i))/btsigmatbl(i)>sigmathreshold )
        {
            tmptestok = false;
        }
        if( fabs(rttbl(i)-lrqtbl(i))/lrsigmatbl(i)>sigmathreshold )
        {
            tmptestok = false;
        }
        if( fabs(lttbl(i)-lrqtbl(i))/lrsigmatbl(i)>sigmathreshold )
        {
            tmptestok = false;
        }
    }
    ctestok = tmptestok;
    
    //
    // Report
    //
    if( !silent&&detailedreport )
    {
        printf("\nCONTINUOUS TEST TABLE\n");
        printf("Q no.     BT err.   LT err.   RT err.   \n");
        for(i = 0; i <= qcnt-1; i++)
        {
            printf("%1ld         %3.1lf std.  %3.1lf std.  %3.1lf std.  \n",
                long(i+1),
                double(fabs(bttbl(i)-btqtbl(i))/btsigmatbl(i)),
                double(fabs(lttbl(i)-lrqtbl(i))/lrsigmatbl(i)),
                double(fabs(rttbl(i)-lrqtbl(i))/lrsigmatbl(i)));
        }
        if( tmptestok )
        {
            printf("TEST PASSED\n\n");
        }
        else
        {
            printf("TEST FAILED\n\n");
        }
    }
    
    //
    // Discrete distribuiton test for validity.
    // Note discrete specifics: due to possible X[i]=Med
    // we can't expect that P(tail<=alpha) = alpha.
    // We can expect only that P(tail<=alpha) <= alpha.
    // So test code is slightly modified.
    //
    
    //
    // Prepare p-table
    //
    for(i = 0; i <= qcnt-1; i++)
    {
        bttbl(i) = 0;
        lttbl(i) = 0;
        rttbl(i) = 0;
    }
    
    //
    // Fill p-table
    //
    for(pass = 1; pass <= passcount; pass++)
    {
        med = ap::randominteger(11)-5;
        generatediscrete(n, med, 2, x);
        onesamplesigntest(x, n, double(med), bt, lt, rt);
        for(i = 0; i <= qcnt-1; i++)
        {
            if( bt<=btqtbl(i) )
            {
                bttbl(i) = bttbl(i)+double(1)/double(passcount);
            }
            if( lt<=lrqtbl(i) )
            {
                lttbl(i) = lttbl(i)+double(1)/double(passcount);
            }
            if( rt<=lrqtbl(i) )
            {
                rttbl(i) = rttbl(i)+double(1)/double(passcount);
            }
        }
    }
    
    //
    // Check
    //
    tmptestok = true;
    for(i = 0; i <= qcnt-1; i++)
    {
        if( (bttbl(i)-btqtbl(i))/btsigmatbl(i)>sigmathreshold )
        {
            tmptestok = false;
        }
        if( (rttbl(i)-lrqtbl(i))/lrsigmatbl(i)>sigmathreshold )
        {
            tmptestok = false;
        }
        if( (lttbl(i)-lrqtbl(i))/lrsigmatbl(i)>sigmathreshold )
        {
            tmptestok = false;
        }
    }
    dtestok = tmptestok;
    
    //
    // Report
    //
    if( !silent&&detailedreport )
    {
        printf("\nDISCRETE TEST TABLE (dont worry, zeros are normal)\n");
        printf("Q no.     BT err.   LT err.   RT err.   \n");
        for(i = 0; i <= qcnt-1; i++)
        {
            printf("%1ld         %3.1lf std.  %3.1lf std.  %3.1lf std.  \n",
                long(i+1),
                double(ap::maxreal((bttbl(i)-btqtbl(i))/btsigmatbl(i), double(0))),
                double(ap::maxreal((lttbl(i)-lrqtbl(i))/lrsigmatbl(i), double(0))),
                double(ap::maxreal((rttbl(i)-lrqtbl(i))/lrsigmatbl(i), double(0))));
        }
        if( tmptestok )
        {
            printf("TEST PASSED\n\n");
        }
        else
        {
            printf("TEST FAILED\n\n");
        }
    }
    
    //
    // Power test
    //
    
    //
    // Prepare p-table
    //
    for(i = 0; i <= qcnt-1; i++)
    {
        bttbl(i) = 0;
        lttbl(i) = 0;
        rttbl(i) = 0;
    }
    
    //
    // Fill p-table
    //
    lpower = 0;
    rpower = 0;
    bpower = 0;
    for(pass = 1; pass <= passcount; pass++)
    {
        v = 0.7;
        generate(n, 0.0, 1.0, x);
        onesamplesigntest(x, n, v, bt, lt, rt);
        if( lt<0.05 )
        {
            lpower = lpower+double(1)/double(passcount);
        }
        onesamplesigntest(x, n, -v, bt, lt, rt);
        if( rt<0.05 )
        {
            rpower = rpower+double(1)/double(passcount);
        }
        onesamplesigntest(x, n, v*(2*ap::randominteger(2)-1), bt, lt, rt);
        if( bt<0.05 )
        {
            bpower = bpower+double(1)/double(passcount);
        }
    }
    
    //
    // Check
    //
    powertestok = lpower>0.95&&rpower>0.95&&bpower>0.95;
    
    //
    // Report
    //
    if( !silent&&detailedreport )
    {
        printf("\nPOWER TEST TABLE\n");
        printf("LEFT-TAIL TEST POWER:  %4.2lf\n",
            double(lpower));
        printf("RIGHT-TAIL TEST POWER: %4.2lf\n",
            double(rpower));
        printf("BOTH-TAIL TEST POWER:  %4.2lf\n",
            double(bpower));
        if( powertestok )
        {
            printf("TEST PASSED\n\n");
        }
        else
        {
            printf("TEST FAILED\n\n");
        }
    }
    
    //
    // Final report
    //
    waserrors = !ctestok||!dtestok||!powertestok;
    if( !silent )
    {
        printf("CONTINUOUS VALIDITY TEST:                ");
        if( ctestok )
        {
            printf("OK\n");
        }
        else
        {
            printf("FAILED\n");
        }
        printf("DISCRETE VALIDITY TEST:                  ");
        if( dtestok )
        {
            printf("OK\n");
        }
        else
        {
            printf("FAILED\n");
        }
        printf("POWER TEST:                              ");
        if( powertestok )
        {
            printf("OK\n");
        }
        else
        {
            printf("FAILED\n");
        }
        if( waserrors )
        {
            printf("TEST SUMMARY: FAILED\n");
        }
        else
        {
            printf("TEST SUMMARY: PASSED\n");
        }
        printf("\n\n");
    }
    result = !waserrors;
    return result;
}


static void generate(int n, double med, double width, ap::real_1d_array& r)
{
    int i;

    for(i = 0; i <= n-1; i++)
    {
        r(i) = med+width*(2*ap::randomreal()-1);
    }
}


static void generatediscrete(int n, int med, int width, ap::real_1d_array& r)
{
    int i;

    for(i = 0; i <= n-1; i++)
    {
        r(i) = med+(ap::randominteger(2*width+1)-width);
    }
}


/*************************************************************************
Silent unit test
*************************************************************************/
bool teststestunit_test_silent()
{
#ifndef ALGLIB_OPTIMIZED
    bool result;

    result = testsigntest(true);
    return result;
#else
    return _i_teststestunit_test_silent();
#endif
}


/*************************************************************************
Unit test
*************************************************************************/
bool teststestunit_test()
{
#ifndef ALGLIB_OPTIMIZED
    bool result;

    result = testsigntest(false);
    return result;
#else
    return _i_teststestunit_test();
#endif
}


