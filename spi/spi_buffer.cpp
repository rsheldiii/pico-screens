#include "spi_buffer.h"
#include <pico.h>
#include <hardware/pio.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <stdio.h>
#include <assert.h>
#include "../screen.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "hardware/irq.h"
#include "hardware/timer.h"
#include <hardware/clocks.h>


namespace spi_buffer
{
    static SPI_Buffer *irqInstance_ = nullptr;

    bool repeating_timer_callback(struct repeating_timer *t)
    {
        if (irqInstance_) {
            irqInstance_->irqHandler();
        }
        return true;
    }

    SPI_Buffer::SPI_Buffer()
    {
        frameCounter_ = 0;
        queue_init(&freeLineQueue_, sizeof(LineBuffer*), N_BUFFERS);
        queue_init(&validLineQueue_, sizeof(ValidLineEntry), N_BUFFERS);
        debugCurrentLineBuffer_ = new LineBuffer(PICO_SCREENS_WIDTH);
        allocateBuffers();
        I_initScreen();

        dummyAudioSamples_ = new ScreenOutput::AudioSample[2];
        dummyAudioRingBuffer_ = new util::RingBuffer<ScreenOutput::AudioSample>();
        dummyAudioRingBuffer_->setBuffer(dummyAudioSamples_, 2);
        printf("SPI_Buffer constructor done\n");

        irqInstance_ = this;
    }

    SPI_Buffer::~SPI_Buffer()
    {
        stop();
        // unregisterIRQThisCore();
        irqInstance_ = nullptr;
        queue_free(&freeLineQueue_);
        queue_free(&validLineQueue_);

        delete dummyAudioRingBuffer_;
        delete[] dummyAudioSamples_;
    }

    void
    SPI_Buffer::allocateBuffers()
    {
        {
            for (auto &linebuffer : lineBuffers_)
            {
                linebuffer.resize(PICO_SCREENS_WIDTH);
                LineBuffer* p = &linebuffer;
                queue_add_blocking(&freeLineQueue_, &p);
            }
        }
    }

    SPI_Buffer::LineBuffer *
    SPI_Buffer::getLineBuffer()
    {
        return debugCurrentLineBuffer_;
        // LineBuffer* p = nullptr;
        // queue_remove_blocking(&freeLineQueue_, &p);
        // return p;
    }

    void
    SPI_Buffer::setLineBuffer(int line, LineBuffer *p)
    {
        ValidLineEntry entry = {line, p};
        handleLine(p, line);
        // queue_add_blocking(&validLineQueue_, &entry);
    }

    void
    SPI_Buffer::waitForValidLine()
    {
        queue_peek_blocking(&validLineQueue_, nullptr);
    }

    uint32_t SPI_Buffer::getFrameCounter() const
    {
        return frameCounter_;
    }

    void SPI_Buffer::registerIRQThisCore()
    {
        if (!add_repeating_timer_us(-69, repeating_timer_callback, NULL, &repeatingTimer_))
        {
            printf("Failed to add repeating timer for SPI buffer!\n");
        }
    }

    void SPI_Buffer::unregisterIRQThisCore()
    {
        cancel_repeating_timer(&repeatingTimer_);
    }

    void SPI_Buffer::start()
    {
        if (!started_)
        {
            frameCounter_ = 0;
            // registerIRQThisCore();
            started_ = true;
        }
    }

    void SPI_Buffer::stop()
    {
        if (started_)
        {
            // unregisterIRQThisCore();
            started_ = false;
        }
    }

    void SPI_Buffer::handleLine(LineBuffer *lineBuffer, int line)
    {
        int displayLine = line; // - blankSettings_.top;

        if (displayLine == 0) {
            I_handleFrameStart(frameCounter_);
        }

        if (displayLine >= 0 && displayLine < PICO_SCREENS_HEIGHT) {
            I_handleScanline(lineBuffer->data(), displayLine);
        }

        if (displayLine == PICO_SCREENS_HEIGHT - 1) {
            I_handleFrameEnd(frameCounter_++);
        }
    }

    void SPI_Buffer::irqHandler()
    {
        if (!started_ || !queue_get_level(&validLineQueue_)) {
            return;
        }

        ValidLineEntry item;
        if (queue_try_remove(&validLineQueue_, &item))
        {
            int line = item.line;
            currentLineBuffer_ = item.buffer;

            handleLine(currentLineBuffer_, line);

            queue_add_blocking(&freeLineQueue_, &currentLineBuffer_);
            currentLineBuffer_ = nullptr;
        }
    }

    util::RingBuffer<ScreenOutput::AudioSample>& SPI_Buffer::getAudioRingBuffer()
    {
        if (!dummyAudioRingBuffer_) {
            dummyAudioSamples_ = new ScreenOutput::AudioSample[2];
            dummyAudioRingBuffer_ = new util::RingBuffer<ScreenOutput::AudioSample>();
            dummyAudioRingBuffer_->setBuffer(dummyAudioSamples_, 2);
        }
        return *dummyAudioRingBuffer_;
    }

    // Remove redundant definitions below (already defined inline in header)
/*
    void SPI_Buffer::convertScanBuffer12bpp() override { / * Stub * / }
    void SPI_Buffer::convertScanBuffer12bppScaled16_7(int srcPixelOfs, int dstPixelOfs, int dstPixels) override { / * Stub * / (void)srcPixelOfs; (void)dstPixelOfs; (void)dstPixels; }
    void SPI_Buffer::convertScanBuffer12bpp(uint16_t line, uint16_t *buffer, size_t size) override { / * Stub * / (void)line; (void)buffer; (void)size; }
    void SPI_Buffer::convertScanBuffer12bppScaled16_7(int srcPixelOfs, int dstPixelOfs, int dstPixels, uint16_t line, uint16_t *buffer, size_t size) override { / * Stub * / (void)srcPixelOfs; (void)dstPixelOfs; (void)dstPixels; (void)line; (void)buffer; (void)size;}
*/
} 