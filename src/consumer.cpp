#include <cstdlib>

#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Port.h>
#include <yarp/os/Property.h>
#include <yarp/os/Time.h>

using namespace std;
using namespace yarp::os;

int main(int argc, char *argv[]) {
    Network yarp;

    Property prop;
    prop.fromCommand(argc, argv);
    double delay = 0.0;
    if(prop.check("delay")) {
        delay = prop.find("delay").asDouble();
        yInfo()<<"Consumer delays reading by"<<delay<<"seconds.";
    }

    Port inPort, outPort;
    // open "/consumer/in" and  "/consumer/out" ports

    while (true) {
        Bottle data;
        // read the data from the port and measure the rate (time delay)
        // use yarp::os::Time::now() for measuring the time
        // ...

        // print out the data
        // ...
        double t1 = Time::now();

        // write measured rate to the output port

        // delay the reading
        Time::delay(delay);
    }

    return EXIT_SUCCESS;
}
