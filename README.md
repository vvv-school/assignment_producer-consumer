Sync/async data-flow communication
==================================

# Prerequisites
By now, you should be an expert of the following components :wink::
- [yarp::os::Port] (http://www.yarp.it/classyarp_1_1os_1_1Port.html)
- [yarp::os::BufferedPort] (http://www.yarp.it/classyarp_1_1os_1_1BufferedPort.html)
- [the main YARP command-line interfaces] (http://www.yarp.it/yarp.html)

# Assignment
This assignment evaluates your understanding of sync/async data-flow communication in YARP. 
We want you to develop ...

1. Implement a module called **`producer`** with the following functionalities: 
  - has an output port (i.e. `/producer/out`)
  - writes a random numbers to the port every 100ms
  - has an argumnet called `--async`. If it is executed using `--async` switch, it should use the **`yarp::os::BufferedPort`** for streaming data otherwise it uses the **`yarp::os::Port`**
  
2. Implement another module called **`consumer`** with the following functionalities: 
  - has an input port (i.e. `/consumer/in`) to receive data from the `producer` and an output port (i.e. `/consumer/out`) to stream out some statistics
  - reads from the input port and measures the data receiving rate (i.e. the time between to conscutive read). It should print out the received data to the terminal.
  - it should stream out the measured data rate via its output port. 
  - has an argumnet called `--delay`. If it is executed, for example using `--delay 1.0`, it should wait for `1.0` second before perfoming the next read (i.e. introducing an arbirtary delay in reading)  
  - uses only **`yarp::os::Port`**

# Scenario 1
![scenario1](/misc/scenario1.png)

- open a terminal and run the **producer** in sync mode (i.e., without any switch) 
```
$ ./producer 
```

- open another terminal and run the **consumer** without any switch
```
$ ./consumer 
```
- connect the **producer** to the **consumer** using `tcp' 
```
$ yarp connect /producer/out /consumer/in tcp
```
- read the measured data rate using yarp command , i.e. `yarp read ... /consumer/out` and see your measurment. 
- repeat the above scenario using `udp` connection and/or `BufferedPort` (i.e. `--async`) to see if you notice any difference in data rate!


# Scenario 2
![scenario2](/misc/scenario2.png)

In the second scenario, you should run another instance of **consumer** with the `--delay 1.0` argument. To avoid the port name confilict, you can run the second consumer as follows. 
open another terminal and type:
```
$ YARP_PORT_PREFIX=/second ./consumer --delay 1.0
```
The **`YARP_PORT_PREFIX=/second`** environment variable simply adds a prefix (i.e. `/second`) to all the port which are opened by **`consumer`**. Thus to connect the producer to the second consumer you shoud type: 
```
yarp connect /producer/out /second/consumer/in tcp
```
- At first use only `tcp` carrier for all the connections. Check the data rate measured by the first consumer (as the previous scenario) to see if there is any difference. 
- repeat the scenario using 'udp' connections and see the difference. 
- repeat the scenario, this time using Buffered port (i.e. running **producer --async**) for both `tcp` and `udp` connection and check the diferences.


# [How to complete the assignment](https://github.com/vvv-school/vvv-school.github.io/blob/master/instructions/how-to-complete-assignments.md)
