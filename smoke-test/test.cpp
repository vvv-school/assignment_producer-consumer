/* 
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Ugo Pattacini <ugo.pattacini@iit.it>
 * CopyPolicy: Released under the terms of the GNU GPL v3.0.
*/

#include <string>
#include <cmath>
#include <algorithm>

#include <rtf/yarp/YarpTestCase.h>
#include <rtf/dll/Plugin.h>

#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include <yarp/sig/all.h>
#include <yarp/math/Math.h>

using namespace std;
using namespace RTF;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::math;

/**********************************************************************/
class TestAssignmentProducerConsumer : public YarpTestCase
{
private:
        BufferedPort<Bottle> port;
        string which;
public:
    /******************************************************************/
    TestAssignmentProducerConsumer() :
        YarpTestCase("TestAssignmentProducerConsumer") { }
    
    /******************************************************************/
    virtual ~TestAssignmentProducerConsumer() {
    }
    
    /******************************************************************/
    virtual bool setup(yarp::os::Property& property) {       
        RTF_ASSERT_ERROR_IF(property.check("module"), "Missing 'module' parameter");

        which = property.find("module").asString();
        RTF_TEST_REPORT(Asserter::format("Testing %s module", which.c_str()));
        RTF_ASSERT_ERROR_IF(port.open("/TestAssignmentProducerConsumer/io"), "Cannot open the test port");
        if(which == "producer") {
            RTF_ASSERT_ERROR_IF(NetworkBase::connect("/async/producer/out", port.getName()),
                                "Cannot connect to /async/producer/out");
        }
        else if(which == "consumer") {
            RTF_ASSERT_ERROR_IF(NetworkBase::connect("/first/consumer/out", port.getName()),
                                "Cannot connect to /first/consumer/out");
        }
        else
            RTF_ASSERT_ERROR(Asserter::format("Got wrong value (%s) for 'module' param", which.c_str()));

        return true;
    }
    
    /******************************************************************/
    virtual void tearDown() {
        RTF_TEST_REPORT("Tearing down TestAssignmentProducerConsumer");
        if(which == "producer")
            NetworkBase::disconnect("/async/producer/out", port.getName());
        else
            NetworkBase::connect("/first/consumer/out", port.getName());
        port.close();
    }
    
    /******************************************************************/
    virtual void run()
    {   
        Bottle* data = port.read();
        if(which == "producer") {
            if(data) {
                RTF_ASSERT_FAIL_IF(data->size()>=1, "wrong data size");
                RTF_TEST_CHECK(data->get(0).isInt(), "data type");
                RTF_TEST_REPORT(Asserter::format("Got %s", data->toString().c_str()));
            }
        }
        else {
            RTF_ASSERT_FAIL_IF(data->size()>=1, "wrong data size");
            RTF_TEST_CHECK(data->get(0).isDouble(), "data type");
            RTF_TEST_REPORT(Asserter::format("Got %s", data->toString().c_str()));
        }
    }

    /******************************************************************/
    virtual void interrupt() {
        port.interrupt();
    }
};

PREPARE_PLUGIN(TestAssignmentProducerConsumer)
