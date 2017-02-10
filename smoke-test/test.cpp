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

#define PORT_READ_TIMEOUT   5.0 //seconds

class Interrupter : public Thread {
    double timeout;
    YarpTestCase* testCase;
public:
        Interrupter(YarpTestCase* test, double timeout=1.0) : testCase(test) {
            setTimeout(timeout);
        }
        void setTimeout(double timeout) {
            Interrupter::timeout = timeout;
        }
        virtual void run() {
            double t1 = Time::now();
            while (((Time::now() - t1) < timeout) && (!isStopping()))
                Time::delay(0.5);
            if((Time::now() - t1) >= timeout) {
                RTF::Asserter::report(RTF::TestMessage("reports",
                                      "Interrupting the test (timeout)",
                                      RTF_SOURCEFILE(),
                                      RTF_SOURCELINE()), testCase);
                testCase->interrupt();
            }
        }
};

/**********************************************************************/
class TestAssignmentProducerConsumer : public YarpTestCase
{
private:
        BufferedPort<Bottle> port;
        string which;
        Interrupter timer;
public:
    /******************************************************************/
    TestAssignmentProducerConsumer() : timer(this, PORT_READ_TIMEOUT),
        YarpTestCase("TestAssignmentProducerConsumer") { }

    /******************************************************************/
    virtual ~TestAssignmentProducerConsumer() {
    }

    /******************************************************************/
    virtual bool setup(yarp::os::Property& property) {
        RTF_ASSERT_ERROR_IF(property.check("module"), "Missing 'module' parameter");

        which = property.find("module").asString();
        RTF_TEST_REPORT(Asserter::format("Testing %s module", which.c_str()));
        Contact contact("/TestAssignmentProducerConsumer/io");
        contact.setTimeout(5.0);
        RTF_ASSERT_ERROR_IF(port.open(contact), "Cannot open the test port");
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
        timer.stop();
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
        Bottle* data = NULL;
        // read few time to ensure bottle from BufferedPort is cleared
        // by user
        if(which == "producer") {
            for (int i=0; i<5; i++) {
                RTF_TEST_REPORT(Asserter::format("Reading data from '/async/producer/out'. itteration %d", i));
                timer.start();
                data = port.read();
                timer.stop();
                RTF_ASSERT_ERROR_IF(data, "Cannot read from the port");
            }
            RTF_TEST_REPORT("Cheking data size of producer module");
            RTF_ASSERT_FAIL_IF(data->size()==1,
                               Asserter::format("Wrong data size (expected size=1, got size=%d)", data->size()));
            RTF_TEST_CHECK(data->get(0).isInt(), "data type");
            RTF_TEST_REPORT(Asserter::format("Got %s", data->toString().c_str()));
        }
        else {
            RTF_TEST_REPORT("Reading data from '/first/consumer/out' ...");
            timer.start();
            data = port.read();
            timer.stop();
            RTF_ASSERT_ERROR_IF(data, "Cannot read from the port");
            RTF_ASSERT_FAIL_IF(data->size()>=1,
                               Asserter::format("Wrong data size (expected size>1, got size=%d)", data->size()));
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
