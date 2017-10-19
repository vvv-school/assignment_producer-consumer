
#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Property.h>
#include <yarp/os/Time.h>
#include <yarp/math/Rand.h>

using namespace std;
using namespace yarp::os;
using namespace yarp::math;

int produce_sync() {
    Port outPort;
    // open the output port
    //...

    while(true) {        
        Bottle data;
        // add a random value using Rand::scalar(1,100) to the bottle
        //...

        // write to the port
        //...

        // delay for 100ms
        Time::delay(0.1);
    }
    return 0;
}

int produce_async() {
    BufferedPort<Bottle> outPort;
    // open the output buffered port
    //...

    while(true) {
        // add a random value using Rand::scalar(1,100) to a bottle
        //...

        // write to the port
        //...

        // delay for 100ms
        Time::delay(0.1);
    }

    return 0;
}


int main(int argc, char *argv[]) {
    Network yarp;

    Property prop;
    prop.fromCommand(argc, argv);
    if(prop.check("async")) {
        yInfo()<<"Producer using async mode";
        return produce_async();
    }

    yInfo()<<"Producer using sync mode";
    return produce_sync();
}
