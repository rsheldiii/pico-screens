/*
 * author : Shuichi TAKANO
 * since  : Sun Jun 20 2021 03:34:30
 */
#ifndef SPI_BUFFER_H_
#define SPI_BUFFER_H_

#include <vector>
#include <memory>
#include <hardware/sync.h>
#include <pico/util/queue.h>
#include <hardware/timer.h> // For timer IRQ
#include <hardware/irq.h>   // For IRQ handling

#include "../screen.h" // For screen dimensions etc.
#include "InfoNES_Types.h" // For WORD

#include "../screen_output.h" // Include the base class

// Forward declarations (Remove old DVI ones)
// namespace dvi {
//     struct Config;
//     struct Timing;
//     struct AudioSample;
//     template <typename T, size_t N> class RingBuffer;
// }

namespace spi_buffer
{
    class SPI_Buffer final : public ScreenOutput // Inherit from ScreenOutput
    {
    public:
        using LineBuffer = ScreenOutput::LineBuffer;

        SPI_Buffer();
        ~SPI_Buffer() override;



        // --- Core Methods (Implement base) ---
        LineBuffer *getLineBuffer() override;
        void setLineBuffer(int line, LineBuffer *p) override;
        void waitForValidLine() override;
        uint32_t getFrameCounter() const override;
        // --- Control Methods (Implement base) ---
        void registerIRQThisCore() override;
        void unregisterIRQThisCore() override;
        void start() override;
        void stop() override;

        // --- Configuration Methods (Implement base) ---
        ScreenOutput::BlankSettings& getBlankSettings() override { return blankSettings_; }
        void setScanLine(bool enable) override { /* Stub */ (void)enable; }
        void handleLine(LineBuffer *lineBuffer, int line);

        // --- Audio Methods (Implement base - Stubs) ---
        void setAudioFreq(int freq, int CTS, int N) override { /* Stub */ (void)freq; (void)CTS; (void)N; }
        void allocateAudioBuffer(size_t size) override { /* Stub */ (void)size; }
        util::RingBuffer<ScreenOutput::AudioSample>& getAudioRingBuffer() override;

        // --- Conversion methods (Implement base - Stubs/Internal) ---
        void convertScanBuffer12bpp() override { /* Stub - Handled by IRQ */ }
        void convertScanBuffer12bppScaled16_7(int srcPixelOfs, int dstPixelOfs, int dstPixels) override { /* Stub - Handled by IRQ */ (void)srcPixelOfs; (void)dstPixelOfs; (void)dstPixels; }
        void convertScanBuffer12bpp(uint16_t line, uint16_t *buffer, size_t size) override { /* Stub - Handled by IRQ */ (void)line; (void)buffer; (void)size; }
        void convertScanBuffer12bppScaled16_7(int srcPixelOfs, int dstPixelOfs, int dstPixels, uint16_t line, uint16_t *buffer, size_t size) override { /* Stub - Handled by IRQ */ (void)srcPixelOfs; (void)dstPixelOfs; (void)dstPixels; (void)line; (void)buffer; (void)size;}

        // --- Public IRQ Handler --- 
        void irqHandler(); // Instance method for ISR logic (now public)

    private:
        void allocateBuffers(); // Add declaration
        static constexpr int N_BUFFERS = 2;
        LineBuffer* debugCurrentLineBuffer_;
        LineBuffer lineBuffers_[N_BUFFERS];

        queue_t freeLineQueue_{};
        struct ValidLineEntry {
            int line;
            LineBuffer *buffer;
        };
        queue_t validLineQueue_{};

        LineBuffer *currentLineBuffer_{};

        uint32_t frameCounter_ = 0;
        // void I_initScreen();
        // void I_handleFrameStart(uint32_t frame);
        // void I_handleScanline(uint16_t *buffer, int line);
        // void I_handleFrameEnd(uint32_t frame);

        bool started_ = false;
        uint timerIRQ_ = 0; // Which hardware timer IRQ to use (e.g., TIMER_IRQ_0)
        uint alarmNum_ = 0; // Which alarm number to use
        repeating_timer_t repeatingTimer_;
        static void staticIRQHandler(); // Keep this for the C-style callback

        ScreenOutput::BlankSettings blankSettings_;
        ScreenOutput::AudioSample* dummyAudioSamples_ = nullptr;
        util::RingBuffer<ScreenOutput::AudioSample>* dummyAudioRingBuffer_ = nullptr;
    };
} // namespace spi_buffer

#endif // SPI_BUFFER_H_ 