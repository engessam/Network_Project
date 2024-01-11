package network_project;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    parameters:
        int WS = default(3); //sender window size
        int WR = default(3); //receiver window size
        int TO = default(10); //time out
        double PT = default(0.5);//processing time
        int TD = default(1); //transmission delay
        int ED = default(4); //error delay
        double DD = default(0.1); //duplication delay
        int ACK = default(1);
        int NACK = default(2);//negative ack
        int data = default(0);
        @display("bgb=720,379");
    submodules:
        coordinator: Coordinator {
            @display("p=343,254");
        }
        node0: Node0 {
            @display("p=173,112");
        }
        node1: Node0 {
            @display("p=526,114");
        }
    connections:
        coordinator.out[0] --> node0.in[0];
        coordinator.out[1] --> node1.in[0];
        node1.out --> node0.in[1];
        node0.out --> node1.in[1];
}

