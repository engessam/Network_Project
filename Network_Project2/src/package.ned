package network_project;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=720,379");
    submodules:
        coordinator: Coordinator {
            @display("p=343,254");
        }
        node0: Node0 {
            @display("p=173,112");
        }
        node1: Node0 {
            @display("p=531,112");
        }
    connections:
        coordinator.out[1] --> node1.in[0];
        coordinator.out[0] --> node0.in[0];
        node0.out --> node1.in[1];
        node1.out --> node0.in[1];
}

