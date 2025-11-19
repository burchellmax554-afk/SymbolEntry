The Symbol Entry project is an embedded input system developed on the FRDM-MCXN947 platform to allow users to select and transmit custom symbols through an MCU-based interface. This 
system provides a programmable framework for scrolling, selecting, and sending predefined characters to a connected host device, simulating HID-style text entry. The interface manages 
symbol navigation, switch inputs, menu logic, display feedback, and message transmission while maintaining responsiveness and accuracy across the entire workflow.

The architecture uses multiple RTOS tasks, shared data structures, GPIO-driven controls, and UART-based debugging output to coordinate user input and system behavior. Semaphores and 
mutexes ensure safe interaction between tasks and prevent race conditions during symbol updates or transmission events. Although still under development, the project demonstrates a 
scalable approach to embedded UI design and lays the foundation for a more advanced system capable of full custom-symbol recognition and HID keyboard integration.
