#include "MCP23S17.h"

#include <bitset>
#include <iostream>

MCP23S17:: 
MCP23S17 (uint8_t hw_addr_bits)
  : m_hardware_address (hw_addr_bits)
{

}

uint8_t MCP23S17:: 
control_byte (MCP23S17::OPERATION op) const
{
  return ((CLIENT_ADDRESS_HEADER & 0xF0) | ((m_hardware_address & 0x7) << 1) | 
    static_cast<bool>(op));
}

void MCP23S17:: 
mcp_write (MCP23S17::REGISTER reg,
           MCP23S17::PORT     port,
           uint8_t            data)
{
  uint8_t txdata[3] = 
  {
    control_byte (MCP23S17::OPERATION::WRITE),
    bank_address (reg, port),
    data
  };

  uint8_t rxdata[3];

  spi_xfer (rxdata, txdata, 3);
}

uint8_t MCP23S17:: 
mcp_read (MCP23S17::REGISTER  reg,
          MCP23S17::PORT      port)
{
  uint8_t txdata[3] = 
  {
    control_byte (MCP23S17::OPERATION::READ),
    bank_address (reg, port),
    0x00
  };

  uint8_t rxdata[3] = {0};

  spi_xfer (rxdata, txdata, 3);

  std::cout << "mcp_read" << "\n";
  std::cout << std::bitset<8>(rxdata[0]) << "\n";
  std::cout << std::bitset<8>(rxdata[1]) << "\n";
  std::cout << std::bitset<8>(rxdata[2]) << "\n\n";

  return (rxdata[2]);
}

uint8_t MCP23S17:: 
bank_address (MCP23S17::REGISTER reg,
              MCP23S17::PORT     port)
{
  uint8_t addr = 0;

  if (m_IOCON_BANK)
  {
    addr = (static_cast<bool>(port) << 4) | (static_cast<uint8_t>(reg) & 0xF);
  }
  else 
  {
    addr = (static_cast<uint8_t>(reg) * 2) + (static_cast<bool>(port));
  }

  return (addr);
}

// Setter
void MCP23S17:: 
hardware_address (uint8_t hw_addr_bits)
{
  m_hardware_address = CLIENT_ADDRESS_HEADER | hw_addr_bits;
}

uint8_t MCP23S17:: 
hardware_address () const
{
  return (m_hardware_address);
}

uint8_t MCP23S17:: 
opcode () const
{
  return (m_opcode);
}

void MCP23S17:: 
write (MCP23S17::PORT port,
       uint8_t        data)
{
  mcp_write (MCP23S17::REGISTER::GPIO, port, data);
}

uint8_t MCP23S17:: 
read (MCP23S17::PORT port)
{
  return (mcp_read (MCP23S17::REGISTER::GPIO, port));
}

void MCP23S17:: 
direction (MCP23S17::PORT       port,
           MCP23S17::DIRECTION  direction)
{
  mcp_write (MCP23S17::REGISTER::IODIR, port, 
    static_cast<bool>(direction) ? 0xFF : 0x00);
}

void MCP23S17:: 
direction (MCP23S17::PORT       port,
           uint8_t              pin,
           MCP23S17::DIRECTION  direction)
{
  std::bitset<8> curr_iodir (mcp_read (MCP23S17::REGISTER::IODIR, port));
}


// EOF