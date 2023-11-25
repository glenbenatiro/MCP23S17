#include <iostream>
#include <bitset>
#include <thread>
#include <chrono>

#include <pigpio.h>

#include "../MCP23S17.h"

// compile with
// g++ -Wall -pthread test.cpp -o test -lpigpio -lrt

class MCP23S17_pigpio : public MCP23S17
{
  private:
    int m_spi_fd = 0;

  private:
    virtual void spi_xfer (uint8_t* rxd, uint8_t* txd, unsigned length)
    {
      spiXfer (m_spi_fd, reinterpret_cast<char*>(txd), reinterpret_cast<char*>(rxd), length);
    }
  
  public:
    MCP23S17_pigpio (uint8_t hw_addr_bits) : MCP23S17 (hw_addr_bits)
    {
      m_spi_fd = spiOpen (0, 100'000, 0);
    }
};

int main ()
{
  gpioInitialise ();
  
  MCP23S17_pigpio xpander (0);

  xpander.direction (MCP23S17_pigpio::PORT::A, MCP23S17_pigpio::DIRECTION::OUTPUT);

  //

  while (true)
  {
    uint8_t data = xpander.read (MCP23S17_pigpio::PORT::B);

    std::cout << "read data : " << std::bitset<8> (data) << "\n";
    std::cout << "write data: 0xAA\n"; xpander.write (MCP23S17_pigpio::PORT::A, 0xFF);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));

    std::cout << "read data : " << std::bitset<8> (data) << "\n";
    std::cout << "write data: 0x55\n"; xpander.write (MCP23S17_pigpio::PORT::A, 0x00);

    std::this_thread::sleep_for (std::chrono::milliseconds (250));
  }

  //

  gpioTerminate ();
}