# MCP23S17

MCP23S17 is a C++ base library for the Microchip Technology MCP23S17 16-bit SPI I/O expander.

Use this base class with your SPI library/implementation of your choice.

### How to Use
Create a new class deriving from this base class and implement the virtual function _spi_xfer_. SPI setup and cleanup should be placed in your class' constructor and destructor.
