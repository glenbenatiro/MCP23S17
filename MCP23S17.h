#ifndef MCP23S17_H
#define MCP23S17_H

#include <cstdint>

class MCP23S17
{
  public:
    enum class PORT
    {
      A = 0,
      B = 1
    };

    enum class DIRECTION : bool
    {
      OUTPUT  = 0,
      INPUT   = 1
    };

  private:
    enum class OPERATION
    {
      READ  = 1,
      WRITE = 0
    };

    enum class REGISTER
    {
      IODIR   = 0,
      IPOL    = 1,
      GPINTEN = 2,
      DEFVAL  = 3,
      INTCON  = 4,
      IOCON   = 5,
      GPPU    = 6,
      INTF    = 7,
      INTCAP  = 8,
      GPIO    = 9,
      OLAT    = 0xA,
    };

    enum class ADDR_BANK_0 
    {
      IODIRA    = 0x00,
      IODIRB    = 0x01,
      IPOLA     = 0x02,
      IPOLB     = 0x03,
      GPINTENA  = 0x04,
      GPINTENB  = 0x05,
      DEFVALA   = 0x06,
      DEFVALB   = 0x07,
      INTCONA   = 0x08,
      INTCONB   = 0x09,
      IOCON     = 0x0A,  
  //  IOCON     = 0x0B // IOCON is shared between ports A and B
      GPPUA     = 0x0C,
      GPPUB     = 0x0D,
      INTFA     = 0x0E,
      INTFB     = 0x0C,
      INTCAPA   = 0x10,
      INTCAPB   = 0x11,
      GPIOA     = 0x12,
      GPIOB     = 0x13,
      OLATA     = 0x14,
      OLATB     = 0x15
    };

    const uint8_t CLIENT_ADDRESS_HEADER = 0x40;
  
  private:
    uint8_t m_hardware_address;
    uint8_t m_opcode;
    bool    m_IOCON_BANK = 0;

  private:
    uint8_t control_byte  (OPERATION op) const;
    void    mcp_write     (REGISTER reg, PORT port, uint8_t data);
    uint8_t mcp_read      (REGISTER reg, PORT port);
    uint8_t bank_address  (REGISTER reg, PORT port);

  protected:
    virtual void spi_xfer (uint8_t* rxd, uint8_t* txd, unsigned length) = 0;

  public:
    MCP23S17 (uint8_t hw_addr_bits);

    // Setter
    void hardware_address (uint8_t hw_addr_bits);

    // Getter
    uint8_t hardware_address  () const;
    uint8_t opcode            () const;

    void    write     (PORT port, uint8_t data);
    uint8_t read      (PORT port);
    void    direction (PORT port, DIRECTION direction);
    void    direction (PORT port, uint8_t pin, DIRECTION direction);
};

#endif