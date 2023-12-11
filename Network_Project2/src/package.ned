package network_project;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=720,379");
    submodules:
        node0: Node0 {
            @display("p=173,112");
        }
        node1: Node0 {
            @display("p=531,112");
        }
        coordinator: Coordinator {
            @display("p=349,251");
        }
    connections:
        coordinator.out[1] --> node1.in[0];     
        node1.out[0] --> coordinator.in[1];		
        coordinator.out[0] --> node0.in[0];		
        node0.out[0] --> coordinator.in[0];		
        node0.out[1] --> node1.in[1];			
        node1.out[1] --> node0.in[1];			
}

