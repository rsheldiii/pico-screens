#include "spi_buffer.h"
#include <pico.h>
#include <hardware/pio.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <stdio.h>
#include <assert.h>
#include "../screen.h"
#include "pico/stdlib.h"

namespace spi_buffer
{

    SPI_Buffer::SPI_Buffer()
    {
        frameCounter_ = 0;
        allocateBuffers();
        I_initScreen();
    }

    void
    SPI_Buffer::spi_task()
    {
        int line = -1;

        if (validLineQueue_.size()) {
            const auto& item = validLineQueue_.deque();
            line = item.line;
            currentLineBuffer_ = std::move(item.buffer);
            if (line <= 8) {
                I_handleFrameStart(frameCounter_);
            } 

            I_handleScanline(currentLineBuffer_->data(), line-8);
            
            // // TODO HORRENDOUS HACK
            if (frameCounter_ < 100) {
                // sleep_ms(1);
            }
            
            
            if (line == 231) { // was 239 but I can't get it to work
                I_handleFrameEnd(frameCounter_++);
            }

            freeLineQueue_.enque(std::move(currentLineBuffer_));
        }
    }

    void
    SPI_Buffer::allocateBuffers()
    {
        {
            // alignment が4なのを期待する…
            for (auto &linebuffer : lineBuffers_)
            {
                linebuffer.resize(256);
                freeLineQueue_.enque(&linebuffer);
            }
        }
    }

    SPI_Buffer::LineBuffer *
    SPI_Buffer::getLineBuffer()
    {
        return freeLineQueue_.deque();
    }

    void
    SPI_Buffer::setLineBuffer(int line, LineBuffer *p)
    {
        validLineQueue_.enque({line, p});
    }

    void
    SPI_Buffer::waitForValidLine()
    {
        validLineQueue_.waitUntilContentAvailable();
    }
} 